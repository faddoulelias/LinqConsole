#pragma once
#include <string>
#include <vector>

enum class PlayerRole
{
    COUNTER_SPY,
    SPY
};

enum class ServerRequestType
{
    CONNECTED,
    DENIED,
    START,
    PLAY,
    SAID,
    VOTE,
    VOTED,
    REVEAL,
    WIN,
    INVALID
};

struct ServerRequest
{
    ServerRequestType type;
    std::vector<std::string> args;

    ServerRequest(std::string request);
};

class LinqClientGame
{
public:
    int id;
    std::string username;
    PlayerRole role;
    std::string word;
    std::vector<std::string> players;

    LinqClientGame();
    ~LinqClientGame();

    void handleStartRequest(ServerRequest request);
};