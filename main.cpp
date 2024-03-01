#include <iostream>
#include <raylib.h>
#include "Player.h"
#include <vector>

void Player::DrawPlayer() {
	DrawCircle(x, y, 10, RED);
	DrawText(name.c_str(), x, y, 10, WHITE);
}

int main(int argc, char* argv[]) {
	InitWindow(640, 480, "MPChallenge");
	SetTargetFPS(60);
	
	if (argc < 2) {
		std::cout << "No username. L+Ratio\n";
		return 1;
	}

	Player player;
	player.name = (std::string)argv[1];
	player.x = 0; player.y = 0;
	std::cout << "Player is called " << player.name << std::endl;
	std::vector<Player> players;
	ConnectToServer("localhost", 69);
	while (!WindowShouldClose()) {
		NetworkUpdate();
		players.clear(); players = GetAllPlayers();
		BeginDrawing();
		
		ClearBackground(GREEN);
		if (IsKeyDown(KEY_LEFT)) player.x--;
		if (IsKeyDown(KEY_RIGHT)) player.x++;
		if (IsKeyDown(KEY_UP)) player.y--;
		if (IsKeyDown(KEY_DOWN)) player.y++;
		SendPacket(TextFormat("%s %i %i", player.name.c_str(), (int)player.x, (int)player.y));
		for (int i = 0; i < players.size(); i++) {
			players.at(i).DrawPlayer();
		}
		
		player.DrawPlayer();
		EndDrawing();
	}
	DisconnectFromServer();
	CloseWindow();
}