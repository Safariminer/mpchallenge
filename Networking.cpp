#include <iostream>
#include "Player.h"
#include <vector>
#include "dyad.h"
#include <map>
#include <string>
std::string tosend;
std::vector<std::string> playerslist;
std::map<std::string, Player> playerinfo;

void ParseMessage(std::string message) {
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
		for (int i = 0; i < playerslist.size(); i++) {
			if (playerslist.at(i) == tokens.at(0)) playerExists = true;
		}
		if (!playerExists) playerslist.push_back(tokens.at(0));
		Player player = { std::stoi(tokens.at(1)), std::stoi(tokens.at(2)), tokens.at(0) };
		playerinfo.insert_or_assign(tokens.at(0), player);
	}
	else {
		std::cout << "Message might be invalid... :\\ \n";
	}
}

static void onError(dyad_Event* e) {
	std::cout << "NetError: " << e->msg;
	abort();
}

static void onConnect(dyad_Event* e) {
	std::cout << "Connected to server. YIPPEEEEEEEE\n";
}
static void onData(dyad_Event* e) {
	std::string data = e->data;
	std::cout << e->data;
	std::vector<std::string> messages;
	std::string buffer;
	for (int i = 0; i < data.size(); i++) {
		if (data.at(i) == '\n' || i == data.size()-1) { 
			messages.push_back(buffer);
			buffer.clear();
		}
		else buffer += data.at(i);
	}

	for (int i = 0; i < messages.size(); i++) {
		ParseMessage(messages.at(i));
	}

	messages.clear();
}
static void onTick(dyad_Event* e){
	if (tosend != "") {
		dyad_writef(e->stream, tosend.c_str());
		tosend = "";
	}
}

std::vector<Player> GetAllPlayers(){
	std::vector<Player> playersget;
	for (int i = 0; i < playerslist.size(); i++) {
		playersget.push_back(playerinfo.at(playerslist.at(i)));
	}
	return playersget;
}

void NetworkUpdate() {
	dyad_update();
}
dyad_Stream* s;
void ConnectToServer(std::string ip, int port){
	dyad_init();
	s = dyad_newStream();
	dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
	dyad_addListener(s, DYAD_EVENT_DATA, onData, NULL);
	dyad_addListener(s, DYAD_EVENT_ERROR, onError, NULL);
	dyad_addListener(s, DYAD_EVENT_TICK, onTick, NULL);
	dyad_connect(s, ip.c_str(), port);

	dyad_setNoDelay(s, true);
	dyad_setTickInterval(1);
	dyad_setUpdateTimeout(0);
	
	
}
void DisconnectFromServer(){
	dyad_shutdown();
}
void SendPacket(std::string message){
	tosend = message;
}