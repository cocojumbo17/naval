#include "MultyClientServer.h"
#include <iostream>
#include "..\common\common.h"
#include "../common/RequestParser.h"


MultyClientServer::MultyClientServer(int max_clients, int port)
	: m_max_clients(max_clients)
	, m_port(port)
	, m_clients_connected(0)
	, m_clients(m_max_clients, ClientData())
	, m_is_finish(false)
{
	m_db = std::make_shared<DB>();
	m_request_handler = std::make_shared<RequestHandler>(m_db);
}

void MultyClientServer::StartServer()
{
	// Initialize the server and start listening for clients
	// LOCAL VARIABLES //
	WSADATA wsaData;
	int res, i = 1;
	// Set up the address structure
	m_server_address.sin_family = AF_INET;
	m_server_address.sin_addr.s_addr = INADDR_ANY;
	m_server_address.sin_port = htons(m_port);
	// IM GUESSING : Copy over some addresses, conversions of some sort ?
	memcpy(&m_server_socket_address, &m_server_address, sizeof(SOCKADDR_IN));
	res = WSAStartup(MAKEWORD(1, 1), &wsaData);
	// Start winsock
	if (res != 0)
	{
		std::cout << "WSADATA ERROR : Error while attempting to initialize winsock." << std::endl;
	}
	// Create a listening socket for the server
	m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_server_socket == INVALID_SOCKET)
	{
		std::cout << "SOCKET ERROR : Invalid socket." << std::endl;
	}
	else if (m_server_socket == SOCKET_ERROR)
	{
		std::cout << "SOCKET ERROR : Socket error." << std::endl;
	}
	else
	{
		std::cout << "SOCKET ESTABLISHED" << std::endl;
	}
	// Sets the option to re-use the address the entire run of the program
	setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i));
	// Bind the socket to the address
	res = bind(m_server_socket, &m_server_socket_address, sizeof(m_server_socket_address));
	std::cout << "Binding socket:" << res << std::endl;
	if (res != 0)
	{
		std::cout << "BINDING ERROR : Failed to bind socket to address." << std::endl;
	}
	else
	{
		std::cout << "Socket Bound to port : " << m_port << std::endl;
	}
	// Start listening for connection requests
	res = listen(m_server_socket, 8);
	// This makes the server non blocking, hence it won't wait for a response
	unsigned long b = 1;
	ioctlsocket(m_server_socket, FIONBIO, &b);
	// Clear all clients state
	for (int j = 0; j < m_max_clients; ++j) 
	{ 
		m_clients[j].m_connected = false; 
	}
}
void MultyClientServer::EndServer()
{
	// Shut down the server by disconnecting all clients and clearing winsock
	// Disconnect all clients
	for (int j = 0; j < m_max_clients; ++j)
	{ 
		DisconnectClient(m_clients[j]); 
	}
	// Close the listening socket for the server
	closesocket(m_server_socket);
	// Clean up winsock
	WSACleanup();
}
bool MultyClientServer::AcceptClient(ClientData& current_client)
{
	// Accept incoming connections
	current_client.m_result_op = sizeof(sockaddr);
	current_client.m_socket = accept(m_server_socket, (sockaddr*)&current_client.m_address, &current_client.m_result_op);
	if (current_client.m_socket == 0)
	{
		// No data in socket
		return false;
	}
	else if (current_client.m_socket == SOCKET_ERROR)
	{
		// Socket error
		return false;
	}
	else
	{
		// Occupy the client slot
		current_client.m_connected = true;
		FD_ZERO(&current_client.m_socket_data);
		FD_SET(current_client.m_socket, &current_client.m_socket_data);
		return true;
	}
	return false;

}
void MultyClientServer::AcceptConnections()
{
	if (m_clients_connected < m_max_clients)
	{
		for (int j = 0; j < m_max_clients; ++j)
		{
			if (!m_clients[j].m_connected)
			{
				if (AcceptClient(m_clients[j]))
				{
					// Increment the client count
					++m_clients_connected;
					// Grab the ip address of the client ... just for fun
					char* client_ip_address = inet_ntoa(m_clients[j].m_address.sin_addr);
					// Output connection
					std::cout << "ACCEPTING CLIENT to array position [" << j << "] with IP ADDRESS " << client_ip_address << std::endl;
				}
			}
		}
	}
}

void MultyClientServer::DisconnectClient(ClientData& current_client)
{
	// Disconnect a client
	if (current_client.m_connected)
	{ // Close the socket for the client
		closesocket(current_client.m_socket);
	}
	// Set the new client state
	current_client.m_connected = false;
	// Clear the address length
	current_client.m_result_op = -1;
	// Decrement the current number of connected clients
	m_clients_connected--;
	// Declare a variable to store the disconnect message into
	//char raw_data[BUFFER_SIZE];
	// Parse in the client data to send
//	sprintf_s(raw_data, "~4 %s", current_client.m_screen_name);
	// Echo out the disconnect message so all clients drop this client
	//EchoMessage(raw_data);
	std::cout << "Disconnecting client[]" << std::endl;
}

