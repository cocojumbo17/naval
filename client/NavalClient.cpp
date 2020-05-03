#include "NavalClient.h"
#include <iostream>
#include <sstream>
#include "../common/common.h"
#include "../common/interfaces.h"
#include <conio.h>
#include "Strategy.h"
#include "Player.h"

char g_buffer[BUFFER_SIZE];

NavalClient::NavalClient(const char* server_ip, int port)
	: m_server_ip(server_ip)
	, m_port(port)
	, m_is_finish(false)
{	
}

void NavalClient::StartClient()
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
void NavalClient::EndClient()
{
	// Close the socket with the server
	closesocket(m_client_socket);
	// Clean up winsock
	WSACleanup();
}

bool NavalClient::SendData(const char* buffer, int size)
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

void NavalClient::ReceiveData(char* buffer, int size)
{
	//Recive Data
	int res = recv(m_client_socket, buffer, size, 0);
	if (res > 0)
	{
		buffer[res] = 0;
	}
}

bool NavalClient::IsFinish()
{
	return m_is_finish;
}

bool NavalClient::SendRequest(IRequestPtr rq)
{
	std::string str = RequestParser::Pack(rq);
	return SendData(str.c_str(), str.size());
}
IResponsePtr NavalClient::GetResponse()
{
	if (m_responses.empty())
	{
		ReceiveData(g_buffer, BUFFER_SIZE);
		std::string str(g_buffer);
		// It is possible to get more than 1 response at a time.
		// So we should store them and use one by one.
		size_t offset = 0;
		std::string sep("</response>");
		do
		{
			size_t new_offset = str.find(sep, offset);
			if (new_offset == std::string::npos)
				break;
			new_offset += sep.size();
			std::string oneres = str.substr(offset, new_offset - offset);
			IResponsePtr rs = ResponseParser::Parse(oneres);
			m_responses.push(rs);
			offset = new_offset;
		} while (true);
	}
	
	IResponsePtr res = m_responses.front();
	m_responses.pop();
	return res;
}

void NavalClient::InitPlayer()
{
	char name[BUFFER_SIZE];
	std::cout << "My name is:\n>";
	std::cin >> name;
	m_player_level = PLAYER_LEVEL::PL_ERROR;
	while (m_player_level == PLAYER_LEVEL::PL_ERROR)
	{
		std::cout << "My level is:\n";
		std::cout << "1 - human\n";
		std::cout << "2 - dump\n";
		std::cout << "3 - lazy\n";
		std::cout << "4 - normal\n";
		std::cout << "5 - advanced\n>";
		char ch = _getche();
		int l = ch -'0' + 1;
		if (l > 1 and l < 7)
			m_player_level = (PLAYER_LEVEL)l;
	}
	m_player_id = -1;
	IRequestPtr rq = std::make_shared<RegisterRequest>(name, m_player_level);
	if (SendRequest(rq))
	{
		IResponsePtr rs = GetResponse();
		m_player_id = rs->GetPlayerID();
		m_player_name = name;
	}
}

void NavalClient::PlayGame()
{
	bool is_finish = false;
	int winner;
	bool my_turn = StartGame();
	do
	{
		my_turn = PlayRound(my_turn);
		is_finish = IsGameFinished(winner);
	} while (!is_finish);
	FinishGame(winner);
}

