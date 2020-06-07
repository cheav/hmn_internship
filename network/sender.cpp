#include "sender.h"

#include <QUdpSocket>
#include <QTimer>

Sender::Sender(QWidget *pParent, QHostAddress haMulticastAddress, int nMulticastPort)
    : QDialog(pParent), m_haMulticastAddress(haMulticastAddress), m_nMulticastPort(nMulticastPort)
{
    setupUi(this);
    //
    connect(buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    //
    lineEditAddress->setEnabled(false);
    spinBoxPort->setEnabled(false);
    //
    m_pSocket = new QUdpSocket(this);
    m_pSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    //
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(500);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(sendDatagram()));
    //
    // timer stops when this closed (disconnected from host):
    connect(m_pSocket, SIGNAL(disconnected()), m_pTimer, SLOT(stop()));
}
Sender::SoundSignal::SoundSignal()
{
    const double pi = 3.14;
    const double T = 2.0 * pi;
    const char A = 127;
    const double dt = T / (2.0 * A);
    const double eps = -1.0e-10;

    for(int w = 1; w < 100; ++w)
    {
        double t = 0.0;
        for(int i = 0; t - T < eps; ++i)
        {
            t = i * dt;
            char value = A * sin(w * t);
            m_baSoundSignal.append(value);
        }
    }
}

void Sender::closeEvent(QCloseEvent*)
{
    m_pSocket->disconnectFromHost();
}
//-------------------------------------------------------------------------------------------------
void Sender::on_buttonSend_clicked()
{
    m_pTimer->start();
}
void Sender::on_buttonStop_clicked()
{
    m_pTimer->stop();
}
void Sender::sendDatagram()
{
    QByteArray baDatagram;
    QDataStream out(&baDatagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    //
    SoundSignal sound;
    out << static_cast<quint32>(sound.m_baSoundSignal.size());
    out << sound.m_baSoundSignal;
    //
    qint64 nBytesSent = m_pSocket->writeDatagram(baDatagram, m_haMulticastAddress,
                                                 static_cast<quint16>(m_nMulticastPort));
    //
    textEditMessage->append("Sent: " + QString::number(nBytesSent) + " bytes");
}
//-------------------------------------------------------------------------------------------------