bool MultyClientServer::SendData(ClientData& current_client, const char* buffer, int size)
{
	// Store the return information about the sending
	current_client.m_result_op = send(current_client.m_socket, buffer, size, 0);
	if ((current_client.m_result_op == SOCKET_ERROR) || (current_client.m_result_op == 0))
	{ // Check for errors while sending
		DisconnectClient(current_client);
		return false;
	}
	else 
		return true;

}

void MultyClientServer::ReceiveData()
{
	char buffer[BUFFER_SIZE];
	for (int j = 0; j < m_max_clients; ++j)
	{
		if (m_clients[j].m_connected)
		{
			if (ReceiveClient(m_clients[j], buffer, BUFFER_SIZE))
			{
				IRequestPtr rq = RequestParser::Parse(buffer);
				m_request_handler->AddRequest(j, rq);

				/*

				//HandleRequest(rq);
				if (buffer[0] == '~')
				{ // All data should be buffered by a '~' just because
					if (buffer[1] == '1') // Add Client Command
					{
						// Declare the buffer to store new client information into
						char raw_data[BUFFER_SIZE];
						// Parse out the 'Add Client' command
						Midcopy(buffer, raw_data, 3, strlen(buffer));
						// Store the client information into our RAM client database
//						sscanf_s(raw_data, "%s %s %s", m_clients[j].m_template_name, (unsigned)_countof(m_clients[j].m_template_name),
//														m_clients[j].m_screen_name, (unsigned)_countof(m_clients[j].m_screen_name), 
//														m_clients[j].m_siegepos, (unsigned)_countof(m_clients[j].m_siegepos));
						for (int k = 0; k < m_max_clients; k++)
						{
							if ((m_clients[k].m_connected) && (j != k))
							{
								// Parse in the client data to send
//								sprintf_s(raw_data, "~1 %s %s %s", m_clients[k].m_template_name, m_clients[k].m_screen_name, m_clients[k].m_siegepos);
								// Send the client data
								SendData(m_clients[j], raw_data, BUFFER_SIZE);
							}
						}
					}
					else if (buffer[1] == '2') // Move Client Command
					{
						// Declare the buffer to store new client information into
						char raw_data[BUFFER_SIZE];
						// Parse out the 'Move Client' command
						Midcopy(buffer, raw_data, 3, strlen(buffer));
						// Update the client information into our RAM client database
//						sscanf_s(raw_data, "%s %s", m_clients[j].m_screen_name, (unsigned)_countof(m_clients[j].m_screen_name),
//							m_clients[j].m_siegepos, (unsigned)_countof(m_clients[j].m_siegepos));
					}
					else if (buffer[1] == '3') // Chat Client Command
					{
						// ECHO THE MESSAGE BACK TO ALL CLIENTS
					}
					else if (buffer[1] == '4') // Remove Client Command
					{
						// Disconnect the current client
						DisconnectClient(m_clients[j]);
					}
					else if (buffer[1] == '6') // Remove Client Command
					{
						// Kill server
						m_is_finish = true;
					}
					// Display all data received
					// cout << buffer << endl;
					// Echo the message to the other clients
					EchoMessage(buffer);
					// Clear the buffer
					buffer[0] = '\0';
				}

				*/

			}
		}
	}
	m_request_handler->Listen();
	int client_id;
	IResponsePtr rs;
	if (m_request_handler->Answer(client_id, rs))
	{
		std::string str = rs->PackToString();
		SendData(m_clients[client_id], str.c_str(), str.size());
	}

}

void MultyClientServer::Midcopy(char* input, char* output, int start_pos, int stop_pos)
{
	int index = 0;
	for (int i = start_pos; i < stop_pos; i++)
	{
		output[index] = input[i];
		index++;
	}
	output[index] = 0;
}

int MultyClientServer::ReceiveClient(ClientData& current_client, char* buffer, int size)
{
	if (FD_ISSET(current_client.m_socket, &current_client.m_socket_data))
	{
		// Store the return data of what we have sent
		current_client.m_result_op = recv(current_client.m_socket, buffer, size, 0);
		if (current_client.m_result_op == 0)
		{ // Data error on client
			DisconnectClient(current_client);
			return false;
		}
		else if (current_client.m_result_op == -1)
			return false;
		buffer[current_client.m_result_op] = 0;
		return true;
	}
	return false;
}

bool MultyClientServer::IsFinish()
{
	return m_is_finish;
}

