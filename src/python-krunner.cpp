#include "python-krunner.hpp"
#include "convert-krunner.hpp"

#include <KF5/KCoreAddons/kexportplugin.h>
#include <QObject>
#include <QDebug>
#include <QtGlobal>
#include <QDir>
#include <Python.h>
#include <dlfcn.h>
#include <string>


namespace python = boost::python;


class GILRAII {
    PyGILState_STATE gstate;
public:
    GILRAII() {
        gstate = PyGILState_Ensure();
    }
    ~GILRAII() {
        PyGILState_Release(gstate);
    }
};

// https://wiki.python.org/moin/boost.python/EmbeddingPython
std::string extractException() {
    using namespace boost::python;

    PyObject *exc,*val,*tb;
    PyErr_Fetch(&exc,&val,&tb);
    PyErr_NormalizeException(&exc,&val,&tb);
    handle<> hexc(exc),hval(allow_null(val)),htb(allow_null(tb));
    if (!hval) {
        return extract<std::string>(str(hexc));
    }
    else {
        object traceback(import("traceback"));
        object format_exception(traceback.attr("format_exception"));
        object formatted_list(format_exception(hexc,hval,htb));
        object formatted(str("").join(formatted_list));
        return extract<std::string>(formatted);
    }
}

void handle_exception() {
    qWarning("%s", extractException().c_str());
}

PythonRunner::PythonRunner(QObject* parent, std::string fname_, const QVariantList& args)
    : AbstractRunner(parent, args), fname(fname_) {
    setIgnoredTypes(Plasma::RunnerContext::NetworkLocation |
                    Plasma::RunnerContext::Executable |
                    Plasma::RunnerContext::ShellCommand);
    setSpeed(SlowSpeed);
    setPriority(LowPriority);
    setHasRunOptions(false);

    auto gil = GILRAII();
    python::object main_namespace;
    try {
        python::object main_module = python::import("__main__");
        main_namespace = main_module.attr("__dict__");
        python::exec_file(python::str(fname), main_namespace, main_namespace);
    }
    catch (python::error_already_set) {
        handle_exception();
        return;
    }

    try {
        pyobj = main_namespace["Runner"](convert_sip<Plasma::AbstractRunner>(this), convert_qvariantlist(args));
    }
    catch (python::error_already_set) {
        handle_exception();
    }
}

void PythonRunner::init() {
    reloadConfiguration();

    auto gil = GILRAII();
    if (!pyobj) {
        return;
    }
    try {
        pyobj.attr("init")();
    }
    catch (python::error_already_set) {
        handle_exception();
    }

    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
    connect(this, SIGNAL(teardown()), this, SLOT(matchSessionFinished()));
}

void PythonRunner::reloadConfiguration() {

}

void PythonRunner::prepareForMatchSession() {

}

void PythonRunner::match(Plasma::RunnerContext& context) {
    auto gil = GILRAII();
    if (!pyobj) {
        return;
    }
    auto ctx = convert_sip<Plasma::RunnerContext>(&context);

    try {
        pyobj.attr("match")(ctx);
    }
    catch (python::error_already_set) {
        handle_exception();
    }
}

void PythonRunner::matchSessionFinished() {

}

void PythonRunner::run(const Plasma::RunnerContext& context, const Plasma::QueryMatch& match) {

}

void PythonRunner::createRunOptions(QWidget* widget) {

}

class factory : public KPluginFactory {
    Q_OBJECT
    Q_INTERFACES(KPluginFactory)
    Q_PLUGIN_METADATA(IID KPluginFactory_iid)

public:
    explicit factory() {
        // https://bugs.python.org/issue19153
        dlopen("libpython3.5m.so", RTLD_LAZY | RTLD_GLOBAL);
        Py_InitializeEx(0);
        PyEval_InitThreads();
        PyEval_SaveThread();
    }
    ~factory() {

    }

protected:
    virtual QObject *create(const char* iface, QWidget* /*parentWidget*/, QObject* parent,
                            const QVariantList& args, const QString& keyword) {
        qDebug() << "python-krunner loading:" << keyword;
        return new PythonRunner(parent, keyword.toStdString(), args);
    }
};

K_EXPORT_PLUGIN_VERSION(PLASMA_VERSION)

#include "python-krunner.moc"
