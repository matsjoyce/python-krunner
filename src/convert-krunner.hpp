#ifndef CONVERT_KRUNNER_HPP
#define CONVERT_KRUNNER_HPP

#include "sipAPI_krunner.h"
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

template<class T> const char* classname();


template<class T> boost::python::handle<PyObject> convert_sip(T* obj) {
    auto sip_api = get_sip_api(); // TODO maybe make this a global-level static
    auto type_obj = sip_api->api_find_type(classname<T>());
    if (!type_obj) {
        qWarning("Type %s not found", classname<T>());
        throw boost::python::error_already_set();
    }
    auto res = sip_api->api_convert_from_type(static_cast<void *>(obj), type_obj, NULL);
    return boost::python::handle<PyObject>(res);
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


// Types

template<> const char* classname<Plasma::AbstractRunner>() {return "Plasma::AbstractRunner";}

template<> const char* classname<Plasma::QueryMatch>() {return "Plasma::QueryMatch";}

template<> const char* classname<Plasma::RunnerContext>() {return "Plasma::RunnerContext";}

template<> const char* classname<Plasma::RunnerSyntax>() {return "Plasma::RunnerSyntax";}

#endif // CONVERT_KRUNNER_HPP
