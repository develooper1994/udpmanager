// 113302+1+"SULTAN SELİM MAH"+990+483+709#7011+1+"HT11"+990+483+709
#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "udpdef.h"

#include <QNetworkDatagram>

inline static QLoggingCategory UdpReceiverCategory("UdpReceiver");

class UdpReceiver : public QObject {
  Q_OBJECT
public:
  UdpReceiver(qint16 port = UDP_RECEIVE_PORT, QObject *parent = nullptr);

  std::optional<Utilities::HatStructList> parse(const QByteArray &data);

  // getter-setter
  qint16 getPort() const;
  void setPort(qint16 newPort);

signals:
  void dataReceived(const QByteArray &data);
  void dataReceived(QStringList hatList);
  void dataReceived(Utilities::HatStructList hatList);
  void dataReceived(std::optional<Utilities::HatStructList> hatList);
  // void dataReceived(const std::optional<Utilities::HatStructList>& hatList);
  // void dataReceived(std::optional<Utilities::HatStructList>&& hatList);

public slots:
  void run(); // thread start
  void start(QAbstractSocket::BindFlag flag =
                 QAbstractSocket::BindFlag::DefaultForPlatform);
  void stop();
  void readyRead();
  void onSocketStateChange(QAbstractSocket::SocketState state);
  void onErrorOccured();
  void QtReceive();

private:
  QEventLoop *m_loop;
  QUdpSocket *m_socket;
  qint16 m_port;
  QNetworkDatagram m_datagram;
  Utilities::HatBilgi hatbilgi;
  QByteArray m_accumulatedData; // Accumulated data
};

#endif
