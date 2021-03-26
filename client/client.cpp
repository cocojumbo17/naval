
#include "NavalClient.h"
#include <iostream>
#include <sstream>
#include <conio.h>
#include "..\common\common.h"
#include <time.h>

#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "ERROR: Incorrec format. Should be: client.exe <serverIP> <server port>";
		return 0;
	}
	std::string adress = argv[1];
	int port = atoi(argv[2]);
	srand((unsigned int)time(NULL));

	//char buffer[BUFFER_SIZE];
	std::cout << "\t\tNaval Client\n\n";
	NavalClient client(adress.c_str(), port);
//	NavalClient client(SERVER_ADRESS, PORT);
	client.StartClient();
	client.InitPlayer();
	bool looping = true;
	while (looping)
	{
		char choise;
		while (true)
		{
			std::cout << "\n0-exit 1-start new game\n>";
			choise = _getche();
			int l = choise - '0';
			if (l >= 0 and l < 2)
				break;
		}
		if (choise == '0')
			looping = false;
		else
		{
			client.PlayGame();
			looping = !client.IsFinish();
		}
	}
	client.EndClient();
	return 0;
}