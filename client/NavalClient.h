#pragma once
#include <winsock.h>
#include <queue>
#include "../common/RequestParser.h"
#include "../common/ResponseParser.h"

class NavalClient
{
public:
	NavalClient(const char* server_ip, int port);

	void	StartClient();
	void	EndClient();
	void	ReceiveData(char* buffer, int size);
	bool	SendData(const char* buffer, int size);
	bool	IsFinish();

	void	InitPlayer();
	void	PlayGame();
	bool	StartGame();
	bool	PlayRound(bool my_turn);
	bool	IsGameFinished(int& winner_id);
	void	FinishGame(int winner_id);
protected:
	bool SendRequest(IRequestPtr rq);
	IResponsePtr GetResponse();

protected:
	sockaddr_in	m_client_address;
	sockaddr	m_client_socket_address;
	SOCKET		m_client_socket;
	int			m_port;
	bool		m_is_finish;
	const char* m_server_ip;
	int			m_result_op;

	std::string		m_player_name;
	int				m_player_id;
	PLAYER_LEVEL	m_player_level;
	int				m_game_id;
	IPlayerPtr		m_player;
	std::queue<IResponsePtr> m_responses;
};

