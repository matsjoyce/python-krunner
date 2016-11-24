#include "pyrunner.hpp"

#include <QDebug>
#include <QDir>
#include <Python.h>

namespace python = boost::python;


void handle_exception() {
    PyErr_Print();
    try {
        python::object traceback = python::import("traceback");
        traceback.attr("print_exc")();
    }
    catch (python::error_already_set) {
    }
}


PyRunner::PyRunner(QObject* parent, std::string fname_, const QVariantList& args)
    : AbstractRunner(parent, args), fname(fname_) {
    setIgnoredTypes(Plasma::RunnerContext::NetworkLocation |
                    Plasma::RunnerContext::Executable |
                    Plasma::RunnerContext::ShellCommand);
    setSpeed(SlowSpeed);
    setPriority(LowPriority);
    setHasRunOptions(false);
}

void PyRunner::init() {
    reloadConfiguration();

    python::object main_namespace;
    try {
        python::object main_module = python::import("__main__");
        main_namespace = main_module.attr("__dict__");
        python::exec_file(python::str(fname), main_namespace, main_namespace);
    }
    catch (python::error_already_set) {
        handle_exception();
    }

    try {
        pyobj = main_namespace["Runner"]();
    }
    catch (python::error_already_set) {
        handle_exception();
    }

    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
    connect(this, SIGNAL(teardown()), this, SLOT(matchSessionFinished()));
}

void PyRunner::reloadConfiguration() {

}

void PyRunner::prepareForMatchSession() {

}

void PyRunner::match(Plasma::RunnerContext& context) {
    Plasma::QueryMatch match(this);
    match.setText(QString::fromStdString(boost::python::extract<std::string>(pyobj.attr("name"))));
    match.setData("a");
    match.setId("a");
    match.setIconName("text-x-python");
    context.addMatch(match);
}

void PyRunner::matchSessionFinished() {

}

void PyRunner::run(const Plasma::RunnerContext& context, const Plasma::QueryMatch& match) {

}

void PyRunner::createRunOptions(QWidget* widget) {

}

class factory : public KPluginFactory {
    Q_OBJECT
    Q_INTERFACES(KPluginFactory)
    Q_PLUGIN_METADATA(IID KPluginFactory_iid)
public:
    explicit factory() {
        Py_InitializeEx(0);
        qDebug() << "App path : " << QDir::currentPath();
    }
    ~factory() {

    }
protected:
    virtual QObject *create(const char* iface, QWidget* /*parentWidget*/, QObject* parent,
                            const QVariantList& args, const QString& keyword) {
        const QString identifier = QLatin1String(iface) + QLatin1Char('_') + keyword;
        qDebug() << identifier;
        return new PyRunner(parent, keyword.toStdString(), args);
    }
};

K_EXPORT_PLUGIN_VERSION(PLASMA_VERSION)

#include "pyrunner.moc"
