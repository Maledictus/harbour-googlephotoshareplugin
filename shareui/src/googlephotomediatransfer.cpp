#include "googlephotomediatransfer.h"

#include <QtDebug>

GooglePhotoMediaTransfer::GooglePhotoMediaTransfer(QObject *parent)
: MediaTransferInterface(parent)
{
    qWarning() << Q_FUNC_INFO;
}

GooglePhotoMediaTransfer::~GooglePhotoMediaTransfer()
{
}

bool GooglePhotoMediaTransfer::cancelEnabled() const
{
    return false;
}

QString GooglePhotoMediaTransfer::displayName() const
{
    return QString("GooglePhoto");
}

bool GooglePhotoMediaTransfer::restartEnabled() const
{
    return false;
}

QUrl GooglePhotoMediaTransfer::serviceIcon() const
{
    return QUrl();
}

void GooglePhotoMediaTransfer::cancel()
{
    setStatus(MediaTransferInterface::TransferCanceled);
}

void GooglePhotoMediaTransfer::start()
{
    qWarning() << Q_FUNC_INFO;
}
