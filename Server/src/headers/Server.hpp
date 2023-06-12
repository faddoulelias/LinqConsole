#pragma once
#include <functional>
#include <string>
#include <vector>
#include <map>

class ClientInfo;
class ServerConnection;

typedef std::function<void(ServerConnection *, int, std::string)> MessageHandler;
typedef std::function<void(ServerConnection *, int)> ClientHandler;

class ClientInfo
{
public:
    int socket;
    std::string ip;
    int port;
};

class ServerConnection
{
private:
    int last_client_id;
    std::string ip;
    int port;

public:
    std::map<int, ClientInfo> clients;

    ServerConnection(std::string ip, int port);
    ~ServerConnection();
    void start(
        MessageHandler message_handler,
        ClientHandler client_connect_handler,
        ClientHandler client_disconnect_handler);

    void sendToClient(int client_id, std::string message);
    void sendToAll(std::string message);
};