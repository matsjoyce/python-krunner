class KConfigGroup : public KConfigBase {
    %TypeHeaderCode
    #include "kconfiggroup.h"
    %End

public:
    KConfigGroup();
    KConfigGroup(KConfigBase *master, const QString &group);
    KConfigGroup(KConfigBase *master, const char *group);
    KConfigGroup(const KConfigGroup &);
    ~KConfigGroup();
    bool isValid() const;
    QString name() const;
    bool exists() const;
    bool sync();
    void markAsClean();
    AccessMode accessMode() const;
    KConfig *config();
    void copyTo(KConfigBase *other, WriteConfigFlags pFlags = Normal) const;
    void reparent(KConfigBase *parent, WriteConfigFlags pFlags = Normal);
    KConfigGroup parent() const;
    QStringList groupList() const;
    QStringList keyList() const;
    void deleteGroup(WriteConfigFlags pFlags = Normal);
    QVariant readEntry(const QString &key, const QVariant &aDefault) const;
    QVariant readEntry(const char *key, const QVariant &aDefault) const;
    QString readEntry(const QString &key, const QString &aDefault) const;
    QString readEntry(const char *key, const QString &aDefault) const;
    QString readEntry(const QString &key, const char *aDefault = Q_NULLPTR) const;
    QString readEntry(const char *key, const char *aDefault = Q_NULLPTR) const;
    QVariantList readEntry(const QString &key, const QVariantList &aDefault) const;
    QVariantList readEntry(const char *key, const QVariantList &aDefault) const;
    QStringList readEntry(const QString &key, const QStringList &aDefault) const;
    QStringList readEntry(const char *key, const QStringList &aDefault) const;
    QStringList readXdgListEntry(const QString &pKey, const QStringList &aDefault = QStringList()) const;
    QStringList readXdgListEntry(const char *pKey, const QStringList &aDefault = QStringList()) const;
    QString readPathEntry(const QString &pKey, const QString &aDefault) const;
    QString readPathEntry(const char *key, const QString &aDefault) const;
    QStringList readPathEntry(const QString &pKey, const QStringList &aDefault) const;
    QStringList readPathEntry(const char *key, const QStringList &aDefault) const;
    QString readEntryUntranslated(const QString &pKey,
                                  const QString &aDefault = QString()) const;
    QString readEntryUntranslated(const char *key,
                                  const QString &aDefault = QString()) const;
    void writeEntry(const QString &key, const QVariant &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const char *key, const QVariant &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const QString &key, const QString &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const char *key, const QString &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const QString &key, const QByteArray &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const char *key, const QByteArray &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const QString &key, const char *value, WriteConfigFlags pFlags = Normal);
    void writeEntry(const char *key, const char *value, WriteConfigFlags pFlags = Normal);
    void writeEntry(const QString &key, const QStringList &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const char *key, const QStringList &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const QString &key, const QVariantList &value,
                    WriteConfigFlags pFlags = Normal);
    void writeEntry(const char *key, const QVariantList &value,
                    WriteConfigFlags pFlags = Normal);
    void writeXdgListEntry(const QString &pKey, const QStringList &value,
                           WriteConfigFlags pFlags = Normal);
    void writeXdgListEntry(const char *pKey, const QStringList &value,
                           WriteConfigFlags pFlags = Normal);
    void writePathEntry(const QString &pKey, const QString &path,
                        WriteConfigFlags pFlags = Normal);
    void writePathEntry(const char *pKey, const QString &path,
                        WriteConfigFlags pFlags = Normal);
    void writePathEntry(const QString &pKey, const QStringList &value,
                        WriteConfigFlags pFlags = Normal);
    void writePathEntry(const char *pKey, const QStringList &value,
                        WriteConfigFlags pFlags = Normal);
    void deleteEntry(const QString &pKey, WriteConfigFlags pFlags = Normal);
    void deleteEntry(const char *pKey, WriteConfigFlags pFlags = Normal);
    bool hasKey(const QString &key) const;
    bool hasKey(const char *key) const;
    bool isImmutable() const;
    bool isEntryImmutable(const QString &key) const;
    bool isEntryImmutable(const char *key) const;
    void revertToDefault(const QString &key);
    void revertToDefault(const char *key);
    bool hasDefault(const QString &key) const;
    bool hasDefault(const char *key) const;
    QMap<QString, QString> entryMap() const;
};
