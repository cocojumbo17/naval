#pragma once
#include "..\common\interfaces.h"
#include <map>

class Game : public IServerGame
{
	struct PlayerData
	{
		IPlayerPtr mp_player;
		int m_left_decks;
	};

public:
	Game(int player1_id, int player2_id);
	virtual ~Game();
	//IServerGame
	virtual std::string GetField(int player_id) override;
	virtual AttackResult PlayRound(int attacker_id, int deffender_id, int ver, int hor) override;
	virtual bool IsFinish(int& o_winner_id, int& o_looser_id) override;
	virtual void GetStats(int& o_round_played, int& o_decks_left) override;
protected:
	std::map<int, PlayerData> m_players;
	int m_rounds;
};
