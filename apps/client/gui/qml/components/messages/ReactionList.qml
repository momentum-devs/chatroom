import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings
import ".."

ListView {
    id: reactionButtonsListView
    model: [
        {
            "iconRow": [
                {"icon": "../../../../resources/svg/reactions/like.svg", "name": ":like:"},
                {"icon": "../../../../resources/svg/reactions/dislike.svg", "name": ":dislike:"},
                {"icon": "../../../../resources/svg/reactions/heart.svg", "name": ":heart:"},
                {"icon": "../../../../resources/svg/reactions/laugh.svg", "name": ":laugh:"},
                {"icon": "../../../../resources/svg/reactions/laugh-beam.svg", "name": ":laugh-beam:"}
            ]
        },
        {
            "iconRow": [
                {"icon": "../../../../resources/svg/reactions/laugh-squint.svg", "name": ":laugh-squint:"},
                {"icon": "../../../../resources/svg/reactions/sad.svg", "name": ":sad:"}
            ]
        }
    ]

    delegate: ReactionButtonsRow {
        width: reactionButtonsListView.width
        model: modelData.iconRow
        height: 50
    }
}