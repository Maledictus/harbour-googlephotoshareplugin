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

    identity->queryInfo();

    m_authSession = identity->createSession(QStringLiteral("oauth2"));
    if (!m_authSession) {
        qWarning() << "Could not create auth session for identity!";
        return;
    }

    connect(m_authSession.data(), &SignOn::AuthSession::response,
            this, &GooglePhotoMediaTransfer::handleGotAuthSessionResponce);
    connect(m_authSession.data(), &SignOn::AuthSession::error, [](const SignOn::Error &err) {
        qWarning() << "SignOn error!: " << err.type() << err.message();
    });
    qWarning() << Q_FUNC_INFO;

    SignOn::SessionData sessionData;
    sessionData.setUserName(accountId);
    m_authSession->process(sessionData, QStringLiteral("web_server"));
}

void GooglePhotoMediaTransfer::handleGotAuthSessionResponce(const SignOn::SessionData& sessionData)
{
    qWarning() << Q_FUNC_INFO;
    qWarning() << sessionData.getAccessControlTokens()
               << sessionData.propertyNames()
               << sessionData.Realm()
               << sessionData.RenewToken()
               << sessionData.WindowId()
               << sessionData.Caption() << sessionData.Secret() << sessionData.UserName();
}
