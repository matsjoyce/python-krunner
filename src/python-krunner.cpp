#include "python-krunner.hpp"
#include "convert-krunner.hpp"
#include "python-krunner-conf.hpp"

#include <QObject>
#include <QDebug>
#include <QtGlobal>
#include <QIcon>
#include <QList>
#include <QAction>
#include <QFileInfo>
#include <Python.h>
#include <dlfcn.h>
#include <string>
#include <KF5/KCoreAddons/kexportplugin.h>
#include <KF5/KCoreAddons/kpluginfactory.h>


namespace python = boost::python;
using namespace Plasma;

Q_LOGGING_CATEGORY(LOG_PYTHON_KRUNNER, "plasma.krunner.python")

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
    qCWarning(LOG_PYTHON_KRUNNER) << extractException().c_str();
}

// Public Methods

PythonRunner::PythonRunner(QObject* parent, std::string fname_, const QVariantList& args) : AbstractRunner(parent, args),
                                                                                            fname(fname_) {
    auto gil = GILRAII();

    qCDebug(LOG_PYTHON_KRUNNER) << "Loading module...";
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

    qCDebug(LOG_PYTHON_KRUNNER) << "Creating object...";
    try {
        pyobj = main_namespace["Runner"](convert_sip<AbstractRunner>(this), convert_qvariantlist(args));

        bool all_connected = true;
        // Child -> Parent
        all_connected = all_connected && connect_sip(pyobj.attr("prepare"),
                                         this, SLOT(_child_prepare()));
        all_connected = all_connected && connect_sip(pyobj.attr("teardown"),
                                                     this, SLOT(_child_teardown()));
        all_connected = all_connected && connect_sip(pyobj.attr("matchingSuspended"),
                                                     this, SLOT(_child_matchingSuspended(bool)));
        if (!all_connected) {
            qCWarning(LOG_PYTHON_KRUNNER) << "Some signals failed to connect";
        }

        // Parent -> Child

        QObject::connect(this, SIGNAL(prepare()), this, SLOT(_parent_prepare()));
        QObject::connect(this, SIGNAL(teardown()), this, SLOT(_parent_teardown()));
        QObject::connect(this, SIGNAL(matchingSuspended(bool)), this, SLOT(_parent_matchingSuspended(bool)));
    }
    catch (python::error_already_set) {
        handle_exception();
    }
}

PythonRunner::~PythonRunner() {
    // Destroy here under the GIL to avoid segfaults
    auto gil = GILRAII();
    pyobj = python::object();
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
    auto gil = GILRAII();

    if (!pyobj) {
        return;
    }
    try {
        pyobj.attr("createRunOptions")(convert_sip<QWidget>(widget));
    }
    catch (python::error_already_set) {
        handle_exception();
    }
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
        return ret;
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::categories();
}

