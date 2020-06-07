#include <QApplication>
#include <QHostAddress>
#include <vector>

#include "receiver.h"

int main(int nArgC, char *apszArgV[])
{
    QApplication app(nArgC, apszArgV);
    //
    // Local Network IP Multicast 224.0.0.0 - 224.0.0.255, 239.255.43.21
    //
    QHostAddress haMulticastAddress("224.0.0.1");
    int nMulticastPort = 45567;
    bool bConnectToGroup = true;

    Receiver receiver({}, haMulticastAddress, nMulticastPort, bConnectToGroup);
    receiver.joinMulticastGroup(haMulticastAddress);
    //
    receiver.move(700, 50);
    receiver.show();
    //
    return app.exec();
}
