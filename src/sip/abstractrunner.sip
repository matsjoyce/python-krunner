namespace Plasma {
class AbstractRunner : public QObject {
    %TypeHeaderCode
    #include "krunner/abstractrunner.h"
    %End

    public:
        enum Speed {
            SlowSpeed,
            NormalSpeed
        };

        enum Priority {
            LowestPriority = 0,
            LowPriority,
            NormalPriority,
            HighPriority,
            HighestPriority
        };

        virtual ~AbstractRunner();

        virtual void match(Plasma::RunnerContext &context);
        void performMatch(Plasma::RunnerContext &context);
        bool hasRunOptions();
        virtual void createRunOptions(QWidget *widget);
        virtual void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match);
        virtual QStringList categories() const;
        virtual QIcon categoryIcon(const QString& category) const;
        Speed speed() const;
        Priority priority() const;
        Plasma::RunnerContext::Types ignoredTypes() const;
        void setIgnoredTypes(Plasma::RunnerContext::Types types);
        QString name() const;
        QString id() const;
        QString description() const;
//         KPluginInfo metadata() const;
        QIcon icon() const;
//         Package package() const;
        virtual void reloadConfiguration();
        QList<Plasma::RunnerSyntax> syntaxes() const;
        Plasma::RunnerSyntax *defaultSyntax() const;
        bool isMatchingSuspended() const;

    Q_SIGNALS:
        void prepare();
        void teardown();
        void matchingSuspended(bool suspended);

    protected:
        explicit AbstractRunner(QObject *parent = 0, const QString &path = QString());
        AbstractRunner(QObject *parent, const QVariantList &args);
        void suspendMatching(bool suspend);
//         KConfigGroup config() const;
        void setHasRunOptions(bool hasRunOptions);
        void setSpeed(Speed newSpeed);
        void setPriority(Priority newPriority);
        virtual QList<QAction*> actionsForMatch(const Plasma::QueryMatch &match);
        QAction* addAction(const QString &id, const QIcon &icon, const QString &text);
        void addAction(const QString &id, QAction *action);
        void removeAction(const QString &id);
        QAction* action(const QString &id) const;
        QHash<QString, QAction*> actions() const;
        void clearActions();
        void addSyntax(const Plasma::RunnerSyntax &syntax);
        void setDefaultSyntax(const Plasma::RunnerSyntax &syntax);
        void setSyntaxes(const QList<Plasma::RunnerSyntax> &syns);
//         DataEngine *dataEngine(const QString &name) const;
        virtual void init();
        virtual QMimeData *mimeDataForMatch(const Plasma::QueryMatch &match);
};
};
