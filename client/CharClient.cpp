#include "CharClient.h"
#include <iostream>


CharClient::CharClient(const char* server_ip, int port)
	: m_server_ip(server_ip)
	, m_port(port)
	, m_is_finish(false)
{	
}

void CharClient::StartClient()
{
	// Initialize the server and start listening for clients
	WSADATA wsaData;
	int res, i = 1;
	// Set up the address structure
	m_client_address.sin_family = AF_INET;
	m_client_address.sin_addr.s_addr = inet_addr(m_server_ip);
	m_client_address.sin_port = htons(m_port);
	// IM GUESSING : Copy over some addresses, conversions of some sort ?
	memcpy(&m_client_socket_address, &m_client_address, sizeof(SOCKADDR_IN));
	res = WSAStartup(MAKEWORD(1, 1), &wsaData);
	std::cout << "\n\nWSAStartup"
		<< "\nVersion: " << wsaData.wVersion
		<< "\nDescription: " << wsaData.szDescription
		<< "\nStatus: " << wsaData.szSystemStatus << std::endl;


	// Create a listening socket for the server
	m_client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_client_socket == INVALID_SOCKET)
	{
		std::cout << "SOCKET ERROR : Invalid socket." << std::endl;
	}
	else if (m_client_socket == SOCKET_ERROR)
	{
		std::cout << "SOCKET ERROR : Socket error." << std::endl;
	}
	else
	{
		std::cout << "SOCKET ESTABLISHED" << std::endl;
	}

	//Connect to the server
	res = connect(m_client_socket, &m_client_socket_address, sizeof(m_client_socket_address));
	if (res != 0)
	{
		std::cout << "SERVER UNAVAILABLE: " << res << std::endl;
	}
	else
	{
		std::cout << "\nConnected to Server: ";
	}
}
void CharClient::EndClient()
{
	// Close the socket with the server
	closesocket(m_client_socket);
	// Clean up winsock
	WSACleanup();
}

bool CharClient::SendData(char* buffer, int size)
{
	// Store the return information about the sending
	m_result_op = send(m_client_socket, buffer, size, 0);
	if ((m_result_op == SOCKET_ERROR) || (m_result_op == 0))
	{ // Check for errors while sending
		std::cout << "SERVER terminated connection" << std::endl;
		m_is_finish = true;
		return false;
	}
	return true;
}

void CharClient::ReceiveData(char* buffer, int size)
{
	//Recive Data
	int res = recv(m_client_socket, buffer, size, 0);
	if (res > 0)
	{
		std::cout << "\nRecieved string:" << buffer;
	}
}

bool CharClient::IsFinish()
{
	return m_is_finish;
}

