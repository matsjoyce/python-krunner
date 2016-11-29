namespace Plasma {
class QueryMatch {
    %TypeHeaderCode
    #include "krunner/querymatch.h"
    %End

    public:
        enum Type {
            NoMatch = 0,
            CompletionMatch = 10,
            PossibleMatch = 30,
            InformationalMatch = 50,
            HelperMatch = 70,
            ExactMatch = 100
        };

        explicit QueryMatch(Plasma::AbstractRunner *runner);
        QueryMatch(const Plasma::QueryMatch &other);
        ~QueryMatch();
//         QueryMatch &operator=(const QueryMatch &other);
        bool operator==(const Plasma::QueryMatch &other) const;
        bool operator!=(const Plasma::QueryMatch &other) const;
        bool operator<(const Plasma::QueryMatch &other) const;
        Plasma::AbstractRunner *runner() const;
        void run(const Plasma::RunnerContext &context) const;
        bool isValid() const;
        void setType(Type type);
        Type type() const;
        void setMatchCategory(const QString& category);
        QString matchCategory() const;
        void setRelevance(qreal relevance);
        qreal relevance() const;
        void setData(const QVariant &data);
        QVariant data() const;
        void setId(const QString &id);
        QString id() const;
        void setText(const QString &text);
        QString text() const;
        void setSubtext(const QString &text);
        QString subtext() const;
        void setIcon(const QIcon &icon);
        QIcon icon() const;
        void setIconName(const QString &iconName);
        QString iconName() const;
        void setMimeType(const QString &mimeType);
        QString mimeType() const;
        void setUrls(const QList<QUrl> &urls);
        QList<QUrl> urls() const;
        void setEnabled(bool enable);
        bool isEnabled() const;
        QAction* selectedAction() const;
        void setSelectedAction(QAction *action);
        bool hasConfigurationInterface() const;
        void createConfigurationInterface(QWidget *parent);
};
};
