#include "googlephotoplugininfo.h"

#include <QFile>
#include <QSettings>

#include <Accounts/Manager>
#include <Accounts/Service>

GooglePhotoPluginInfo::GooglePhotoPluginInfo()
: m_Ready(false)
{
}

GooglePhotoPluginInfo::~GooglePhotoPluginInfo()
{
}

QList<TransferMethodInfo> GooglePhotoPluginInfo::info() const
{
    return m_InfoList;
}

void GooglePhotoPluginInfo::query()
{
    qWarning() << Q_FUNC_INFO;
    Accounts::Manager manager("sharing");

    // Get the list of enabled AccountService objects of type e-mail.
    const auto& accountsIds = manager.accountListEnabled();
    for (const auto& id : accountsIds) {
        Accounts::Account *acc = manager.account(id);
        if (!acc)
        {
            qWarning() << Q_FUNC_INFO << "invalid account";
        }

        if (acc->providerName() != "googlephoto") {
            continue;
        }

        TransferMethodInfo info;
        info.displayName = QLatin1String("GooglePhoto");
        info.userName = acc->displayName();
        info.accountId = id;
        info.methodId = QLatin1String("GooglePhotoSharePlugin");
        info.shareUIPath = QLatin1String("/usr/share/harbour-googlephotoshareplugin/qml/GooglePhotoShareUi.qml");
        info.capabilitities = QStringList({ QLatin1String("image/*") });
        m_InfoList << info;
    }

    m_Ready = true;
    emit infoReady();
}

bool GooglePhotoPluginInfo::ready() const
{
    return m_Ready;
}
