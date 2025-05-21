#pragma once

#include "MAVLinkProtocol.h"
#include "QGCPalette.h"

#include <QtCore/QLoggingCategory>
#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringListModel>
#include <QtQmlIntegration/QtQmlIntegration>
#include <QtQml/qqmlregistration.h>

class QGCPalette;
class Vehicle;

class MAVLinkTest : public QStringListModel
{
    Q_OBJECT
    // QML_ELEMENT
    Q_MOC_INCLUDE("Vehicle.h")

    // 设定属性变量
    Q_PROPERTY(QString accel_1 READ accel_1 NOTIFY accel_Changed)
    QString accel_1() const { return _accel_1; }

    Q_PROPERTY(QString accel_2 READ accel_2 NOTIFY accel_Changed)
    QString accel_2() const { return _accel_2; }

    Q_PROPERTY(QString accel_3 READ accel_3 NOTIFY accel_Changed)
    QString accel_3() const { return _accel_3; }

    public:
        explicit MAVLinkTest(QObject *parent = nullptr);
        ~MAVLinkTest();

        Q_INVOKABLE void _sendmsg(QString test1, QString test2, QString test3);

    private slots:
        void _setActiveVehicle(Vehicle *vehicle);
        void _receiveMessage (LinkInterface* link, mavlink_message_t message);
        void _receiveData(uint8_t device, uint8_t flags, uint16_t timeout, uint32_t baudrate, const QByteArray &data);

    // 定义信号
    signals:
        void accel_Changed();

    private:
        void _sendSerialData(const QByteArray &data, bool close = false);

        QString _accel_1 = "--.--", _accel_2 = "--.--", _accel_3 = "--.--";
        QGCPalette *_palette = nullptr;
        QList<QMetaObject::Connection> _connections;
        Vehicle *_vehicle = nullptr;

};





