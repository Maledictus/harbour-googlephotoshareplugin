#include "googlephotomediatransfer.h"

#include <QtDebug>

#include <accounts-qt5/Accounts/Account>
#include <accounts-qt5/Accounts/Manager>
#include <libsailfishkeyprovider/sailfishkeyprovider.h>

GooglePhotoMediaTransfer::GooglePhotoMediaTransfer(QObject *parent)
: MediaTransferInterface(parent)
{
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
    return QUrl("/usr/share/harbour-googlephotoshareplugin/icons/picasa.png");
}

void GooglePhotoMediaTransfer::cancel()
{
    setStatus(MediaTransferInterface::TransferCanceled);
}

void GooglePhotoMediaTransfer::start()
{
    const auto& userData = mediaItem()->value (MediaItem::UserData).toMap();
    const auto& accountId = userData["accountId"].toInt();
    if (accountId < 0) {
        qWarning() << "Account id is invalid";
        cancel();
        return;
    }

    Accounts::Manager manager("sharing");
    Accounts::Account *acc = manager.account(accountId);
    if (!acc || acc->providerName() != "googlephoto") {
        qWarning() << "There is no account with this id";
        cancel();
        return;
    }

    Accounts::Service service = manager.service("googlephoto-sharing");
    acc->selectService(service);

    SignOn::Identity *identity = SignOn::Identity::existingIdentity(acc->credentialsId());
    if (!identity) {
        qWarning() << "Failed to find GooglePhoto account signon identity!";
        cancel();
        return;
    }

    SignOn::AuthSessionP authSession = identity->createSession(QStringLiteral("oauth2"));
    if (!authSession) {
        qWarning() << "Could not create auth session for identity!";
        return;
    }

    connect(authSession.data(), &SignOn::AuthSession::response,
            this, &GooglePhotoMediaTransfer::handleGotAuthSessionResponce);
    connect(authSession.data(), &SignOn::AuthSession::error, [this] (const SignOn::Error &err) {
        qWarning() << "SignOn error!: " << err.type() << err.message();
        cancel();
    });

    QVariantMap sessionContent {
        { "Host", acc->valueAsString("auth/oauth2/web_server/Host") },
        { "AuthPath", acc->valueAsString("auth/oauth2/web_server/AuthPath") },
        { "TokenPath", acc->valueAsString("auth/oauth2/web_server/TokenPath") },
        { "RedirectUri", acc->valueAsString("auth/oauth2/web_server/RedirectUri") }
    };

    char *clientIdBuf = nullptr;
    int ret = SailfishKeyProvider_storedKey(acc->providerName().toLocal8Bit(),
            acc->selectedService().name().toLocal8Bit(), "client_id", &clientIdBuf);
    if (ret) {
        cancel();
        return;
    }
    char *clientSecretBuf = nullptr;
    ret = SailfishKeyProvider_storedKey(acc->providerName().toLocal8Bit(),
            acc->selectedService().name().toLocal8Bit(), "client_secret", &clientSecretBuf);
    if (ret) {
        cancel();
        return;
    }
    sessionContent.insert("ClientId", QString(clientIdBuf));
    sessionContent.insert("ClientSecret", QString(clientSecretBuf));
    free(clientIdBuf);
    free(clientSecretBuf);

    authSession->process(SignOn::SessionData(sessionContent), acc->valueAsString("auth/mechanism"));
}

void GooglePhotoMediaTransfer::handleGotAuthSessionResponce(const SignOn::SessionData& sessionData)
{
    qWarning() << Q_FUNC_INFO << sessionData.getProperty("AccessToken");
}
