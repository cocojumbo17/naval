#include "ProxyPlayer.h"
#include <iostream>
#include <sstream>

ProxyPlayer::ProxyPlayer()
{
	m_my_field = std::make_shared<Field>();
	Init();
}
ProxyPlayer::~ProxyPlayer()
{

}
FieldPtr ProxyPlayer::GetMyField()
{
	return m_my_field;
}
void ProxyPlayer::Init()
{
	m_my_field->Generate();
}
std::string ProxyPlayer::GetField()
{
	std::ostringstream s;
	m_my_field->Pack(s);
	return s.str();
}
void ProxyPlayer::MakeShot(int& o_ver, int& o_hor)
{
	//do nothing on server side
}
void ProxyPlayer::ResultOfYourShot(int i_ver, int i_hor, AttackResult res)
{
	//do nothing on server side
}
AttackResult ProxyPlayer::AttackYou(int i_ver, int i_hor)
{
	AttackResult res;
	CellState cs = m_my_field->GetCellState(i_ver, i_hor);
	if (cs & CellState::CS_ATTACKED)
	{
		res = AttackResult::AR_ALREADY_ATTACKED;
	}
	else if (cs & CellState::CS_EMPTY)
	{
		res = AttackResult::AR_MISS;
	}
	else if (cs & CellState::CS_SHIP)
	{
		if (m_my_field->IsMortalAttack(i_ver, i_hor))
		{
			res = AttackResult::AR_SUNK;
		}
		else
		{
			res = AttackResult::AR_INJURED;
		}
	}
	else
		res = AttackResult::AR_ERROR;
	m_my_field->Attack(i_ver, i_hor);
	return res;
}
void ProxyPlayer::Finish(bool is_victory)
{
	//do nothing on server side
}
