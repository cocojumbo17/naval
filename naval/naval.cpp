// naval.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Game.h"
#include "Player.h"
#include "Strategy.h"

int main()
{
	srand((unsigned int)time(NULL));
	const int num_games = 1000;
	int num_win1 = 0, num_win2 = 0;
	int num_rounds = 0;
	std::shared_ptr<IStrategy> strategy1 = std::make_shared<AdvancedStrategy>();
	std::shared_ptr<IStrategy> strategy2 = std::make_shared<LazyStrategy>();
	IPlayerPtr p1 = std::make_shared<Player>("Vasiliy", strategy1);
	IPlayerPtr p2 = std::make_shared<Player>("Vovan", strategy2);

	for (int i = 0; i < num_games; ++i)
	{
		if (i > 0)
		{
			p1->Init();
			p2->Init();
		}
 		Game g(p1, p2);
		std::cout << "LET'S START" << std::endl;
		IPlayerPtr p_winner = g.PlayGame();
		num_rounds += g.GetNumRounds();
		if (p_winner == p1)
			++num_win1;
		else
			++num_win2;
	}
	std::cout << "==========["<< num_win1<<" : " << num_win2 << "] ANR="<< num_rounds/ num_games <<"==========" << std::endl;
	return 0;
};

