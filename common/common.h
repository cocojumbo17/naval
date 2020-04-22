
#define		SERVER_ADRESS "192.168.1.9"		// Define server adress to connect to
#define		PORT 4000						// Define the port to connect to
#define		MAX_CLIENTS 10					// Define the maximum number of clients we can receive
#define		BUFFER_SIZE 256					// Define the buffer size of the messages

/*
//DECLARATIONS
//error trapping signals
#define SIGINT 2
#define SIGKILL 9
#define SIGQUIT 3

//SOCKETS
SOCKET sock, client;

void s_handle(int s)
{
	if (sock)
		closesocket(sock);
	if (client)
		closesocket(client);
	WSACleanup();
	std::cout << "EXIT SIGNAL :" << s;
	exit(0);
}
void s_cl(const char* a, int x)
{
	std::cout << a;
	s_handle(x + 1000);
}
*/