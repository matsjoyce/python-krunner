namespace Plasma {
class RunnerContext : public QObject{
    %TypeHeaderCode
    #include "krunner/runnercontext.h"
    %End

    public:
        enum Type {
            None = 0,
            UnknownType = 1,
            Directory = 2,
            File = 4,
            NetworkLocation = 8,
            Executable = 16,
            ShellCommand = 32,
            Help = 64,
            FileSystem = 54
        };
        typedef QFlags<Plasma::RunnerContext::Type> Types;

        explicit RunnerContext(QObject *parent = 0);
        RunnerContext(Plasma::RunnerContext &other, QObject *parent = 0);
//         RunnerContext &operator=(const Plasma::RunnerContext &other);
        ~RunnerContext();
        void reset();
        void setQuery(const QString &term);
        QString query() const;
        Type type() const;
        QStringList enabledCategories() const;
        void setEnabledCategories(const QStringList &categories);
        QString mimeType() const;
        bool isValid() const;
        bool addMatches(const QList<Plasma::QueryMatch> &matches);
        bool addMatch(const Plasma::QueryMatch &match);
        bool removeMatch(const QString matchId);
        bool removeMatches(const QStringList matchIdList);
        bool removeMatches(Plasma::AbstractRunner *runner);
        QList<Plasma::QueryMatch> matches() const;
        Plasma::QueryMatch match(const QString &id) const;
        void setSingleRunnerQueryMode(bool enabled);
        bool singleRunnerQueryMode() const;
//         void restore(const KConfigGroup &config);
//         void save(KConfigGroup &config);
        void run(const Plasma::QueryMatch &match);

    Q_SIGNALS:
        void matchesChanged();
};
};
