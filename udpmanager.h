#ifndef UDPMANAGER_H
#define UDPMANAGER_H

#include "udpdef.h"

#include "udpsender.h"
#include "udpreceiver.h"
#include "../utilities/hatbilgi/hatbilgi.h"

template<typename TYPE>
struct TransactionData {
    QString sender;
    TYPE data;
    // Add more fields as needed
};

class UdpManager : public QObject
{
    Q_OBJECT
public:
    explicit UdpManager(QObject *parent = nullptr);
    static QLoggingCategory UdpManagerCategory;

public slots:
    void sent(const QByteArray& message);
    //QByteArray received();

signals:
    void receive();
    void receive(Utilities::HatStructList hatStructList);
    void receive(std::optional<Utilities::HatStructList> hatStructList);
    //void receive(const std::optional<Utilities::HatStructList>& hatStructList);
    //void receive(std::optional<Utilities::HatStructList>&& hatStructList);

private slots:

private:
    QHostAddress m_address;
    UdpSender* m_sender;
    UdpReceiver* m_receiver;
    QThread* m_thread;
    MesajDurumu m_msgDurum;

    void init();
    void connections();
    void startAll();

};

#endif // UDPMANAGER_H
