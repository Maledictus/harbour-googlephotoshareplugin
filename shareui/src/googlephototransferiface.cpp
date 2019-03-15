#include "googlephototransferiface.h"

#include <QtPlugin>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>

#include "googlephotoplugininfo.h"
#include "googlephotomediatransfer.h"

GooglePhotoSharePlugin::GooglePhotoSharePlugin()
{
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
    return true;
}

TransferPluginInfo* GooglePhotoSharePlugin::infoObject()
{
    return new GooglePhotoPluginInfo;
}

MediaTransferInterface* GooglePhotoSharePlugin::transferObject()
{
    return new GooglePhotoMediaTransfer;
}
