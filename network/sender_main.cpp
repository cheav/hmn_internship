#include <QApplication>
#include <QHostAddress>

#include "sender.h"

int main(int nArgC, char *apszArgV[])
{
    QApplication app(nArgC, apszArgV);

    // Local Network IP Multicast 224.0.0.0 - 224.0.0.255, 239.255.43.21

    QHostAddress haMulticastAddress("224.0.0.1");
    int nMulticastPort = 45567;

    Sender sender({}, haMulticastAddress, nMulticastPort);
    sender.move(50, 100);
    sender.show();

    return app.exec();
}
