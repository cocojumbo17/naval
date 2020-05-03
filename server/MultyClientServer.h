#pragma once
#include <winsock.h>
#include <vector>
#include "DB.h"
#include "RequestHandler.h"

struct ClientData
{
	ClientData():m_connected(false){};
	bool		m_connected;
	sockaddr_in	m_address;
	SOCKET		m_socket;
	fd_set		m_socket_data;
	int			m_result_op;
};

class MultyClientServer
{
public:
	MultyClientServer(int max_clients, int port);

	bool	AcceptClient(ClientData& current_client);
	void	AcceptConnections();
	void	DisconnectClient(ClientData& current_client);
	void	EndServer();
	void	Midcopy(char* input, char* output, int start_pos, int stop_pos);
	int		ReceiveClient(ClientData& current_client, char* buffer, int size);
	void	ReceiveData();
	bool	SendData(ClientData& current_client, const char* buffer, int size);
	void	StartServer();
	bool	IsFinish();

protected:
	const int	m_max_clients;
	sockaddr_in	m_server_address;
	sockaddr	m_server_socket_address;
	SOCKET		m_server_socket;
	std::vector<ClientData>	m_clients;
	int			m_clients_connected;
	int			m_port;
	bool		m_is_finish;
	DBPtr		m_db;
	RequestHandlerPtr m_request_handler;
};

