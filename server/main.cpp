#include <iostream>
#include "dyad.h"
#include <map>
#include <vector>
#include <string>

class Player {
public:
	int x, y;
	std::string name;
};

std::map<std::string, Player> players;
std::vector<std::string> playerlist;

static void onData(dyad_Event* e) {
	std::string data;
	for (int i = 0; i < playerlist.size(); i++) {
		data += players.at(playerlist.at(i)).name + " " + std::to_string(players.at(playerlist.at(i)).x) + " " + std::to_string(players.at(playerlist.at(i)).y) + "\n";
	}
	std::string message = e->data;
	std::cout << message + "\n";
	std::string buffer = "";
	std::vector<std::string> tokens;
	for (int i = 0; i < message.size(); i++) {
		if (message.at(i) != ' ') buffer += message.at(i);
		if (message.at(i) == ' ' || i == message.size() - 1) {
			tokens.push_back(buffer);
			buffer.clear();
		}
	}
	if (tokens.size() == 3) {
		bool playerExists = false;
		for (int i = 0; i < playerlist.size(); i++) {
			if (playerlist.at(i) == tokens.at(0)) playerExists = true;
		}
		if (!playerExists) playerlist.push_back(tokens.at(0));
		Player player = { std::stoi(tokens.at(1)), std::stoi(tokens.at(2)), tokens.at(0) };
		players.insert_or_assign(tokens.at(0), player);
	}
	else {
		std::cout << "Message might be invalid... :\\ \n";
	}
	dyad_writef(e->stream, data.c_str());

}

static void onAccept(dyad_Event* e) {
	dyad_addListener(e->remote, DYAD_EVENT_DATA, onData, NULL);
	std::cout << "New client\n";
	
}

int main() {
	std::cout << "MPChallenge server\nWritten by Safariminer\nTo troll AmethystGear\n\n";
	dyad_init();
	dyad_Stream* serv = dyad_newStream();
	dyad_addListener(serv, DYAD_EVENT_ACCEPT, onAccept, NULL);
	dyad_listen(serv, 69);
	while (dyad_getStreamCount() > 0) {
		dyad_update();
	}
	dyad_shutdown();
	return 0;
}