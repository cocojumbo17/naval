#include "stdafx.h"
#include "Game.h"
#include <iostream>

Game::Game(IPlayerPtr p_player1, IPlayerPtr p_player2)
	: mp_player1(p_player1)
	, mp_player2(p_player2)
	, m_rounds(0)
{
}

Game::~Game()
{
}
int Game::GetNumRounds()
{
	return m_rounds;
}

IPlayerPtr Game::PlayGame()
{
	IPlayerPtr attacker = mp_player1;
	IPlayerPtr deffender = mp_player2;
	int m_attacker_decks = 20;
	int m_deffender_decks = 20;
	m_rounds = 0;
	while (m_deffender_decks > 0)
	{
		AttackResult res = PlayRound(++m_rounds, attacker, deffender);
		switch (res)
		{
			case AttackResult::AR_MISS:
			case AttackResult::AR_ALREADY_ATTACKED:
				std::swap(attacker, deffender);
				std::swap(m_attacker_decks, m_deffender_decks);
				break;
			case AttackResult::AR_INJURED:
			case AttackResult::AR_SUNK:
				--m_deffender_decks;
				break;
		}
	}
	PlayFinish(attacker, deffender, m_attacker_decks);
	return attacker;
}

AttackResult Game::PlayRound(int round, IPlayerPtr p_attacker, IPlayerPtr p_deffender)
{
	std::cout << "Round #" << round << std::endl;
	int v, h;
	p_attacker->MakeShot(v, h);
	AttackResult res = p_deffender->AttackYou(v, h);
	p_attacker->ResultOfYourShot(v, h, res);
	return res;
}

void Game::PlayFinish(IPlayerPtr p_winner, IPlayerPtr p_looser, int decks_left)
{
	p_winner->Finish(true);
	p_looser->Finish(false);
	std::cout << "-------------------20:"<< 20-decks_left << "----------------------" << std::endl;
}
