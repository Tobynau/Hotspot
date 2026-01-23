#ifndef IPERF3_WRAPPER_H
#define IPERF3_WRAPPER_H

#include <QString>

class Iperf3Wrapper {
public:
    Iperf3Wrapper();
    ~Iperf3Wrapper();

    // Run a client test against the given host. Returns true if started successfully.
    bool runClient(const QString &host, int port = 5201);
    // Stop any running test
    void stop();
};

#endif // IPERF3_WRAPPER_H
