#pragma once

#include <QString>
#include <QUrl>

#include <TransferEngine-qt5/mediaitem.h>
#include <TransferEngine-qt5/mediatransferinterface.h>

class GooglePhotoMediaTransfer : public MediaTransferInterface
{
    Q_OBJECT

public:
    GooglePhotoMediaTransfer(QObject * parent = 0);
    virtual ~GooglePhotoMediaTransfer();

    bool cancelEnabled() const;
    QString displayName() const;
    bool restartEnabled() const;
    QUrl serviceIcon() const;

public slots:
    void cancel();
    void start();

};