bool NavalClient::StartGame()
{
	PLAYER_LEVEL opponent_level = PLAYER_LEVEL::PL_ERROR;
	while (opponent_level == PLAYER_LEVEL::PL_ERROR)
	{
		std::cout << "\nI want to play with:\n";
		std::cout << "0 - newer mind\n";
		std::cout << "1 - human\n";
		std::cout << "2 - dump bot\n";
		std::cout << "3 - lazy bot\n";
		std::cout << "4 - normal bot\n";
		std::cout << "5 - advanced bot\n>";
		char ch = _getche();
		int l = ch - '0' + 1;
		if (l > 0 and l < 7)
			opponent_level = (PLAYER_LEVEL)l;
	}
	std::cout << "\nWaiting for proper player..............\n";
	m_game_id = -1;
	IRequestPtr rq = std::make_shared<StartGameRequest>(m_player_id, opponent_level);
	if (SendRequest(rq))
	{
		IResponsePtr rs = GetResponse();
		m_game_id = rs->GetGameID();
		std::cout << "Proper player is found and Game_" << m_game_id << " is initializing...\n";

	}
	bool is_my_turn = false;
	IRequestPtr rq2 = std::make_shared<GetFieldRequest>(m_player_id, m_game_id);
	if (SendRequest(rq2))
	{
		IResponsePtr rs = GetResponse();
		std::string field = rs->GetField();
		is_my_turn = rs->IsMyTurn();
		IStrategyPtr strategy;
		switch (m_player_level)
		{
		case PLAYER_LEVEL::PL_DUMP:
			strategy = std::make_shared<DumpStrategy>();
			break;
		case PLAYER_LEVEL::PL_LAZY:
			strategy = std::make_shared<LazyStrategy>();
			break;
		case PLAYER_LEVEL::PL_NORMAL:
			strategy = std::make_shared<NormalStrategy>();
			break;
		case PLAYER_LEVEL::PL_ADVANCED:
			strategy = std::make_shared<AdvancedStrategy>();
			break;
		default:
			strategy = std::make_shared<HumanStrategy>();
		}
		std::shared_ptr<Player> player = std::make_shared<Player>(m_player_name, strategy);;
		m_player = player;
		std::istringstream s(field);
		player->GetMyField()->Unpack(s);
	}
	return is_my_turn;
}

bool NavalClient::PlayRound(bool my_turn)
{
	bool res_my_turn = false;
	if (my_turn)
	{
		int ver, hor;
		m_player->MakeShot(ver, hor);
		IRequestPtr rq = std::make_shared<MakeShotRequest>(m_player_id, m_game_id, ver, hor);
		if (SendRequest(rq))
		{
			IResponsePtr rs = GetResponse();
			AttackResult res = rs->GetAttackResult();
			res_my_turn = rs->IsMyTurn();
			auto p = rs->GetShotPos();
			m_player->ResultOfYourShot(p.first, p.second, res);
			std::cout << char('A' + p.first) << p.second << " - ";
			switch (res)
			{
			case AttackResult::AR_SUNK:
				std::cout << "KILLED\n";
				break;
			case AttackResult::AR_INJURED:
				std::cout << "INJURED\n";
				break;
			case AttackResult::AR_MISS:
				std::cout << "MISS\n";
				break;
			case AttackResult::AR_ALREADY_ATTACKED:
				std::cout << "ALREADY ATTACKED\n";
				break;
			default:
				std::cout << "SOMETHING WRONG!!!\n";
			}
		}
	}
	else
	{
		std::cout << "Waiting for opponent shot...\n";
		IResponsePtr rs = GetResponse();
		AttackResult res = rs->GetAttackResult();
		res_my_turn = rs->IsMyTurn();
		auto p = rs->GetShotPos();
		m_player->AttackYou(p.first, p.second);
		std::cout <<"Opponent made shot: " << char('A' + p.first) << p.second << " - ";
		switch (res)
		{
		case AttackResult::AR_SUNK:
			std::cout << "KILLED\n";
			break;
		case AttackResult::AR_INJURED:
			std::cout << "INJURED\n";
			break;
		case AttackResult::AR_MISS:
			std::cout << "MISS\n";
			break;
		case AttackResult::AR_ALREADY_ATTACKED:
			std::cout << "ALREADY ATTACKED\n";
			break;
		default:
			std::cout << "SOMETHING WRONG!!!\n";
		}
	}
	return res_my_turn;
}

bool NavalClient::IsGameFinished(int& winner_id)
{
	IResponsePtr rs = GetResponse();
	auto f = rs->IsFinish();
	winner_id = f.second;
	return f.first;
}

void NavalClient::FinishGame(int winner_id)
{
	std::cout << "Game over! You are " << (winner_id == m_player_id ? "WINNER!!!!" : "looser.");
}
