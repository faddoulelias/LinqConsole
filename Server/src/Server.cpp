#include <iostream>
#include <array>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "./headers/Server.hpp"

ServerConnection::ServerConnection(std::string ip, int port)
{
    this->ip = ip;
    this->port = port;
    this->last_client_id = 0;
}

ServerConnection::~ServerConnection()
{
    for (auto &client : this->clients)
    {
        close(client.second.socket);
    }
}

void ServerConnection::sendToClient(int client_id, std::string message)
{
    try
    {
        int ret = send(this->clients[client_id].socket, message.c_str(), message.size(), MSG_NOSIGNAL);
        if (ret < 0)
        {
            throw std::runtime_error("Error in sending data.\n");
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void ServerConnection::sendToAll(std::string message)
{
    for (auto &client : this->clients)
    {
        this->sendToClient(client.first, message);
    }
}

void ServerConnection::start(
    MessageHandler message_handler,
    ClientHandler client_connect_handler,
    ClientHandler client_disconnect_handler)
{
    int sockfd;
    int ret;
    struct sockaddr_in serverAddr;

    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        throw std::runtime_error("Error in connection.\n");
    }

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());

    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        throw std::runtime_error("Error in binding.\n");
    }

    ret = listen(sockfd, 10);
    if (ret < 0)
    {
        throw std::runtime_error("Error in binding.\n");
    }

    int newSocket;
    struct sockaddr_in newAddr;
    while (1)
    {
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);

        this->last_client_id++;
        int client_id = this->last_client_id;
        ClientInfo client;
        client.socket = newSocket;
        client.ip = inet_ntoa(newAddr.sin_addr);
        client.port = ntohs(newAddr.sin_port);

        this->clients[client_id] = client;

        if (newSocket < 0)
        {
            throw std::runtime_error("Error in binding.\n");
        }

        std::thread client_thread([this, client_id, message_handler, client_connect_handler, client_disconnect_handler]()
                                  {
            std::array<char, 1024> buffer = {0};
            while (1)
            {
                int ret = recv(this->clients[client_id].socket, buffer.data(), buffer.size(), 0);
                if (ret < 0)
                {
                    std::cout << "Error in receiving data.\n";
                    break;
                }
                else if (ret == 0)
                {
                    std::cout << "Client disconnected.\n";
                    break;
                }
                else
                {
                    std::string message(buffer.data(), ret);
                    message_handler(this, client_id, message);
                }
            }

            client_disconnect_handler(this, client_id);
            close(this->clients[client_id].socket);
            this->clients.erase(client_id); });

        client_thread.detach();
    }
}
