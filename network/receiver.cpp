#include "receiver.h"

#include <QUdpSocket>
#include <QTimer>
#include <QAudioOutput>

Receiver::Receiver(QWidget *pParent, QHostAddress haMulticastAddress, int nMulticastPort,
                   bool bConnectedToMulticast)
    : QDialog(pParent), m_haMulticastAddress(haMulticastAddress), m_nMulticastPort(nMulticastPort),
      m_bConnectToMulticast(bConnectedToMulticast)
{
    setupUi(this);
    setWindowTitle("Receiver " + QString::number(nMulticastPort));
    //
    labelAddress->setText(m_haMulticastAddress.toString());
    labelPort->setText(QString::number(nMulticastPort));
    //
    connect(buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    //
    m_pSocket = new QUdpSocket(this);
    //
    quint16 uReceiverPort = static_cast<quint16>(nMulticastPort);
    bool bound = m_pSocket->bind(m_haMulticastAddress, uReceiverPort, QUdpSocket::ShareAddress);
    if(!bound)
        m_pSocket->bind(QHostAddress::AnyIPv4, uReceiverPort, QUdpSocket::ReuseAddressHint);
    //
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(processSoundDatagrams()));
    //
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    //
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    //
    QAudioFormat preferFormat = info.preferredFormat();
    if(preferFormat.isValid())
    {
//        qDebug() << "Preffered audio format:\n"
//                 << "sample rate: " << preferFormat.sampleRate() << '\n'
//                 << "channels: " << preferFormat.channelCount() << '\n'
//                 << "sample size: " << preferFormat.sampleSize() << '\n'
//                 << "sample type: " << preferFormat.sampleType() << '\n'
//                 << "byte order: " << preferFormat.byteOrder() << '\n';
    }
    //
    if (!info.isFormatSupported(format)) {
        qDebug() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }
    qDebug() << info.deviceName();
    //
    m_pAudioOutput = new QAudioOutput(format, this);
    m_pOutputDevice = m_pAudioOutput->start();
    //
    connect(buttonPlay, SIGNAL(clicked(bool)), SLOT(playAudioOutput()));
}

void Receiver::joinMulticastGroup(const QHostAddress& haMulticastGroup)
{
    if(m_bConnectToMulticast)
    {
        if(m_pSocket->state() == QAbstractSocket::BoundState)
        {
            bool bJoined = m_pSocket->joinMulticastGroup(haMulticastGroup);
            //
            if(bJoined)
            {
                setWindowTitle(windowTitle() + ": joined Multicast Group");
                qDebug() << "Port " << m_nMulticastPort << " is joined Multicast Group "
                         << haMulticastGroup.toString();
            }
            else
            {
                setWindowTitle(windowTitle() + ": could not joined Multicast Group");
                qDebug() << "Port " << m_nMulticastPort << " could not joined Multicast Group "
                         << haMulticastGroup.toString();
            }
        }
        else
            qDebug() << "Receiver " + QString::number(m_nMulticastPort) + ": not in bound state";
    }
}

void Receiver::leaveMulticastGroup(const QHostAddress &haMulticastGroup)
{
    if(m_bConnectToMulticast)
    {
        if(m_pSocket->state() == QAbstractSocket::BoundState)
        {
            bool bLeft = m_pSocket->leaveMulticastGroup(haMulticastGroup);
            //
            if(bLeft)
            {
                setWindowTitle(windowTitle() + ": left Multicast Group");
                qDebug() << "Port " << m_nMulticastPort << " is left Multicast Group "
                         << haMulticastGroup.toString();
            }
            else
            {
                setWindowTitle(windowTitle() + ": could not leave to Multicast Group");
                qDebug() << "Port " << m_nMulticastPort << " could not leave Multicast Group "
                         << haMulticastGroup.toString();
            }
        }
        else
            qDebug() << "Receiver " + QString::number(m_nMulticastPort) + ": not in bound state";
    }
}

void Receiver::playAudioOutput()
{
    QByteArray baSoundSignal;

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
            baSoundSignal.append(value);
        }
    }
    m_pOutputDevice->write(baSoundSignal);
}

void Receiver::processSoundDatagrams()
{
    QByteArray baDatagram;
    while(m_pSocket->hasPendingDatagrams())
    {
        baDatagram.resize(m_pSocket->pendingDatagramSize());
        m_pSocket->readDatagram(baDatagram.data(), baDatagram.size());
    }
    //
    QDataStream in(&baDatagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);
    //
    quint32 baSize;
    in >> baSize;
    QByteArray baSoundSignal;
    baSoundSignal.resize(baSize);
    in >> baSoundSignal;
    textEditMessage->append("Sent: " + QString::number(baSoundSignal.size() + sizeof(baSize))
                            + " bytes");
    m_pOutputDevice->write(baSoundSignal);
}

void Receiver::on_buttonJoinGroup_clicked()
{
    joinMulticastGroup(m_haMulticastAddress);
}

void Receiver::on_buttonLeaveGroup_clicked()
{
    leaveMulticastGroup(m_haMulticastAddress);
}
