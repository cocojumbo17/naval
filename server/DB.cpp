#include "DB.h"
#include "ProxyPlayer.h"
#include "Game.h"
#include <iostream>

DB::DB()
{

}

DB::~DB()
{

}

int DB::AddPlayer(int client_id, std::string player_name, PLAYER_LEVEL	player_level)
{
	PlayerItem item;
	item.m_player_id = client_id;
	item.m_player_level = player_level;
	item.m_player_name = player_name;
	m_players.insert(std::make_pair(client_id, item));
	return client_id;
}

int DB::AddGame(int player1_id, int player2_id)
{
	auto it1 = m_players.find(player1_id);
	auto it2 = m_players.find(player2_id);
	if (it1 == m_players.end() || it2 == m_players.end())
		return -1;

	int game_id = m_games.size();
	it1->second.m_game_id = game_id;
	it1->second.m_ready_for_game = false;
	it2->second.m_game_id = game_id;
	it2->second.m_ready_for_game = false;

	GameItem item;
	item.m_game_id = game_id;
	item.m_player1_id = player1_id;
	item.m_player2_id = player2_id;
	m_games.insert(std::make_pair(game_id, item));
	CreateGameObject(game_id);
	return game_id;
}

void DB::RemovePlayer(int player_id)
{
	auto it = m_players.find(player_id);
	if (it != m_players.end())
	{
		int game_id = it->second.m_game_id;
		RemoveGame(game_id);
		m_players.erase(player_id);
	}
}

void DB::RemoveGame(int game_id)
{
	if (m_games.find(game_id) != m_games.end())
		m_games.erase(game_id);
}
	
int DB::SelectGame(int player1_id, PLAYER_LEVEL opponent_level)
{
	if (m_players.find(player1_id) == m_players.end())
		return -1;

	int res = -1;
	m_players[player1_id].m_ready_for_game = true;
	for (auto it : m_players)
	{
		if (it.first != player1_id && it.second.m_ready_for_game && 
			(opponent_level == PLAYER_LEVEL::PL_ANY || it.second.m_player_level == opponent_level))
		{
			res = it.first;
			break;
		}
	}
	return res;
}

void DB::CreateGameObject(int game_id)
{
	GameItem& pi = m_games[game_id];
	IGamePtr p_game = std::make_shared<Game>(pi.m_player1_id, pi.m_player2_id);
	pi.m_game_object = p_game;
}

bool DB::GetPlayerInfo(int player_id, PlayerItem& info)
{
	auto it = m_players.find(player_id);
	if (it == m_players.end())
		return false;
	info = it->second;
	return true;
}

bool DB::GetGameInfo(int game_id, GameItem& info)
{
	auto it = m_games.find(game_id);
	if (it == m_games.end())
		return false;
	info = it->second;
	return true;
}
