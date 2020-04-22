
#include "CharClient.h"
#include <iostream>
#include "..\common\common.h"
#pragma comment (lib, "Ws2_32.lib")

int main()
{
	char buffer[BUFFER_SIZE];
	char name[BUFFER_SIZE];
	std::cout << "\t\tEcho Client";
	CharClient client(SERVER_ADRESS, PORT);
	client.StartClient();
	std::cout << "My name is:\n>";
	fgets(name, BUFFER_SIZE, stdin);
	sprintf_s(buffer, "~1 template_name %s siegepos", name);
	if (client.SendData(buffer, BUFFER_SIZE))
		client.ReceiveData(buffer, BUFFER_SIZE);

	bool looping = true;
	while (looping)
	{
		std::cout << "1-listen 2-send message\n>";
		if (getchar() == '2')
		{
			fgets(buffer, BUFFER_SIZE, stdin);
			if (client.SendData(buffer, BUFFER_SIZE))
				client.ReceiveData(buffer, BUFFER_SIZE);
		}
		else
			client.ReceiveData(buffer, BUFFER_SIZE);
		looping = !client.IsFinish();
	}
	client.EndClient();
	return 0;
}