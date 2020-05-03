/*
// COMMENTS //
	Author	:	xmen90s -> based on the tutorial of Aaron aka FireNet
				[email]xmen90s@yahoo.com[/email]
	Date	:	June 10th of 2004
	Purpose	:	This server receives input from multiple clients and keeps
				a RAM database of a few pieces of information like screen
				name, character model, and position
				The server then will echo back information to all clients
				instructing them on where to move actors
				Essentially I'm working on rebuilding the multiplayer system
				of a game called Dungeon Siege from ground up.
	I want to put a special thanks out to Aaron aka FireNet for writing
	the most excellent tutorial I have ever read.  I went from knowing
	cin and cout to writing a full non-blocking multi-client echo server
	in just one and a half weeks because of him.
// LINKS //
	Remember to link to : wsock32.lib
*/
#include "MultyClientServer.h"
#include <iostream>
#include "..\common\common.h"
#include <time.h>

#pragma comment (lib, "Ws2_32.lib")

int main()
{
	srand((unsigned int)time(NULL));

	std::cout << "\tNaval Server\n" << std::endl;
	MultyClientServer server(MAX_CLIENTS, PORT);
	// Initialize winsock and start listening
	server.StartServer();
	// Loop forever
	bool looping = true;
	while (looping)
	{
		// Accept all incoming client connections
		server.AcceptConnections();
		// Receive all data from clients
		server.ReceiveData();
		looping = !server.IsFinish();
		Sleep(10);
	}
	// Shut down winsock
	server.EndServer();
	return 0;
}