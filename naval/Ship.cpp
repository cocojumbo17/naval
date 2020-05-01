#include "stdafx.h"
#include "Ship.h"
#include <algorithm>
#include <sstream>

Ship::Ship()
{

}
Ship::~Ship()
{

}

void Ship::AddDeck(int ver, int hor)
{
	m_decks.insert(std::make_pair(std::make_pair(ver,hor), true));
}

bool Ship::HitDeck(int ver, int hor)
{
	auto& iter = m_decks.find(std::make_pair(ver, hor));
	if (iter == m_decks.end())
		return false;
	bool old_val = iter->second;
	iter->second = false;
	return old_val;
}

bool Ship::IsDeckAlive(int ver, int hor)
{
	auto& iter = m_decks.find(std::make_pair(ver, hor));
	if (iter == m_decks.end())
		return false;
	return iter->second;
}

int Ship::GetNumAliveDecks()
{
	int num_alive_decks = std::count_if(m_decks.begin(), m_decks.end(), [](auto iter) {return iter.second; });
	return num_alive_decks;
}

int Ship::DeckNumbers()
{
	return (int)m_decks.size();
}

bool Ship::operator<(const Ship& other) const
{
	return m_decks < other.m_decks;
}

void Ship::Pack(std::ostringstream& o_stream) const
{
	o_stream << m_decks.size() << ":";
	for (auto it : m_decks)
	{
		o_stream << it.first.first << "," << it.first.second << ";";
	}
}

void Ship::Unpack(std::istringstream& i_stream)
{
	m_decks.clear();
	int v,h,size = 0;
	char sep;
	i_stream >> size >> sep;
	for (int i = 0; i < size; ++i)
	{
		i_stream >> v >> sep >> h >> sep;
		AddDeck(v,h);
	}
}
/*
bool test1()
{
	CShip sh;
	sh.AddDeck(2, 2);
	sh.AddDeck(2, 3);
	sh.AddDeck(2, 4);
	sh.AddDeck(2, 5);
	std::string str = sh.Pack();
	return sh.DeckNumbers() == 4;
}
bool test2()
{
	std::string str("4:2,2;2,3;2,4;2,5;");
	CShip sh;
	sh.UnPack(str);
	return sh.DeckNumbers() == 4;
}
bool g_t = (test1() && test2());
*/
