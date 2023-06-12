#include <iostream>
#include <string>
#include <thread>

#include "./headers/Client.hpp"
#include "./headers/LinqClient.hpp"

#define PORT 4444
LinqClientGame game;

void onMessage(ClientConnection *client, std::string message)
{
	ServerRequest request(message);
	if (request.type == ServerRequestType::CONNECTED)
	{
		std::cout << "Connected to server" << std::endl;
		game.id = std::stoi(request.args[0]);
		game.players = std::vector<std::string>(request.args.begin() + 1, request.args.end());
		std::cout << "Your ID is " << game.id << std::endl;
		std::cout << "Players in the game:" << std::endl;
		for (const std::string &player : game.players)
			std::cout << player << std::endl;
	}
	else if (request.type == ServerRequestType::START)
	{
		game.handleStartRequest(request);

		// Clear the console
		std::cout << std::string(100, '\n');
		std::cout << "Your role is " << (game.role == PlayerRole::COUNTER_SPY ? "Counter-Spy" : "Spy") << std::endl;

		if (game.role == PlayerRole::SPY)
			std::cout << "Your word is " << game.word << std::endl;

		std::cout << "Players in the game:" << std::endl;
		for (const std::string &player : game.players)
			if (player != game.username)
				std::cout << player << std::endl;
			else
				std::cout << player << " (You)" << std::endl;
	}
	else if (request.type == ServerRequestType::PLAY)
	{
		if (std::stoi(request.args[0]) == game.id)
		{
			std::cout << "It's your turn" << std::endl;
			std::cout << "Enter your word: ";
			std::string word;
			std::cin >> word;
			client->sendMessage("WORD " + std::to_string(game.id) + " " + word);
		}
		else
		{
			std::cout << "It's " << request.args[1] << "'s turn" << std::endl;
		}
	}
	else if (request.type == ServerRequestType::SAID)
	{
		std::cout << request.args[0] << " said " << request.args[1] << std::endl;
	}
	else if (request.type == ServerRequestType::VOTE)
	{
		if (std::stoi(request.args[0]) == game.id)
		{
			std::cout << "It's your turn to vote" << std::endl;
			std::cout << "Enter the names of the two players you suspect (separated by a space): ";
			std::string player1, player2;
			std::cin >> player1 >> player2;

			client->sendMessage("CAST " + std::to_string(game.id) + " " + player1 + " " + player2);
		}
		else
		{
			std::cout << "It's " << request.args[1] << "'s turn to vote" << std::endl;
		}
	}
	else if (request.type == ServerRequestType::VOTED)
	{
		std::cout << request.args[0] << " voted for " << request.args[1] << " and " << request.args[2] << std::endl;
	}
	else if (request.type == ServerRequestType::WIN)
	{
		PlayerRole winner = (PlayerRole)std::stoi(request.args[0]);
		if (winner == game.role)
			std::cout << "You won!" << std::endl;
		else
			std::cout << "You lost!" << std::endl;

		std::cout << "Winners:" << std::endl;
		for (size_t i = 1; i < request.args.size(); i++)
			std::cout << request.args[i] << std::endl;
	}
	else
	{
		std::cout << "Unknown message: " << message << std::endl;
	}
}

int main()
{
	ClientConnection client("127.0.0.1", PORT);
	client.connectToServer();

	client.onMessage(onMessage);

	std::string username;
	std::cout << "Enter your username: ";
	std::cin >> username;

	client.sendMessage("CONNECT " + username);
	game.username = username;

	while (1)
		;

	return 0;
}
