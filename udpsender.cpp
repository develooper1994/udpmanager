#include "udpsender.h"

// constructors
UdpSender::UdpSender(const QHostAddress& address, qint16 port, QObject *parent)
    : QObject(parent), m_address{address}, m_port{port}
{
    qDebug(UdpSenderCategory) << Q_FUNC_INFO << QThread::currentThread();
    setObjectName("UdpSender");
    m_datagram.setDestination(address, m_port);
}

UdpSender::UdpSender(qint16 port, QObject *parent)
    : UdpSender(QHostAddress(IP), port, parent)
{}

UdpSender::UdpSender(const QHostAddress& address, QObject *parent)
    : UdpSender(address, UDP_SEND_PORT, parent)
{}

UdpSender::UdpSender(QObject *parent)
    : UdpSender(QHostAddress(IP), UDP_SEND_PORT, parent)
{}

// SLOTS
void UdpSender::sent(const QByteArray &data){
    m_datagram.setData(data);
    if(!m_socket.writeDatagram(m_datagram)){
        qInfo(UdpSenderCategory) << m_socket.errorString();
    }
}



// GETTER-SETTER
QHostAddress UdpSender::getAddress() const{
    return m_address;
}
void UdpSender::setAddress(const QHostAddress &newAddress){
    m_address = newAddress;
}

qint16 UdpSender::getPort() const{
    return m_port;
}
void UdpSender::setPort(qint16 newPort){
    m_port = newPort;
}
