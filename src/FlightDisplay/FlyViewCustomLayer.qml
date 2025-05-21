/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QtLocation
import QtPositioning
import QtQuick.Window
import QtQml.Models

import QGroundControl
import QGroundControl.Controllers
import QGroundControl.Controls
import QGroundControl.FactSystem
import QGroundControl.FlightDisplay
import QGroundControl.FlightMap
import QGroundControl.Palette
import QGroundControl.ScreenTools
import QGroundControl.Vehicle

// To implement a custom overlay copy this code to your own control in your custom code source. Then override the
// FlyViewCustomLayer.qml resource with your own qml. See the custom example and documentation for details.
Item {
    id: _root

    // 模板内容
    property var parentToolInsets               // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets:   _toolInsets // These are the insets for your custom overlay additions
    property var mapControl

    // 用户自定义的属性
    property real   _layoutMargin:          ScreenTools.defaultFontPixelWidth * 0.75
    property real   _layoutSpacing:         ScreenTools.defaultFontPixelWidth

    // 模板内容
    // since this file is a placeholder for the custom layer in a standard build, we will just pass through the parent insets
    QGCToolInsets {
        id:                     _toolInsets
        leftEdgeTopInset:       parentToolInsets.leftEdgeTopInset
        leftEdgeCenterInset:    parentToolInsets.leftEdgeCenterInset
        leftEdgeBottomInset:    parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset:      parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset:   parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset:   parentToolInsets.rightEdgeBottomInset
        topEdgeLeftInset:       parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset:     parentToolInsets.topEdgeCenterInset
        topEdgeRightInset:      parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset:    parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset:  parentToolInsets.bottomEdgeCenterInset
        bottomEdgeRightInset:   parentToolInsets.bottomEdgeRightInset
    }

    /* -------------------- 增加自定义组件 ---------------------- */

    // property real CustomToolMargin: 20.0

    Item {
        id: _tool_1
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: _layoutMargin
        width: 230
        height: 300

        Timer {
            id: _timer
            interval: 1000
            running: false
            repeat: true
            onTriggered: mavlink_test._sendmsg(mavlink_send_1.text, mavlink_send_2.text, mavlink_send_3.text)
        }

        MAVLinkTest{
            id: mavlink_test
        }

        Rectangle {
            id: background
            // anchors.margins: _layoutMargin
            anchors.fill: parent
            color: "black"      // 设置背景颜色
            opacity: 0.5        // 设置透明度
            radius: 10.0        // 设置举行圆角半径
        }

        // 设置数据显示空间
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: _layoutMargin
            spacing: 20

            Text {
                id:mavlink_test_title
                // anchors.margins: _layoutMargin
                anchors.left: parent.left
                anchors.top: parent.top
                // height: 30
                // width: 100
                text: "MAVLink Test:"
                font.family: "微软雅黑"
                font.pointSize: 13
                color: "white"
            }

            RowLayout {
                spacing: 20
                ColumnLayout {
                    Text {
                        id: accel1_title
                        // anchors.margins: _layoutMargin
                        text: "Accel X:"
                        font.pointSize: 12
                        font.family: "微软雅黑"
                        color: "white"
                    }
                    TextEdit {
                        id: accel1_num
                        text: mavlink_test.accel_1
                        font.pointSize: 12
                        font.family: "微软雅黑"
                        color: "white"
                        readOnly: true          // 设置为只读  用于显示
                    }
                }

                ColumnLayout {
                    Text {
                        id: accel2_title
                        anchors.margins: _layoutMargin
                        text: "Accel Y:"
                        font.pointSize: 12
                        font.family: "微软雅黑"
                        color: "white"
                    }
                    TextEdit {
                        id: accel2_num
                        text: mavlink_test.accel_2
                        font.pointSize: 12
                        font.family: "微软雅黑"
                        color: "white"
                        readOnly: true
                    }
                }

                ColumnLayout {
                    Text {
                        id: accel3_title
                        anchors.margins: _layoutMargin
                        text: "Accel Z:"
                        font.pointSize: 12
                        font.family: "微软雅黑"
                        color: "white"
                    }
                    TextEdit {
                        id: accel3_num
                        text: mavlink_test.accel_3
                        font.pointSize: 12
                        font.family: "微软雅黑"
                        color: "white"
                        readOnly: true
                    }
                }

            }

            Text {
                id:mavlink_send_title
                // anchors.margins: _layoutMargin
                anchors.left: parent.left
                // anchors.top: parent.top
                // height: 30
                // width: 100
                text: "MAVLink Send:"
                font.family: "微软雅黑"
                font.pointSize: 13
                color: "white"
            }

            RowLayout {
                spacing: 20

                TextEdit {
                    id: mavlink_send_1
                    text: "0.0"
                    font.pointSize: 12
                    font.family: "微软雅黑"
                    color: "white"
                }

                TextEdit {
                    id: mavlink_send_2
                    text: "0.0"
                    font.pointSize: 12
                    font.family: "微软雅黑"
                    color: "white"
                }

                TextEdit {
                    id: mavlink_send_3
                    text: "0.0"
                    font.pointSize: 12
                    font.family: "微软雅黑"
                    color: "white"
                }
            }

            Button {
                id: mavlink_send_button
                anchors.left: parent.left
                anchors.right: parent.right
                text: "发送"
                font.family: "微软雅黑"
                font.pointSize: 12

                onClicked: {
                    _timer.running = !_timer.running
                }
            }
        }
    }

    
}
