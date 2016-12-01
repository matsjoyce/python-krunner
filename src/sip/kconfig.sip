class KConfig : public KConfigBase /NoDefaultCtors/ {
    %TypeHeaderCode
    #include "kconfig.h"
    %End

public:
    enum OpenFlag {
        IncludeGlobals  = 0x01,
        CascadeConfig   = 0x02,
        SimpleConfig    = 0x00,
        NoCascade       = 0x01,
        NoGlobals       = 0x02,
        FullConfig      = 0x03
    };
    typedef QFlags<KConfig::OpenFlag> OpenFlags;
    explicit KConfig(const QString &file = QString(), OpenFlags mode = FullConfig,
                     QStandardPaths::StandardLocation type = QStandardPaths::GenericConfigLocation);
    KConfig(const QString &file, const QString &backend, QStandardPaths::StandardLocation type = QStandardPaths::GenericConfigLocation);
    virtual ~KConfig();
    QStandardPaths::StandardLocation locationType() const;
    QString name() const;
    OpenFlags openFlags() const;
    bool sync();
    bool isDirty() const;
    void markAsClean();
    AccessMode accessMode() const;
    bool isConfigWritable(bool warnUser);
    KConfig *copyTo(const QString &file, KConfig *config = Q_NULLPTR) const;
    void checkUpdate(const QString &id, const QString &updateFile);
    void reparseConfiguration();
    void addConfigSources(const QStringList &sources);
    QString locale() const;
    bool setLocale(const QString &aLocale);
    void setReadDefaults(bool b);
    bool readDefaults() const;
    bool isImmutable() const;
    QStringList groupList() const;
    QMap<QString, QString> entryMap(const QString &aGroup = QString()) const;
    static void setMainConfigName(const QString &str);
};
