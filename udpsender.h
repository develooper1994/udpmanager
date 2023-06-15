/*
 * udpclient Listen port 8000 and gets response from appBilet for hatList. Runs async
 * udpserver(port 8001) sends selection to appBilet for hatList. Runs sync
*/
#ifndef UDPSENDER_H
#define UDPSENDER_H

#include "udpdef.h"
inline static QLoggingCategory UdpSenderCategory("UdpSender");

class UdpSender : public QObject
{
    Q_OBJECT
public:
    // constructors
    explicit UdpSender(const QHostAddress& address=QHostAddress(IP), qint16 port=UDP_SEND_PORT, QObject *parent = nullptr);
    explicit UdpSender(qint16 port=UDP_SEND_PORT, QObject *parent = nullptr);
    explicit UdpSender(const QHostAddress& address=QHostAddress(IP),QObject *parent = nullptr);
    explicit UdpSender(QObject *parent = nullptr);

    qint16 getPort() const;
    void setPort(qint16 newPort);

    QHostAddress getAddress() const;
    void setAddress(const QHostAddress &newAddress);

signals:
public slots:
    void sent(const QByteArray& data);
private:
    QUdpSocket m_socket;
    QHostAddress m_address;
    qint16 m_port;
    QNetworkDatagram m_datagram;
};

#endif // UDPSERVER_H
