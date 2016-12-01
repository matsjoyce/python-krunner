#ifndef CONVERT_KRUNNER_HPP
#define CONVERT_KRUNNER_HPP

#include "sip.h"
#include <krunner/abstractrunner.h>
#include <boost/python.hpp>
#include <QtGlobal>
#include <QDebug>


// http://pyqt.sourceforge.net/Docs/sip4/embedding.html
const sipAPIDef* get_sip_api()
{
#if defined(SIP_USE_PYCAPSULE)
    return (const sipAPIDef *)PyCapsule_Import("sip._C_API", 0);
#else
    PyObject *sip_module;
    PyObject *sip_module_dict;
    PyObject *c_api;

    /* Import the SIP module. */
    sip_module = PyImport_ImportModule("sip");

    if (sip_module == NULL)
        return NULL;

    /* Get the module's dictionary. */
    sip_module_dict = PyModule_GetDict(sip_module);

    /* Get the "_C_API" attribute. */
    c_api = PyDict_GetItemString(sip_module_dict, "_C_API");

    if (c_api == NULL)
        return NULL;

    /* Sanity check that it is the right type. */
    if (!PyCObject_Check(c_api))
        return NULL;

    /* Get the actual pointer from the object. */
    return (const sipAPIDef *)PyCObject_AsVoidPtr(c_api);
#endif
}

template<class T> const char* classname() {return T::unknown_type_needs_classname_specialisation;}

template<class T> boost::python::handle<PyObject> _convert_sip(T* obj, PyObject* transfer) {
    auto sip_api = get_sip_api(); // TODO maybe make this a global-level static
    auto type_obj = sip_api->api_find_type(classname<T>());
    if (!type_obj) {
        qWarning("Type %s not found", classname<T>());
        throw boost::python::error_already_set();
    }
    auto res = sip_api->api_convert_from_type(static_cast<void *>(obj), type_obj, transfer);
    return boost::python::handle<PyObject>(res);
}

template<class T> boost::python::handle<PyObject> convert_sip(T* obj) {
    return _convert_sip(obj, NULL);
}

template<class T> boost::python::handle<PyObject> convert_sip_transfer(T* obj) {
    return _convert_sip(obj, Py_None);
}

template<class T> T* extract_sip(boost::python::object obj) {
    auto sip_api = get_sip_api(); // TODO maybe make this a global-level static
    auto type_obj = sip_api->api_find_type(classname<T>());
    if (!type_obj) {
        qWarning("Type %s not found", classname<T>());
        throw boost::python::error_already_set();
    }
    int err = 0;
    auto res = sip_api->api_force_convert_to_type(obj.ptr(), type_obj, NULL, SIP_NOT_NONE, NULL, &err);
    if (err) {
        qWarning("Conversion to %s failed", classname<T>());
        throw boost::python::error_already_set();
    }
    return reinterpret_cast<T*>(res);
}

boost::python::object convert_qvariantlist(const QVariantList& qva) {
    typedef PyObject* (*functype)(QVariant &value, PyObject *type);
    auto convert = reinterpret_cast<functype>(get_sip_api()->api_import_symbol("pyqt5_from_qvariant_by_type"));
    auto converted = boost::python::list();
    for(const QVariant& v : qva) {
        QVariant v_copy = v;
        converted.append(boost::python::handle<PyObject>(convert(v_copy, NULL)));
    }
    return converted;
}

bool _connect_sip(boost::python::object signal, const QObject* reciever, const char* method, bool reverse) {
    typedef sipErrorState (*functype)(PyObject *signal, QObject **transmitter, QByteArray &signal_signature);
    auto convert = reinterpret_cast<functype>(get_sip_api()->api_import_symbol("pyqt5_get_pyqtsignal_parts"));
    QObject* sender;
    QByteArray signature;
    sipErrorState state = convert(signal.ptr(), &sender, signature);
    if (state == sipErrorFail) {
        qWarning("Conversion failed");
        throw boost::python::error_already_set();
    }
    if (reverse) {
        return QObject::connect(reciever, method, sender, signature);
    }
    return QObject::connect(sender, signature, reciever, method);
}

bool connect_sip(boost::python::object signal, const QObject* reciever, const char* method) {
    return _connect_sip(signal, reciever, method, false);
}

bool rconnect_sip(boost::python::object signal, const QObject* reciever, const char* method) {
    return _connect_sip(signal, reciever, method, true);
}

// Types

template<> const char* classname<Plasma::AbstractRunner>() {return "Plasma::AbstractRunner";}

template<> const char* classname<Plasma::QueryMatch>() {return "Plasma::QueryMatch";}

template<> const char* classname<Plasma::RunnerContext>() {return "Plasma::RunnerContext";}

template<> const char* classname<Plasma::RunnerSyntax>() {return "Plasma::RunnerSyntax";}

template<> const char* classname<QObject>() {return "QObject";}

template<> const char* classname<QIcon>() {return "QIcon";}

template<> const char* classname<QAction>() {return "QAction";}

template<> const char* classname<QMimeData>() {return "QAction";}

template<> const char* classname<QWidget>() {return "QWidget";}

#endif // CONVERT_KRUNNER_HPP
