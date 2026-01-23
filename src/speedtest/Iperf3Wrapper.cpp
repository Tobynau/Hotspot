#include "Iperf3Wrapper.h"
#include <QDebug>

Iperf3Wrapper::Iperf3Wrapper(){}
Iperf3Wrapper::~Iperf3Wrapper(){}

bool Iperf3Wrapper::runClient(const QString &host, int port)
{
    qDebug() << "Iperf3Wrapper::runClient" << host << port;
    // TODO: spawn iperf3 or use libiperf bindings
    return false;
}

void Iperf3Wrapper::stop()
{
    qDebug() << "Iperf3Wrapper::stop";
}
