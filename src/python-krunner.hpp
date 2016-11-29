#ifndef PYRUNNER_H
#define PYRUNNER_H

#include <krunner/abstractrunner.h>
#include <boost/python.hpp>
#include <string>


class PythonRunner : public Plasma::AbstractRunner {
    Q_OBJECT

public:
    PythonRunner(QObject* parent, std::string fname_, const QVariantList& args);

    void match(Plasma::RunnerContext& context);
    void run(const Plasma::RunnerContext& context, const Plasma::QueryMatch& match);
    void createRunOptions(QWidget* widget);
    void reloadConfiguration();

protected Q_SLOTS:
    void init();
    void prepareForMatchSession();
    void matchSessionFinished();

private:
    std::string fname;
    boost::python::object pyobj;
};

#endif // PYRUNNER_H
