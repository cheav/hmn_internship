#ifndef RECEIVER_H
#define RECEIVER_H

#include <QDialog>
#include <QHostAddress>

#include <cmath>

#include "ui_receiver_dialog.h"

class QUdpSocket;
class QAudioOutput;

class Receiver : public QDialog, public Ui::ReceiverDialog
{
    Q_OBJECT
public:
    Receiver(QWidget *pParent = nullptr, QHostAddress haMulticastAddress = QHostAddress("224.0.0.1"),
             int nMulticastPort = 45567, bool bConnectedToMulticast = true);
    void joinMulticastGroup(const QHostAddress &haMulticastGroup);
    void leaveMulticastGroup(const QHostAddress &haMulticastGroup);
private slots:
    void processSoundDatagrams();
    void playAudioOutput();
    void on_buttonJoinGroup_clicked();
    void on_buttonLeaveGroup_clicked();
private:
    QUdpSocket *m_pSocket;
    QAudioOutput *m_pAudioOutput;
    QIODevice *m_pOutputDevice;
    QHostAddress m_haMulticastAddress;
    int m_nMulticastPort;
    bool m_bConnectToMulticast;
};

#endif // RECEIVER_H
