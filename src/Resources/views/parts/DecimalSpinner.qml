import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2


SpinBox {
    id: spinner

    property int decimals: 2
    property real factor: Math.pow(10, decimals)
    property real realValue: value / factor
    property int stepSizeSmall: 1
    property int stepSizeLarge: 100
    property int stepSizeSwitchMB: 5
    property int stepSizeSwitchUB: stepSizeSwitchMB * factor

    from: 1
    to: 900 * factor
    value: 0
    stepSize: (spinner.value <= stepSizeSwitchUB) ? stepSizeSmall : stepSizeLarge;

    validator: DoubleValidator {
        bottom: Math.min(spinner.from, spinner.to)
        top:  Math.max(spinner.from, spinner.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / factor).toLocaleString(locale, 'f', spinner.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * factor
    }

}
