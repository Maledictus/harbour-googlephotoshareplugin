#include "googlephotomediatransfer.h"

#include <QtDebug>

#include <accounts-qt5/Accounts/Account>
#include <accounts-qt5/Accounts/Manager>

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

    SignOn::Identity *identity = SignOn::Identity::existingIdentity(acc->credentialsId());
    if (!identity) {
        qWarning() << "Failed to find GooglePhoto account signon identity!";
        cancel();
        return;
    }

    connect(identity, &SignOn::Identity::info,
            this, &GooglePhotoMediaTransfer::handleGotIdentityInfo);
    identity->queryInfo();

    SignOn::AuthSessionP authSession = identity->createSession(QStringLiteral("oauth2"));
    if (!authSession) {
        qWarning() << "Could not create auth session for identity!";
        return;
    }

    connect(authSession.data(), &SignOn::AuthSession::response,
            this, &GooglePhotoMediaTransfer::handleGotAuthSessionResponce);
    connect(authSession.data(), &SignOn::AuthSession::error, [] (const SignOn::Error &err) {
        qWarning() << "SignOn error!: " << err.type() << err.message();
    });

    SignOn::SessionData sessionData({
            { "Host", "accounts.google.com" },
            { "AuthPath", "o/oauth2/auth?access_type=offline" },
            { "TokenPath","o/oauth2/token" },
            { "RedirectUri", "urn:ietf:wg:oauth:2.0:oob"},
            { "ResponseType", "code"},
            { "ClientId", "844868161425-usj7ht7n97q02tq0n9ulvaf3rolhakjb.apps.googleusercontent.com"},
            { "ClientSecret", "5aJTCCkCOLSc4uFfyv79wgpL" }
                });
    authSession->process(sessionData, QStringLiteral("web_server"));
}

void GooglePhotoMediaTransfer::handleGotIdentityInfo(const SignOn::IdentityInfo& info)
{
    qWarning() << Q_FUNC_INFO;
    qWarning() << info.isStoringSecret()
               << info.secret()
               << info.type()
               << info.methods()
               << info.accessControlList()
               << info.owner()
               << info.caption()
               << info.userName();
}

void GooglePhotoMediaTransfer::handleGotAuthSessionResponce(const SignOn::SessionData& sessionData)
{
    qWarning() << Q_FUNC_INFO;
    qWarning() << sessionData.getAccessControlTokens()
               << sessionData.getProperty("AccessToken")
               << sessionData.Realm()
               << sessionData.RenewToken()
               << sessionData.WindowId()
               << sessionData.Caption() << sessionData.Secret() << sessionData.UserName();
}
