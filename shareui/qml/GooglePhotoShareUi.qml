import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0

ShareDialog {
    id: root

    property bool isImage: {
        console.log(root.content)
        return false
    }

    onAccepted: {
        shareItem.start ()
    }

    Column {
        anchors.fill: parent

        spacing: Theme.paddingMedium

        DialogHeader {
            acceptText: qsTr("Upload")
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            visible: root.isLink
            text: root.content.linkTitle
            width: root.width - Theme.paddingLarge * 2
            elide: Text.ElideRight
            wrapMode:  Text.Wrap
            maximumLineCount: 2
        }
    }

    SailfishShare {
        id: shareItem

        source: root.source
        metadataStripped: true
        serviceId: root.methodId
        userData: {
            "accountId": root.accountId,
            "link": root.content.status,
            "tags": tagsArea.text
        }
        mimeType: root.content.type
    }
}
