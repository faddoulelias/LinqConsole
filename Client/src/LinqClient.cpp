#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "./headers/LinqClient.hpp"

ServerRequest::ServerRequest(std::string request)
{
    std::istringstream str_stream(request);
    std::string word;

    str_stream >> word;
    if (word == "CONNECTED")
        type = ServerRequestType::CONNECTED;
    else if (word == "DENIED")
        type = ServerRequestType::DENIED;
    else if (word == "START")
        type = ServerRequestType::START;
    else if (word == "PLAY")
        type = ServerRequestType::PLAY;
    else if (word == "SAID")
        type = ServerRequestType::SAID;
    else if (word == "VOTE")
        type = ServerRequestType::VOTE;
    else if (word == "VOTED")
        type = ServerRequestType::VOTED;
    else if (word == "REVEAL")
        type = ServerRequestType::REVEAL;
    else if (word == "WIN")
        type = ServerRequestType::WIN;
    else
        type = ServerRequestType::INVALID;

    while (str_stream >> word)
        args.push_back(word);
}

LinqClientGame::LinqClientGame()
{
    id = -1;
    username = "";
    role = PlayerRole::COUNTER_SPY;
    word = "";
    players = std::vector<std::string>();
}

LinqClientGame::~LinqClientGame() = default;

void LinqClientGame::handleStartRequest(ServerRequest request)
{
    if (request.type != ServerRequestType::START)
        return;

    this->role = (PlayerRole)std::stoi(request.args[1]);
    this->word = request.args[2];
    this->players = std::vector<std::string>(request.args.begin() + 3, request.args.end());
}