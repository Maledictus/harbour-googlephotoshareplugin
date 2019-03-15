#pragma once

#include <QString>
#include <QUrl>

#include <signon-qt5/SignOn/Identity>
#include <signon-qt5/SignOn/SessionData>
#include <TransferEngine-qt5/mediaitem.h>
#include <TransferEngine-qt5/mediatransferinterface.h>

class GooglePhotoMediaTransfer : public MediaTransferInterface
{
    Q_OBJECT

    SignOn::AuthSessionP m_authSession;
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
private slots:
    void handleGotIdentityInfo(const SignOn::IdentityInfo& info);
    void handleGotAuthSessionResponce(const SignOn::SessionData& sessionData);
};
