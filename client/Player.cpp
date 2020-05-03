#include "Player.h"
#include <iostream>
#include <sstream>

Player::Player(const std::string& name, IStrategyPtr p_strategy)
	:m_name(name)
	, mp_strategy(p_strategy)
{
	m_my_field = std::make_shared<Field>();
	m_opponent_field = std::make_shared<Field>();
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	mp_strategy->Init();
	m_my_field->Generate();
	m_opponent_field->Clear();
}

FieldPtr Player::GetMyField()
{
	return m_my_field;
}
std::string Player::GetField()
{
	std::ostringstream s;
	m_my_field->Pack(s);
	return s.str();
}
void Player::MakeShot(int& o_ver, int& o_hor)
{
	system("cls");
	std::cout << "My field\n";
	std::cout << m_my_field->ToString();
	std::cout << "Opponent field\n";
	std::cout << m_opponent_field->ToString();
	mp_strategy->Shot(o_ver, o_hor);
//	char letter = 'A' + o_ver;
//	std::cout << "[" << m_name << "]>" << letter << o_hor << "?" << std::endl;
}

void Player::ResultOfYourShot(int i_ver, int i_hor, AttackResult res)
{
	/*
	std::cout << "[" << m_name << "]>";
	switch (res)
	{
	case AttackResult::AR_MISS:
		std::cout << "Ooo nooooo!";
		break;
	case AttackResult::AR_ALREADY_ATTACKED:
		std::cout << "I'm so stupid";
		break;
	case AttackResult::AR_INJURED:
		std::cout << "Yessss!";
		break;
	case AttackResult::AR_SUNK:
		std::cout << "Ha-ha-ha, minus 1!!!";
		break;
	}
	std::cout << std::endl;
	*/
	if (res == AttackResult::AR_INJURED || res == AttackResult::AR_SUNK)
		m_opponent_field->AttackToDeck(i_ver, i_hor);
	else
		m_opponent_field->Attack(i_ver, i_hor);
	mp_strategy->Correct(i_ver, i_hor, res);
}

AttackResult Player::AttackYou(int i_ver, int i_hor)
{
//	std::cout << "[" << m_name << "]>";
	AttackResult res;
	CellState cs = m_my_field->GetCellState(i_ver, i_hor);
	if (cs & CellState::CS_ATTACKED)
	{
		res = AttackResult::AR_ALREADY_ATTACKED;
//		std::cout << "You have already shut there.";
	}
	else if (cs & CellState::CS_EMPTY)
	{
		res = AttackResult::AR_MISS;
//		std::cout << "MISS";
	}
	else if (cs & CellState::CS_SHIP)
	{
		if (m_my_field->IsMortalAttack(i_ver, i_hor))
		{
			res = AttackResult::AR_SUNK;
//			std::cout << "KILLED";
		}
		else
		{
			res = AttackResult::AR_INJURED;
//			std::cout << "INJURED";
		}
	}
	else
		res = AttackResult::AR_ERROR;
	m_my_field->Attack(i_ver, i_hor);
	std::cout << std::endl;
	return res;
}

void Player::Finish(bool is_victory)
{
//	std::cout << "[" << m_name <<"]>" << (is_victory ? "Huraaa!!!" : "I'm dead............") << std::endl;
}
