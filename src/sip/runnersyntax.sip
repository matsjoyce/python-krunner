namespace Plasma {
class RunnerSyntax {
    %TypeHeaderCode
    #include "krunner/runnersyntax.h"
    %End

    public:
        RunnerSyntax(const QString &exampleQuery, const QString &description);
        RunnerSyntax(const Plasma::RunnerSyntax &other);
        ~RunnerSyntax();
//         RunnerSyntax &operator=(const RunnerSyntax &rhs);
        void addExampleQuery(const QString &exampleQuery);
        QStringList exampleQueries() const;
        QStringList exampleQueriesWithTermDescription() const;
        void setDescription(const QString &description);
        QString description() const;
        void setSearchTermDescription(const QString &description);
        QString searchTermDescription() const;
};
};
