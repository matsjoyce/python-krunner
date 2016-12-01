#include "python-krunner.hpp"
#include "convert-krunner.hpp"

#include <QObject>
#include <QDebug>
#include <QtGlobal>
#include <QIcon>
#include <Python.h>
#include <dlfcn.h>
#include <string>
#include <KF5/KCoreAddons/kexportplugin.h>
#include <KF5/KCoreAddons/kpluginfactory.h>


namespace python = boost::python;
using namespace Plasma;


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
    PyObject *exc,*val,*tb;
    PyErr_Fetch(&exc, &val, &tb);
    PyErr_NormalizeException(&exc, &val, &tb);
    python::handle<> hexc(exc), hval(python::allow_null(val)), htb(python::allow_null(tb));
    if (!hval) {
        return python::extract<std::string>(python::str(hexc));
    }
    else {
        auto traceback = python::import("traceback");
        auto format_exception = traceback.attr("format_exception");
        auto formatted_list = format_exception(hexc, hval, htb);
        auto formatted = python::str("").join(formatted_list);
        return python::extract<std::string>(formatted);
    }
}

void handle_exception() {
    qWarning("%s", extractException().c_str());
}

// Public Methods

PythonRunner::PythonRunner(QObject* parent, std::string fname_, const QVariantList& args)
    : AbstractRunner(parent, args), fname(fname_) {
    setIgnoredTypes(RunnerContext::NetworkLocation |
                    RunnerContext::Executable |
                    RunnerContext::ShellCommand);
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

    qDebug() << "Init obj..." << args;
    try {
        pyobj = main_namespace["Runner"](python::object(), convert_qvariantlist(args));
//         pyobj = main_namespace["Runner"];
    }
    catch (python::error_already_set) {
        handle_exception();
    }

    if (pyobj) {
        // TODO forward signals
    }
    qDebug() << "Done initing obj...";
}

PythonRunner::~PythonRunner() {
    // Destroy here under the GIL to avoid segfaults
    auto gil = GILRAII();
    pyobj = python::object();
    qDebug() << "Gone...";
}

void PythonRunner::match(RunnerContext& context) {
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::match(context);
    }
    try {
        pyobj.attr("match")(convert_sip<RunnerContext>(&context));
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::match(context);
}

void PythonRunner::createRunOptions(QWidget* widget) {
    // TODO
    qDebug() << "CRO";
//     auto gil = GILRAII();
//     if (!pyobj) {
//         return;
//     }
//     try {
//         pyobj.attr("createRunOptions")(convert_sip<RunnerContext>(&context));
//     }
//     catch (python::error_already_set) {
//         handle_exception();
//     }
    return AbstractRunner::createRunOptions(widget);
}

void PythonRunner::run(const RunnerContext& context, const QueryMatch& match) {
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::run(context, match);
    }
    try {
        pyobj.attr("run")(convert_sip(const_cast<RunnerContext*>(&context)),
                          convert_sip_transfer(new QueryMatch(match)));
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::run(context, match);
}

QStringList PythonRunner::categories() const {
    qDebug() << "C";
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::categories();
    }
    try {
        python::object strlist = pyobj.attr("categories")();
        auto ret = QStringList();
        auto end = python::stl_input_iterator<std::string>();
        for (auto iter = python::stl_input_iterator<std::string>(strlist); iter != end; ++iter) {
            ret << QString::fromStdString(*iter);
        }
        qDebug() << "C" << ret;
        return ret;
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::categories();
}

QIcon PythonRunner::categoryIcon(const QString& category) const {
    qDebug() << "CI";
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::categoryIcon(category);
    }
    try {
        python::object icon = pyobj.attr("categoryIcon")(category.toStdString());
        auto qi= QIcon(*extract_sip<QIcon>(icon.ptr()));

        qDebug() << "CI" << category << qi;
        return qi;
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::categoryIcon(category);
}

void PythonRunner::reloadConfiguration() {
    qDebug() << "RC";
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::reloadConfiguration();
    }
    try {
        python::object icon = pyobj.attr("reloadConfiguration")();
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::reloadConfiguration();
}

// Protected Methods

QList<QAction*> PythonRunner::actionsForMatch(const QueryMatch &match) {
    // TODO
    return AbstractRunner::actionsForMatch(match);
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
}

QMimeData* PythonRunner::mimeDataForMatch(const QueryMatch &match) {
    // TODO
    return AbstractRunner::mimeDataForMatch(match);
}

// Plugin discovery

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
        qDebug() << "python-krunner 0.1 loading:" << keyword;
        return new PythonRunner(parent, keyword.toStdString(), args);
    }
};

K_EXPORT_PLUGIN_VERSION(PLASMA_VERSION)

#include "python-krunner.moc"
