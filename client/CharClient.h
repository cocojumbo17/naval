#pragma once
#include <winsock.h>
#include <vector>

class CharClient
{
public:
	CharClient(const char* server_ip, int port);

	void	EndClient();
	void	ReceiveData(char* buffer, int size);
	bool	SendData(char* buffer, int size);
	void	StartClient();
	bool	IsFinish();

protected:
	sockaddr_in	m_client_address;
	sockaddr	m_client_socket_address;
	SOCKET		m_client_socket;
	int			m_port;
	bool		m_is_finish;
	const char* m_server_ip;
	int			m_result_op;
};

