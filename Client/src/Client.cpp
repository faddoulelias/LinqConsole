#include <iostream>
#include <string.h>
#include <array>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "./headers/Client.hpp"

ClientConnection::ClientConnection(std::string ip, int port)
{
    this->ip = ip;
    this->port = port;
}

ClientConnection::~ClientConnection() = default;

void ClientConnection::connectToServer()
{
    int clientSocket, ret;
    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Client Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());

    ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    this->socketfd = clientSocket;
}

std::vector<std::string> split(std::string str, std::string delimiter)
{
    std::vector<std::string> splitted;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);

        if (token != "")
            splitted.push_back(token);

        str.erase(0, pos + delimiter.length());
    }

    if (str != "")
        splitted.push_back(str);

    return splitted;
}

void ClientConnection::onMessage(MessageCallback callback)
{
    std::thread([=]()
                {
        std::array<char, 1024> buffer = {0};
        while (1)
        {
            int valread = read(this->socketfd, buffer.data(), 1024);
            if (valread > 0)
            {
                std::vector<std::string> splitted = split(std::string(buffer.data()), "\n");
                for (const std::string &message : splitted)
                    callback(this, std::string(message));
            }

            buffer.fill(0);
        } })
        .detach();
}

void ClientConnection::sendMessage(std::string message)
{
    send(this->socketfd, message.c_str(), message.size(), 0);
}