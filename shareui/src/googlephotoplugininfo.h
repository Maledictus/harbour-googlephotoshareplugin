#pragma once

#include <TransferEngine-qt5/transfermethodinfo.h>
#include <TransferEngine-qt5/transferplugininfo.h>

class GooglePhotoPluginInfo : public TransferPluginInfo
{
    Q_OBJECT

    bool m_Ready;
    QList<TransferMethodInfo> m_InfoList;

public:
    GooglePhotoPluginInfo();
    virtual ~GooglePhotoPluginInfo();

    QList<TransferMethodInfo> info() const;
    void query();
    bool ready() const;
};
