/*
    * DataLong(coming from udp):
    HATLIST#158&431&34&AVCILAR-Z\xC4\xB0NC\xC4\xB0RL\xC4\xB0KUYU&8&1469+483+806&1#158&431&34&AVCILAR-Z\xC4\xB0NC\xC4\xB0RL\xC4\xB0KUYU&8&1469+483+806&2#158&556&34A&CEV\xC4\xB0ZL\xC4\xB0""BA\xC4\x9E
   -
   S\xC3\x96\xC4\x9E\xC3\x9CTL\xC3\x9C\xC3\x87""E\xC5\x9EME&8&1469+483+806&1#158&556&34A&CEV\xC4\xB0ZL\xC4\xB0""BA\xC4\x9E
   -
   S\xC3\x96\xC4\x9E\xC3\x9CTL\xC3\x9C\xC3\x87""E\xC5\x9EME&8&1469+483+806&2#158&5174&34B&BEYL\xC4\xB0KD\xC3\x9CZ\xC3\x9C
   - AVCILAR&8&1469+483+806&1#158&5174&34B&BEYL\xC4\xB0KD\xC3\x9CZ\xC3\x9C -
   AVCILAR&8&1469+483+806&2#158&5175&34C&BEYL\xC4\xB0KD\xC3\x9CZ\xC3\x9C -
   CEV\xC4\xB0ZL\xC4\xB0""BA\xC4\x9E&8&1469+483+806&1#158&5175&34C&BEYL\xC4\xB0KD\xC3\x9CZ\xC3\x9C
   - CEV\xC4\xB0ZL\xC4\xB0""BA\xC4\x9E&8&1469+483+806&2#158&570&34T&AVCILAR
   -TOPKAPI METROB\xC3\x9CS\xC3\x9C&8&1469+483+806&1#158&570&34T&AVCILAR
   -TOPKAPI METROB\xC3\x9CS\xC3\x9C&8&1469+483+806&2#OK
    * Data Short(copy paste console):
    HATLIST#158&431&34&AVCILAR-ZÄ°NCÄ°RLÄ°KUYU&8&385+125+210&1#158&431&34&AVCILAR-ZÄ°NCÄ°RLÄ°KUYU&8&385+125+210&2#158&556&34A&CEVÄ°ZLÄ°BAÄž
   - SÃ–ÄžÃœTLÃœÃ‡EÅžME&8&385+125+210&1#158&556&34A&CEVÄ°ZLÄ°BAÄž -
   SÃ–ÄžÃœTLÃœÃ‡EÅžME&8&385+125+210&2#158&5174&34B&BEYLÄ°KDÃœZÃœ -
   AVCILAR&8&385+125+210&1#158&5174&34B&BEYLÄ°KDÃœZÃœ -
   AVCILAR&8&385+125+210&2#158&5175&34C&BEYLÄ°KDÃœZÃœ -
   CEVÄ°ZLÄ°BAÄž&8&385+125+210&1#158&5175&34C&BEYLÄ°KDÃœZÃœ -
   CEVÄ°ZLÄ°BAÄž&8&385+125+210&2#158&570&34T&AVCILAR -TOPKAPI
   METROBÃœSÃœ&8&385+125+210&1#OK

    *
*/

/*
// docklight not support longer than 1024 byte in my setup :'(
// Python3 Test Code - ChatGpt
import socket
import random
import string

def generate_data(size):
    return ''.join(random.choices(string.ascii_uppercase +
string.ascii_lowercase + string.digits, k=size))

def send_data(begin, end):
    host = 'localhost'  # Replace with the IP address or hostname of the
receiving machine port = 9008  # Replace with the desired port number

    data = f"{begin}#{generate_data(60000)}{end}"

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        sock.sendto(data.encode(), (host, port))
        print("Data sent successfully!")
    except socket.error as e:
        print(f"Error while sending data: {str(e)}")
    finally:
        sock.close()

# Call the function with begin and end values
send_data("HATLIST#158", "OK")

*/

#include "udpreceiver.h"
#include <QMetaEnum>

#define OK "OK"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
#define CODING "UTF-8"
#define SETENCODE(stream) stream.setCodec(CODING);
#else
#include <QStringConverter>
#define CODING QStringConverter::Encoding::Utf8
#define SETENCODE(stream) stream.setEncoding(CODING);
#endif

QByteArray encodeString(const QString &str) { return str.toUtf8(); }
QString decodeString(const QByteArray &data) { return QString::fromUtf8(data); }

