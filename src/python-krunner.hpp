#ifndef PYRUNNER_H
#define PYRUNNER_H

#include <krunner/abstractrunner.h>
#include <krunner/querymatch.h>
#include <krunner/runnercontext.h>
#include <krunner/runnersyntax.h>
#include <boost/python.hpp>
#include <string>


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
private:
    std::string fname;
    boost::python::object pyobj;
};

#endif // PYRUNNER_H
