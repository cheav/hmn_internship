#ifndef SENDER_H
#define SENDER_H

#include <QDialog>
#include <QHostAddress>

#include <cmath>

#include "ui_sender_dialog.h"

class QUdpSocket;
class QTimer;

class Sender : public QDialog, public Ui::SenderDialog
{
    Q_OBJECT
public:
    Sender(QWidget *pParent = nullptr, QHostAddress haMulticastAddress = QHostAddress("224.0.0.1"),
           int nMulticastPort = 45567);
protected:
    void closeEvent(QCloseEvent*) override;
private slots:
    void on_buttonSend_clicked();
    void on_buttonStop_clicked();
    //
    void sendDatagram();
private:
    QUdpSocket *m_pSocket;
    QTimer *m_pTimer;
    //
    QHostAddress m_haMulticastAddress;
    int m_nMulticastPort;
    //
    struct SoundSignal
    {
        SoundSignal();
        QByteArray m_baSoundSignal;
    };
};

#endif // SENDER_H
