#pragma once
#include <iostream>
#include <vector>


class Player {
public:
	float x, y;
	std::string name;
	void DrawPlayer();
};

std::vector<Player> GetAllPlayers();
void ConnectToServer(std::string ip, int port);
void DisconnectFromServer();
void SendPacket(std::string message);
void NetworkUpdate();