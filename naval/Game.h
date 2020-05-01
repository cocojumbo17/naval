#pragma once
#include "..\common\interfaces.h"


class Game
{
public:
	Game(IPlayerPtr p_player1, IPlayerPtr p_player2);
	virtual ~Game();
	IPlayerPtr PlayGame();
	AttackResult PlayRound(int round, IPlayerPtr p_attacker, IPlayerPtr p_deffender);
	void PlayFinish(IPlayerPtr p_winner, IPlayerPtr p_looser, int decks_left);
	int GetNumRounds();
protected:
	IPlayerPtr mp_player1;
	IPlayerPtr mp_player2;
	int m_rounds;
};

