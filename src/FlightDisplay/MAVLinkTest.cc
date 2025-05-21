#include "MAVLinkTest.h"
#include "MAVLinkProtocol.h"
#include "MultiVehicleManager.h"
#include "QGCLoggingCategory.h"
#include "QGCPalette.h"
#include "Vehicle.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>
#include <QDateTime>

MAVLinkTest::MAVLinkTest(QObject *parent)
    : QStringListModel(parent)
    , _palette(new QGCPalette(this))
{
    (void) connect(MultiVehicleManager::instance(), &MultiVehicleManager::activeVehicleChanged, this, &MAVLinkTest::_setActiveVehicle);
    _setActiveVehicle(MultiVehicleManager::instance()->activeVehicle());

    // 连接 MAVLinkProtocol 的消息接收信号  绑定至处理函数 _receiveMessage
    (void) connect(MAVLinkProtocol::instance(), &MAVLinkProtocol::messageReceived, this, &MAVLinkTest::_receiveMessage);
}

MAVLinkTest::~MAVLinkTest()
{
    if (_vehicle) {
        QByteArray msg;
        _sendSerialData(msg, true);
    }
}

// 接收 MAVLink 消息
void MAVLinkTest::_receiveMessage(LinkInterface*, mavlink_message_t message)
{

    if(message.msgid==MAVLINK_MSG_ID_MAVLINK_UORB)
    {
        qDebug() << "receive MAVLINK_MSG_ID_MAVLINK_UORB";
        mavlink_mavlink_uorb_t mavlink_uorb;

        mavlink_msg_mavlink_uorb_decode(&message, &mavlink_uorb);

        _accel_1 = QString::number(mavlink_uorb.accel0, 'f', 3);
        _accel_2 = QString::number(mavlink_uorb.accel1, 'f', 3);
        _accel_3 = QString::number(mavlink_uorb.accel2, 'f', 3);

        emit accel_Changed();

        qDebug() << _accel_1 << _accel_2 << _accel_3;
    }
}

void MAVLinkTest::_setActiveVehicle(Vehicle *vehicle)
{
    for (QMetaObject::Connection &con : _connections) {
        (void) disconnect(con);
    }
    _connections.clear();

    _vehicle = vehicle;
    if (_vehicle) {
        // Reset the model
        setStringList(QStringList());
        _connections << connect(_vehicle, &Vehicle::mavlinkSerialControl, this, &MAVLinkTest::_receiveData);
    }
}

void MAVLinkTest::_receiveData(uint8_t device, uint8_t flags, uint16_t timeout, uint32_t baudrate, const QByteArray &data)
{
    Q_UNUSED(flags); Q_UNUSED(timeout); Q_UNUSED(baudrate);

    if (device != SERIAL_CONTROL_DEV_SHELL) {
        return;
    }
}

void MAVLinkTest::_sendSerialData(const QByteArray &data, bool close)
{
    if (!_vehicle) {
        return;
    }

    SharedLinkInterfacePtr sharedLink = _vehicle->vehicleLinkManager()->primaryLink().lock();
    if (!sharedLink) {
        return;
    }

    // // Send maximum sized chunks until the complete buffer is transmitted
    // QByteArray output(data);
    // while (output.size()) {
    //     QByteArray chunk(data.left(MAVLINK_MSG_SERIAL_CONTROL_FIELD_DATA_LEN));
    //     const int dataSize = chunk.size();

    //     // Ensure the buffer is large enough, as the MAVLink parser expects MAVLINK_MSG_SERIAL_CONTROL_FIELD_DATA_LEN bytes
    //     (void) chunk.append(MAVLINK_MSG_SERIAL_CONTROL_FIELD_DATA_LEN - chunk.size(), '\0');

    //     const uint8_t flags = close ? 0 : SERIAL_CONTROL_FLAG_EXCLUSIVE |  SERIAL_CONTROL_FLAG_RESPOND | SERIAL_CONTROL_FLAG_MULTI;

    //     mavlink_message_t msg;
    //     (void) mavlink_msg_serial_control_pack_chan(
    //         MAVLinkProtocol::instance()->getSystemId(),
    //         MAVLinkProtocol::getComponentId(),
    //         sharedLink->mavlinkChannel(),
    //         &msg,
    //         SERIAL_CONTROL_DEV_SHELL,
    //         flags,
    //         0,
    //         0,
    //         dataSize,
    //         reinterpret_cast<uint8_t*>(chunk.data()),
    //         _vehicle->id(),
    //         _vehicle->defaultComponentId()
    //     );

    //     (void) _vehicle->sendMessageOnLinkThreadSafe(sharedLink.get(), msg);
    //     (void) output.remove(0, chunk.size());
    // }
}

// 发送 MAVLink 消息函数
void MAVLinkTest::_sendmsg(QString test1, QString test2, QString test3)
{
    if (!_vehicle)
    {
        qWarning() << "Internal error";
        return;
    }

    SharedLinkInterfacePtr sharedLink = _vehicle->vehicleLinkManager()->primaryLink().lock();
    if (!sharedLink) {
        return;
    }

    float send_test1=test1.toFloat();
    float send_test2=test2.toFloat();
    float send_test3=test3.toFloat();
    mavlink_message_t msg;
    (void) mavlink_msg_mavlink_uorb_pack_chan(MAVLinkProtocol::instance()->getSystemId(),
                                             MAVLinkProtocol::getComponentId(),
                                             sharedLink->mavlinkChannel(),
                                             &msg,
                                             0,
                                             send_test1,
                                             send_test2,
                                             send_test3);
    (void) _vehicle->sendMessageOnLinkThreadSafe(sharedLink.get(), msg);
    qDebug() << "[MAVLinkTest] send" << send_test1 << send_test2 << send_test3;
}





