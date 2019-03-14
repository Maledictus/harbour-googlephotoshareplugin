#include "googlephototransferiface.h"

#include <QtPlugin>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>

#include "googlephotoplugininfo.h"
#include "googlephotomediatransfer.h"

GooglePhotoSharePlugin::GooglePhotoSharePlugin()
{
    qWarning() << Q_FUNC_INFO;
}

GooglePhotoSharePlugin::~GooglePhotoSharePlugin()
{
}

QString GooglePhotoSharePlugin::pluginId() const
{
    return QLatin1String("GooglePhotoSharePlugin");
}

bool GooglePhotoSharePlugin::enabled() const
{
    qWarning() << Q_FUNC_INFO;
    return true;
}

TransferPluginInfo* GooglePhotoSharePlugin::infoObject()
{
    qWarning() << Q_FUNC_INFO;
    return new GooglePhotoPluginInfo;
}

MediaTransferInterface* GooglePhotoSharePlugin::transferObject()
{
    qWarning() << Q_FUNC_INFO;
    return new GooglePhotoMediaTransfer;
}
