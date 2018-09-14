import QtQuick 2.0
import QtQuick.VirtualKeyboard 2.1

Item {
    property var flickable
    readonly property var inputItem: InputContext.inputItem

    onInputItemChanged: delayedLoading.start()

    function ensureVisible(flickable) {
        if (!Qt.inputMethod.visible || !inputItem || !flickable || !flickable.visible/* || !flickable.interactive*/)
            return;

        var verticallyFlickable = (flickable.flickableDirection === Flickable.HorizontalAndVerticalFlick || flickable.flickableDirection === Flickable.VerticalFlick
                                   || (flickable.flickableDirection === Flickable.AutoFlickDirection && flickable.contentHeight > flickable.height))
        var horizontallyFlickable = (flickable.flickableDirection === Flickable.HorizontalAndVerticalFlick || flickable.flickableDirection === Flickable.HorizontalFlick
                                     || (flickable.flickableDirection === Flickable.AutoFlickDirection && flickable.contentWidth > flickable.width))

        if ((!verticallyFlickable && !horizontallyFlickable) || !inputItem.hasOwnProperty("cursorRectangle"))
            return

        var cursorRectangle = flickable.contentItem.mapFromItem(inputItem, inputItem.cursorRectangle.x, inputItem.cursorRectangle.y)

        if (verticallyFlickable) {
            var scrollMarginVertical = flickable.scrollMarginVertical ? flickable.scrollMarginVertical : 10
            if (flickable.contentY >= cursorRectangle.y - scrollMarginVertical) {
                // The flickable is foo far down; move it up.
                flickable.contentY = Math.max(0, cursorRectangle.y  - scrollMarginVertical)
            } else if (flickable.contentY + flickable.height <= cursorRectangle.y  + inputItem.cursorRectangle.height + scrollMarginVertical) {
                // The flickable is foo far up; move it down.
                flickable.contentY = Math.min(flickable.contentHeight - flickable.height, cursorRectangle.y + inputItem.cursorRectangle.height - flickable.height + scrollMarginVertical)
            }
        }
        if (horizontallyFlickable) {
            var scrollMarginHorizontal = flickable.scrollMarginHorizontal ? flickable.scrollMarginHorizontal : 10
            if (flickable.contentX >= cursorRectangle.x - scrollMarginHorizontal) {
                // The flickable is foo far down; move it up.
                flickable.contentX = Math.max(0, cursorRectangle.x - scrollMarginHorizontal)
            } else if (flickable.contentX + flickable.width <= cursorRectangle.x + inputItem.cursorRectangle.width + scrollMarginHorizontal) {
                // The flickable is foo far up; move it down.
                flickable.contentX = Math.min(flickable.contentWidth - flickable.width, cursorRectangle.x + inputItem.cursorRectangle.width - flickable.width + scrollMarginHorizontal)
            }
        }
    }
    Timer {
        id: delayedLoading
        interval: 10
        onTriggered: ensureVisible(flickable)
    }
    Connections {
        ignoreUnknownSignals: true
        target: Qt.inputMethod
        onKeyboardRectangleChanged: delayedLoading.start()
    }
    Connections {
        ignoreUnknownSignals: true
        target: inputItem
        enabled: inputItem && inputItem.activeFocus
        onCursorRectangleChanged: delayedLoading.start()
    }
}
