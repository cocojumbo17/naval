#include "RequestHandler.h"
#include <iostream>
#include "../common/ResponseParser.h"

RequestHandler::RequestHandler(DBPtr db)
	:m_db(db)
{
}

RequestHandler::~RequestHandler()
{
}

void RequestHandler::AddRequest(int client_id, IRequestPtr rq)
{
	m_requests.push(std::make_pair(client_id, rq));
}

bool RequestHandler::Answer(int& o_client_id, IResponsePtr& o_response)
{
	if (m_responses.size() > 0)
	{
		auto p = m_responses.front();
		m_responses.pop();
		o_client_id = p.first;
		o_response = p.second;
		return true;
	}
	return false;
}

void RequestHandler::Listen()
{
	if (m_requests.size() > 0)
	{
		auto p = m_requests.front();
		m_requests.pop();
		switch (p.second->GetCommandID())
		{
		case Request_ID::RQ_REGISTER:
			DoRegisterPlayer(p.first, p.second);
			break;
		case Request_ID::RQ_START_GAME:
			DoStartGame(p.first, p.second);
			break;
		case Request_ID::RQ_GET_FIELD:
			DoGetField(p.first, p.second);
			break;
		case Request_ID::RQ_MAKE_SHOT:
			DoMakeShot(p.first, p.second);
			break;
		default:
			std::cerr << "Incorrect request from player_" << p.first << std::endl;
		}
	}
}

void RequestHandler::DoRegisterPlayer(int client_id, IRequestPtr rq)
{
	int player_id = m_db->AddPlayer(client_id, rq->GetPlayerName(), rq->GetLevel());
	IResponsePtr rs = std::make_shared<RegisterResponse>(player_id);
	m_responses.push(std::make_pair(player_id, rs));
	std::cout << "Player " << rq->GetPlayerName() << " is registered as player_"<< player_id << std::endl;
}

void RequestHandler::DoStartGame(int client_id, IRequestPtr rq)
{
	int player_id = rq->GetPlayerID();
	PLAYER_LEVEL opponent_level = rq->GetOpponentLevel();
	int opponent_id = m_db->SelectGame(player_id, opponent_level);
	if (opponent_id != -1)
	{
		int game_id = m_db->AddGame(player_id, opponent_id);
		IResponsePtr rs = std::make_shared<StartGameResponse>(game_id);
		m_responses.push(std::make_pair(opponent_id, rs));
		m_responses.push(std::make_pair(player_id, rs));
		std::cout << "Game_" << game_id << " is started. Player_" << opponent_id << " vs Player_" << player_id << std::endl;
	}
}

void RequestHandler::DoGetField(int client_id, IRequestPtr rq)
{
	int player_id = rq->GetPlayerID();
	int game_id = rq->GetGameID();
	GameItem gi;
	if (!m_db->GetGameInfo(game_id, gi))
	{
		std::cerr << "Game Object for game_" << game_id << " is not created yet." << std::endl;
		return;
	}
	bool my_turn = gi.m_player1_id == player_id; //my turn to make shot

	std::string field = gi.m_game_object->GetField(player_id);

	if (!field.empty())
	{
		IResponsePtr rs = std::make_shared<GetFieldResponse>(field, my_turn);
		m_responses.push(std::make_pair(player_id, rs));
		std::cout << "Field was sent to Player_" << player_id << " and this player make shot "<< (my_turn?"first.":"second.") << std::endl;
	}
}

void RequestHandler::DoMakeShot(int client_id, IRequestPtr rq)
{
	int player_id = rq->GetPlayerID();
	int game_id = rq->GetGameID();
	std::pair<int, int> pos = rq->GetShotPos();
	GameItem gi;
	if (!m_db->GetGameInfo(game_id, gi))
	{
		std::cerr << "Game Object for game_" << game_id << " is not created yet." << std::endl;
		return;
	}
	int defender_id = (gi.m_player1_id == player_id ? gi.m_player2_id : gi.m_player1_id);
	AttackResult attack_res = gi.m_game_object->PlayRound(player_id, defender_id, pos.first, pos.second);
	IResponsePtr rs = std::make_shared<MakeShotResponse>(pos.first, pos.second, attack_res);
	m_responses.push(std::make_pair(player_id, rs));
	IResponsePtr rs2 = std::make_shared<UnderAttackResponse>(pos.first, pos.second, attack_res);
	m_responses.push(std::make_pair(defender_id, rs2));
//	std::cout << "Player_" << player_id << " made shot at" << pos.first << pos.second << std::endl;

	int winner_id, looser_id;
	bool is_finish = gi.m_game_object->IsFinish(winner_id, looser_id);
	IResponsePtr rs3 = std::make_shared<IsFinishResponse>(is_finish, winner_id);
	m_responses.push(std::make_pair(player_id, rs3));
	m_responses.push(std::make_pair(defender_id, rs3));

	if (is_finish)
		std::cout << "Game_" << game_id << " is finished." << pos.first << pos.second << std::endl;
}