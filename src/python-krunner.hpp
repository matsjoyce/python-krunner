#ifndef PYTHON_KRUNNER_HPP
#define PYTHON_KRUNNER_HPP

#include <krunner/abstractrunner.h>
#include <krunner/querymatch.h>
#include <krunner/runnercontext.h>
#include <krunner/runnersyntax.h>
#include <boost/python.hpp>
#include <string>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LOG_PYTHON_KRUNNER)

class PythonRunner : public Plasma::AbstractRunner {
    Q_OBJECT

public:
    PythonRunner(QObject* parent, std::string fname_, const QVariantList& args);
    virtual ~PythonRunner();

    virtual void match(Plasma::RunnerContext &context);
    virtual void createRunOptions(QWidget *widget);
    virtual void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match);
    virtual QStringList categories() const;
    virtual QIcon categoryIcon(const QString& category) const;
    virtual void reloadConfiguration();

protected:
    virtual QList<QAction*> actionsForMatch(const Plasma::QueryMatch &match);
    virtual void init();
    virtual QMimeData* mimeDataForMatch(const Plasma::QueryMatch &match);

protected Q_SLOTS:
    void _child_prepare();
    void _child_teardown();
    void _child_matchingSuspended(bool suspended);

    void _parent_prepare();
    void _parent_teardown();
    void _parent_matchingSuspended(bool suspended);

private:
    std::string fname;
    boost::python::object pyobj;
    bool prepare_signaling = false;
    bool teardown_signaling = false;
    bool matchingSuspended_signaling = false;
};

#endif // PYTHON_KRUNNER_HPP