QIcon PythonRunner::categoryIcon(const QString& category) const {
    auto gil = GILRAII();

    if (!pyobj) {
        return AbstractRunner::categoryIcon(category);
    }
    try {
        python::object icon = pyobj.attr("categoryIcon")(category.toStdString());
        return QIcon(*extract_sip<QIcon>(icon));
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::categoryIcon(category);
}

void PythonRunner::reloadConfiguration() {
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::reloadConfiguration();
    }
    try {
        pyobj.attr("reloadConfiguration")();
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::reloadConfiguration();
}

// Protected Methods

QList<QAction*> PythonRunner::actionsForMatch(const QueryMatch &match) {
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::actionsForMatch(match);
    }
    try {
        python::object actionlist = pyobj.attr("actionsForMatch")(convert_sip_transfer(new QueryMatch(match)));
        auto ret = QList<QAction*>();
        auto end = python::stl_input_iterator<python::object>();
        for (auto iter = python::stl_input_iterator<python::object>(actionlist); iter != end; ++iter) {
            ret << extract_sip<QAction>(*iter);
        }
        return ret;
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::actionsForMatch(match);
}

void PythonRunner::init() {
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
    auto gil = GILRAII();
    if (!pyobj) {
        return AbstractRunner::mimeDataForMatch(match);
    }
    try {
        python::object icon = pyobj.attr("mimeDataForMatch")(convert_sip_transfer(new QueryMatch(match)));
        return extract_sip<QMimeData>(icon);
    }
    catch (python::error_already_set) {
        handle_exception();
    }
    return AbstractRunner::mimeDataForMatch(match);
}

// Slots

void PythonRunner::_child_prepare() {
    if (!prepare_signaling) {
        prepare_signaling = true;
        Q_EMIT prepare();
        prepare_signaling = false;
    }
}

void PythonRunner::_child_teardown() {
    if (!teardown_signaling) {
        teardown_signaling = true;
        Q_EMIT teardown();
        teardown_signaling = false;
    }
}

void PythonRunner::_child_matchingSuspended(bool suspended) {
    if (!matchingSuspended_signaling) {
        matchingSuspended_signaling = true;
        Q_EMIT matchingSuspended(suspended);
        matchingSuspended_signaling = false;
    }
}

void PythonRunner::_parent_prepare() {
    auto gil = GILRAII();
    if (!prepare_signaling) {
        prepare_signaling = true;
        pyobj.attr("prepare").attr("emit")();
        prepare_signaling = false;
    }
}

void PythonRunner::_parent_teardown() {
    auto gil = GILRAII();
    if (!teardown_signaling) {
        teardown_signaling = true;
        pyobj.attr("teardown").attr("emit")();
        teardown_signaling = false;
    }
}

void PythonRunner::_parent_matchingSuspended(bool suspended) {
    auto gil = GILRAII();
    if (!matchingSuspended_signaling) {
        matchingSuspended_signaling = true;
        pyobj.attr("matchingSuspended").attr("emit")(suspended);
        matchingSuspended_signaling = false;
    }
}

// Plugin discovery

class factory : public KPluginFactory {
    Q_OBJECT
    Q_INTERFACES(KPluginFactory)
    Q_PLUGIN_METADATA(IID KPluginFactory_iid)
    bool startup_succeeded = false;

public:
    explicit factory() {
        qCDebug(LOG_PYTHON_KRUNNER) << "Loading python";
        // https://bugs.python.org/issue19153
        dlopen(get_python_dl_location(), RTLD_LAZY | RTLD_GLOBAL);
        Py_InitializeEx(0);
        PyEval_InitThreads();
        qCDebug(LOG_PYTHON_KRUNNER) << "Loading modules";
        python::object sys, krunner;
        try {
            sys = python::import("sys");
        }
        catch (python::error_already_set) {
            qCDebug(LOG_PYTHON_KRUNNER) << "Loading modules failed";
            return;
        }
        try {
            krunner = python::import("krunner");
            sys.attr("excepthook") = krunner.attr("_except_hook");
        }
        catch (python::error_already_set) {
            qCDebug(LOG_PYTHON_KRUNNER) << "Loading modules failed";
            return;
        }

        PyEval_SaveThread();
        qCDebug(LOG_PYTHON_KRUNNER) << "Finished loading python";
        startup_succeeded = true;
    }
    ~factory() {

    }

protected:
    virtual QObject* create(const char* /*iface*/, QWidget* /*parentWidget*/, QObject* parent,
                            const QVariantList& args, const QString& keyword) {
        qCDebug(LOG_PYTHON_KRUNNER) << "Loading" << keyword;
        if (startup_succeeded) {
            QFileInfo file(keyword);
            if (!file.exists() || !file.isFile()) {
                qCWarning(LOG_PYTHON_KRUNNER) << "File does not exist";
                return nullptr;
            }
            return new PythonRunner(parent, keyword.toStdString(), args);
        }
        qCDebug(LOG_PYTHON_KRUNNER) << "Ignoring as startup failed";
        return nullptr;
    }
};

K_EXPORT_PLUGIN_VERSION(PLASMA_VERSION)

#include "python-krunner.moc"
