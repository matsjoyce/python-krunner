class KConfigBase /NoDefaultCtors/ {
    %TypeHeaderCode
    #include "kconfiggroup.h"
    %End

public:
    enum WriteConfigFlag {
        Persistent = 0x01,
        Global = 0x02,
        Localized = 0x04,
        Normal = 0x1
    };
    typedef QFlags<KConfigBase::WriteConfigFlag> WriteConfigFlags;
    virtual ~KConfigBase();
    virtual QStringList groupList() const = 0;
    bool hasGroup(const QString &group) const;
    bool hasGroup(const char *group) const;
    bool hasGroup(const QByteArray &group) const;
    KConfigGroup group(const QByteArray &group);
    KConfigGroup group(const QString &group);
    KConfigGroup group(const char *group);
    void deleteGroup(const QByteArray &group, WriteConfigFlags flags = Normal);
    void deleteGroup(const QString &group, WriteConfigFlags flags = Normal);
    void deleteGroup(const char *group, WriteConfigFlags flags = Normal);
    virtual bool sync() = 0;
    virtual void markAsClean() = 0;
    enum AccessMode { NoAccess, ReadOnly, ReadWrite };
    virtual AccessMode accessMode() const = 0;
    virtual bool isImmutable() const = 0;
    bool isGroupImmutable(const QByteArray &aGroup) const;
    bool isGroupImmutable(const QString &aGroup) const;
    bool isGroupImmutable(const char *aGroup) const;
};
