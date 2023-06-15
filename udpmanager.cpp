#include "udpmanager.h"

#include <QMetaType>

QLoggingCategory UdpManager::UdpManagerCategory("UdpManager");

UdpManager::UdpManager(QObject *parent) : QObject{parent} {
  init();
  connections();
}

void UdpManager::init() {
  // receiver başlamadan istek göndermenin manası yok.
  setObjectName("UdpManager");
  m_address.setAddress(IP);
  // QHostAddress(UDP_IP)
  // QHostAddress::SpecialAddress::AnyIPv4
  // QHostAddress("10.1.1.0")
  // QHostAddress("10.1.1.1")
  // QHostAddress(VALIDATOR_IP)
  // QHostAddress("10.1.1.55")
  // QHostAddress("10.1.1.255")
  m_sender = new UdpSender(m_address, UDP_SEND_PORT, this);

  m_receiver = new UdpReceiver(UDP_RECEIVE_PORT);

  m_thread = new QThread(this);
  m_thread->setObjectName("ReceiverThread");
  m_receiver->moveToThread(m_thread);
}
void UdpManager::connections() {
  QObject::connect(m_thread, &QThread::started, m_receiver, &UdpReceiver::run,
                   Qt::ConnectionType::QueuedConnection);

  qRegisterMetaType<std::optional<Utilities::HatStructList>>(
      "OptionalHatStructList");
  QObject::connect(
      m_receiver,
      QOverload<std::optional<Utilities::HatStructList>>::of(
          &UdpReceiver::dataReceived),
      [this](const std::optional<Utilities::HatStructList> &hatStructList) {
        emit receive(hatStructList);
      });

  startAll();
}
void UdpManager::startAll() {
  // start things(thread, timer, ...) after (signal-slot)connect
  m_thread->start();
}

// slots
void UdpManager::sent(const QByteArray &message) {
  m_sender->sent(std::forward<decltype(message)>(message));
}
