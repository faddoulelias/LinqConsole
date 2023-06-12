#include <iostream>
#include "./headers/Server.hpp"
#include "./headers/LinqServer.hpp"

#define PORT 4444

LinqGame game;

void onMessage(ServerConnection *server, int client_id, std::string message)
{
	PlayerRequest request(message);
	if (request.type == ClientRequestType::CONNECT)
	{
		std::cout << "Client " << client_id << " connected with username " << request.args[0] << std::endl;
		game.addPlayer(client_id, request.args[0]);

		for (const Player &player : game.players)
		{
			std::string connection_message = game.generateConnectionMessageForPlayer(player.id);
			server->sendToClient(player.id, connection_message);
		}

		if (game.players.size() == 5)
		{
			game.assignRoles();
			game.shufflePlayers();
			for (const Player &player : game.players)
			{
				std::string start_message = game.generateStartMessage(player.id);
				server->sendToClient(player.id, "\n");
				server->sendToClient(player.id, start_message);
			}

			game.startGame();

			std::string play_message = game.generatePlayMessage(game.turn);
			for (const Player &player : game.players)
			{
				server->sendToClient(player.id, "\n");
				server->sendToClient(player.id, play_message);
			}
		}
	}
	if (request.type == ClientRequestType::WORD)
	{
		int player_id = std::stoi(request.args[0]);
		std::cout << game.getPlayerById(player_id).username << " said " << request.args[1] << std::endl;
		game.getPlayerById(player_id).words.push_back(request.args[1]);

		std::string said_message = game.generateSaidMessage(player_id);
		for (const Player &player : game.players)
		{
			server->sendToClient(player.id, "\n");
			server->sendToClient(player.id, said_message);
		}

		game.nextTurn();
		std::string play_message = game.generatePlayMessage(game.turn);
		for (const Player &player : game.players)
		{
			server->sendToClient(player.id, "\n");
			server->sendToClient(player.id, play_message);
		}
	}
	else if (request.type == ClientRequestType::CAST)
	{
		int player_id = std::stoi(request.args[0]);
		int target1_id = game.getPlayerByUsername(request.args[1]).id;
		int target2_id = game.getPlayerByUsername(request.args[2]).id;

		game.getPlayerById(player_id).votes.first = target1_id;
		game.getPlayerById(player_id).votes.second = target2_id;

		std::string cast_message = game.generateVotedMessage(player_id);
		for (const Player &player : game.players)
		{
			server->sendToClient(player.id, "\n");
			server->sendToClient(player.id, cast_message);
		}

		bool isFinished = game.nextTurn();
		if (isFinished)
		{
			std::string winners_message = game.generateWinMessage();
			for (const Player &player : game.players)
			{
				server->sendToClient(player.id, "\n");
				server->sendToClient(player.id, winners_message);
			}
			return;
		}
		else
		{
			std::string play_message = game.generatePlayMessage(game.turn);
			for (const Player &player : game.players)
			{
				server->sendToClient(player.id, "\n");
				server->sendToClient(player.id, play_message);
			}
		}
	}
	else
	{
		std::cout << "Client " << client_id << " sent " << message << std::endl;
	}
}

void onConnect(ServerConnection *server, int client_id)
{
	std::cout << "Client " << client_id << " connected" << std::endl;
}

void onDisconnect(ServerConnection *server, int client_id)
{
	std::cout << "Client " << client_id << " disconnected" << std::endl;
}

int main()
{
	ServerConnection server(
		"127.0.0.1",
		PORT);

	server.start(
		onMessage,
		onConnect,
		onDisconnect);
}