QByteArray fixEncoding(const QByteArray &data) {
  /*
  Örnek: Z\xC4\xB0NC\xC4\xB0RL\xC4\xB0KUYU
  =>
  ['Z','\','x','C','4','\','x','B','0','N','C','\','x','C','4','\','x','B','0','R','L','\','x','C','4','\','x','B','0','K','U','Y','U']
  Sorun: Gelen veri "char*" şeklinde geldiğinden her bir karakteri bir char
  olarak görünüyor. Burada Utf8 literal char dizisi olarak gelen veriler Utf8
  karakter karşılığına decode edilmeli.
          ['\','x','C','4','\','x','B','0'] => 'İ'
  Çözüm:
      2. çözüm önerisi çalıştı.
      TODO: hızlandırabilecek bir çözüm önerisi bul ve not et.

      * 1. çözüm önerisi: QTextStream ver kodlamayı yapsın. fakat '\' bile ayrı
  bir char.
      * İşe yaramaz.
      QString str;
      QTextStream stream(&str);
      SETENCODE(stream);
      stream << data;
      stream.flush();
      qDebug() << "stream: " << stream.readAll();
      return str;

      * 2. çözüm önerisi: tüm karakterleri ayır. hexleri al ve QChar oluşan
  QByteArrayler üret.
      * Bunları yeni bir QByteArray içie koy ve decode edilmiş halini al.
      * Yavaş fakat işi çözüyor.
      QByteArray hex1 = QByteArray::fromHex("C4");
      QByteArray hex2 = QByteArray::fromHex("B0");

      QByteArray combinedByteArray;
      combinedByteArray.append(hex1);
      combinedByteArray.append(hex2);

      QString decodedString = QString::fromUtf8(combinedByteArray);

      // Output
      qDebug() << "Decoded string: " << decodedString.toUtf8().constData()
  */

  QByteArray incomingData(data);
  QByteArray encodingFixed;
  QByteArray hexValue;
  QByteArray accumulatorStr;

  for (int i = 0; i < incomingData.length(); i++) {
    auto c = incomingData[i];
    if (c == '\\') {
      // Skip the backslash character
      continue;
    } else if (c == 'x') {
      // Extract the following two characters as a hexadecimal value
      hexValue = incomingData.mid(i + 1, 2);
      bool ok;
      (void)hexValue.toInt(&ok, 16);
      if (ok) {
        hexValue = QByteArray::fromHex(hexValue);
        accumulatorStr.append(hexValue);
        // qDebug() << "accumulator: "<< accumulatorStr;
      }
      i += 2; // Skip the two hexadecimal digits
    } else {
      // Calculate the sum of hexadecimal values encountered so far and convert
      // it to QChar
      if (accumulatorStr.length() > 0) {
        QString decodedString = QString::fromUtf8(accumulatorStr);
        encodingFixed.append(decodedString.toUtf8().constData());
        accumulatorStr.clear();
      }
      encodingFixed.append(incomingData[i]);
    }
  }

  // Calculate the final sum and convert it to QChar if there's any remaining
  // sum value
  if (accumulatorStr.length() > 0) {
    QString decodedString = QString::fromUtf8(accumulatorStr);
    encodingFixed.append(decodedString.toUtf8().constData());
    accumulatorStr.clear();
  }

  // str now contains the QChar instances after summing and converting the
  // hexadecimal digits
  return encodingFixed;
}

UdpReceiver::UdpReceiver(qint16 port, QObject *parent)
    : QObject(parent), m_port(port) {
  qDebug() << Q_FUNC_INFO << QThread::currentThread();
  setObjectName("UdpReceiver");

  qInfo(UdpReceiverCategory)
      << "gönderen:" << m_datagram.senderAddress() << m_datagram.senderPort();
  qInfo(UdpReceiverCategory) << "hedef:" << m_datagram.destinationAddress()
                             << m_datagram.destinationPort();
}

// slots
void UdpReceiver::run() {
  /*
  Bu fonksiyon herhangi threadi ve eventloop çalıştırıp başlatır.
  class için ana giriş noktasıdır.
  UDP üzerinden dinlemek için bu fonksiyonu çalıştırın.
  */
  qDebug(UdpReceiverCategory) << Q_FUNC_INFO << QThread::currentThread();

  m_loop = new QEventLoop(this);
  m_socket = new QUdpSocket(this);
  connect(m_socket, &QUdpSocket::readyRead, this, &UdpReceiver::readyRead);
  connect(m_socket, &QAbstractSocket::errorOccurred, this,
          &UdpReceiver::onErrorOccured);
  connect(m_socket, &QAbstractSocket::stateChanged, this,
          &UdpReceiver::onSocketStateChange);

  start(QAbstractSocket::BindFlag::
            DefaultForPlatform); // QAbstractSocket::BindFlag::ShareAddress

  // set udp buffer size
  // auto defaultBufferSize =
  // m_socket->socketOption(QAbstractSocket::SocketOption::ReceiveBufferSizeSocketOption);
  // qInfo(UdpReceiverCategory) << "Before - OS level default buffer size: " <<
  // defaultBufferSize;
  qInfo(UdpReceiverCategory) << "Before - Socket level read buffer size: "
                             << m_socket->readBufferSize();

  int bufferSize = 0; // Example buffer size
  // Os level buffer size set
  m_socket->setSocketOption(
      QAbstractSocket::SocketOption::ReceiveBufferSizeSocketOption, bufferSize);
  qInfo(UdpReceiverCategory)
      << "Before - OS level default buffer size: "
      << m_socket->socketOption(
             QAbstractSocket::SocketOption::ReceiveBufferSizeSocketOption);

  m_socket->setReadBufferSize(
      bufferSize); // only one socket receive buffer size set
  qInfo(UdpReceiverCategory) << "After - Socket level read buffer size: "
                             << m_socket->readBufferSize();

  m_loop->exec();
}

