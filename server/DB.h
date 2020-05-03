#pragma once
#include <string>
#include "../common/interfaces.h"
#include <map>

struct PlayerItem
{
	PlayerItem()
		: m_player_id(-1), m_player_name(""), m_ready_for_game(false)
		, m_player_level(PLAYER_LEVEL::PL_ERROR), m_opponent_level(PLAYER_LEVEL::PL_ERROR)
		, m_game_id(-1)
	{}
	int				m_player_id;
	std::string		m_player_name;
	bool			m_ready_for_game;
	PLAYER_LEVEL	m_player_level;
	PLAYER_LEVEL	m_opponent_level;
	int				m_game_id;
};

struct GameItem
{
	int				m_game_id;
	int				m_player1_id;
	int				m_player2_id;
	IGamePtr		m_game_object;
};

class DB
{
public:
	DB();
	virtual ~DB();
	int AddPlayer(int client_id, std::string player_name, PLAYER_LEVEL	player_level);
	int AddGame(int player1_id, int player2_id);
	int SelectGame(int player1_id, PLAYER_LEVEL opponent_level);
	bool GetPlayerInfo(int player_id, PlayerItem& info);
	bool GetGameInfo(int game_id, GameItem& info);
	void RemovePlayer(int player_id);
	void RemoveGame(int game_id);
protected:
	void CreateGameObject(int game_id);
protected:
	std::map<int, PlayerItem> m_players;
	std::map<int, GameItem> m_games;
};

typedef std::shared_ptr<DB> DBPtr;