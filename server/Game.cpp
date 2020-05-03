#include "Game.h"
#include <iostream>
#include "ProxyPlayer.h"

Game::Game(int player1_id, int player2_id)
	: m_rounds(0)
{
	PlayerData data1;
	data1.m_left_decks = 20;
	data1.mp_player = std::make_shared<ProxyPlayer>();
	m_players.insert(std::make_pair(player1_id, data1));

	PlayerData data2;
	data2.m_left_decks = 20;
	data2.mp_player = std::make_shared<ProxyPlayer>();
	m_players.insert(std::make_pair(player2_id, data2));
}

Game::~Game()
{
}

std::string Game::GetField(int player_id)
{
	if (m_players.find(player_id) == m_players.end())
		return "";
	return m_players[player_id].mp_player->GetField();
}

AttackResult Game::PlayRound(int attacker_id, int deffender_id, int ver, int hor)
{
	++m_rounds;
	if (m_players.find(deffender_id) == m_players.end())
		return AttackResult::AR_ERROR;

	AttackResult res = m_players[deffender_id].mp_player->AttackYou(ver, hor);
	if (res == AttackResult::AR_INJURED || res == AttackResult::AR_SUNK)
		m_players[deffender_id].m_left_decks -= 1;
	return res;
}

bool Game::IsFinish(int& o_winner_id, int& o_looser_id)
{
	auto it1 = m_players.cbegin();
	auto it2 = it1;
	++it2;
	bool res = false;
	if (it1->second.m_left_decks == 0)
	{
		res = true;
		o_looser_id = it1->first;
		o_winner_id = it2->first;
	}
	else if (it2->second.m_left_decks == 0)
	{
		res = true;
		o_looser_id = it2->first;
		o_winner_id = it1->first;
	}
	return res;
}

void Game::GetStats(int& o_round_played, int& o_decks_left)
{
	o_round_played = m_rounds;
	auto it1 = m_players.cbegin();
	auto it2 = it1;
	++it2;
	if (it1->second.m_left_decks == 0)
		o_decks_left = it2->second.m_left_decks;
	else
		o_decks_left = it1->second.m_left_decks;
}