void UdpReceiver::start(QAbstractSocket::BindFlag flag) {
  /*
  Udp dinlemeye başlar threadi kilitler!
  */

  qDebug(UdpReceiverCategory) << Q_FUNC_INFO << QThread::currentThread();

  // Rx connection: check we are not already bound
  if (m_socket->state() != m_socket->BoundState) {
    if (!m_socket->bind(QHostAddress::Any, m_port,
                        flag)) { // QAbstractSocket::BindFlag::ShareAddress
      qInfo(UdpReceiverCategory) << m_socket->errorString();
      return;
    }
  }

  qInfo(UdpReceiverCategory)
      << "UdpReceiver başladı: " << m_socket->localAddress() << ":"
      << m_socket->localPort();
}
void UdpReceiver::stop() {
  /*
  Udp soketini kapatır ve eventloop çalışıyorsa durdurup siler.
  Ayrıca sınıfın kendisini silmesi için komut verir.
  */

  m_socket->close();

  if (m_loop) {
    qInfo(UdpReceiverCategory) << "Event loop durdu";
    m_loop->quit();
    delete m_loop;
  }

  qInfo(UdpReceiverCategory)
      << Q_FUNC_INFO << "Stopped" << QThread::currentThread();
  deleteLater();
}

// TODO: OK ayrı geliyor!
void UdpReceiver::readyRead() {
  /*
   * Bu method bir UDP akümülatördür.
   * "OK" veya "NOK" geldiğinde m_accumulator içeriği silinir.
   * gelmezse biriktirmeye devam eder.
   */

  qDebug(UdpReceiverCategory) << Q_FUNC_INFO << QThread::currentThread();

  while (m_socket->hasPendingDatagrams()) {
    /*
    m_datagram = m_socket->receiveDatagram();
    // Accumulate the received data
    m_accumulatedData.append(m_datagram.data());
    */

    QByteArray datagram;
    datagram.resize(m_socket->pendingDatagramSize());
    m_socket->readDatagram(datagram.data(), datagram.size());
    // Accumulate the received data
    m_accumulatedData.append(datagram);

    static int countOK = 0;
    qsizetype endIndex;

    if (!m_accumulatedData.isEmpty()) {
      // Process the accumulated data
      QByteArray encodingFixed = fixEncoding(m_accumulatedData);
      qDebug(UdpReceiverCategory)
          << "accumulatedData Received: " << encodingFixed;

      // Check if "OK" is present in the accumulated data
      if (encodingFixed.right(2).contains(OK)) {
        // Process the accumulated data until "OK"
        endIndex =
            encodingFixed.indexOf(OK) + sizeof(OK); // Include "OK" in the data
        // fixEncoding(processData);
        // emit dataReceived(processData);
        std::optional<Utilities::HatStructList> hsl = parse(encodingFixed);
        if (hsl == std::nullopt) {
          m_accumulatedData.clear();
          return;
        }
        emit dataReceived(hsl);
        ++countOK;

        // Remove the processed data from the accumulated data
        // m_accumulatedData.remove(0, endIndex);
      }
    }
  }
}

void UdpReceiver::onSocketStateChange(QAbstractSocket::SocketState state) {
  // connect for each state:
  // https://doc.qt.io/qt-5/qabstractsocket.html#SocketState-enum
  QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
  qInfo(UdpReceiverCategory) << "State:" << metaEnum.valueToKey(state);
  if (state == QAbstractSocket::SocketState::BoundState) {
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpReceiver::QtReceive);
  }
}

void UdpReceiver::onErrorOccured() {
  qCritical(UdpReceiverCategory) << "socket error: " << this->m_socket->error();
  qCritical(UdpReceiverCategory)
      << "socket errorString: " << this->m_socket->errorString();
}

void UdpReceiver::QtReceive() { qInfo(UdpReceiverCategory) << "Pending Data"; }

std::optional<Utilities::HatStructList>
UdpReceiver::parse(const QByteArray &data) {
  // HatListWidget doldurmak için gerekli!
  qDebug(UdpReceiverCategory) << Q_FUNC_INFO << QThread::currentThread();

  if (data.isEmpty()) {
    //|| !(m_datagram.data().trimmed().contains(ARACVALIDATOR) &&
    // m_datagram.data().trimmed().contains(HATTANIMLA_CONST))
    qInfo(UdpReceiverCategory) << "datagram boş";
    return std::nullopt;
  }

  std::optional<Utilities::HatStructList> hatStructList =
      hatbilgi.parseLines(data);

  return hatStructList;
}

// getter-setter
qint16 UdpReceiver::getPort() const { return m_port; }
void UdpReceiver::setPort(qint16 newPort) { m_port = newPort; }
