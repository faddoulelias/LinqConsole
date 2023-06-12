#pragma once
#include <functional>
#include <string>

class ClientConnection;

typedef std::function<void(ClientConnection *, std::string)> MessageCallback;

class ClientConnection
{
private:
    int socketfd;
    std::string ip;
    int port;

public:
    ClientConnection(std::string ip, int port);
    ~ClientConnection();
    void connectToServer();
    void onMessage(MessageCallback callback);
    void sendMessage(std::string message);
};