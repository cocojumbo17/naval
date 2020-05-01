#include "stdafx.h"
#include "Strategy.h"
#include <algorithm>

void DumpStrategy::Init()
{

}

void DumpStrategy::Shot(int& o_ver, int& o_hor)
{
	o_ver = rand() % 10;
	o_hor = rand() % 10;
}

void DumpStrategy::Correct(int ver, int hor, AttackResult res)
{
	//I'm so dump. I won't do anything
}

//---------------------------------------------------------------------
LazyStrategy::LazyStrategy()
{
	Init();
}

void LazyStrategy::Init()
{
	m_random_helper.clear();
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
		{
			m_random_helper.push_back(std::make_pair(i, j));
		}
}

void LazyStrategy::Shot(int& o_ver, int& o_hor)
{
	int r = rand() % m_random_helper.size();
	std::pair<int, int> res = m_random_helper[r];
	m_random_helper.erase(m_random_helper.begin() + r);
	o_ver = res.first;
	o_hor = res.second;
}

void LazyStrategy::Correct(int ver, int hor, AttackResult res)
{
	//I'm still lazy.
}

bool LazyStrategy::IsPosAvailable(int ver, int hor)
{
	std::pair<int, int> pos = std::make_pair(ver, hor);
	return std::find(m_random_helper.begin(), m_random_helper.end(), pos) != m_random_helper.end();
}
//---------------------------------------------------------------------
NormalStrategy::NormalStrategy():LazyStrategy()
{
}
void NormalStrategy::Init()
{
	LazyStrategy::Init();
	m_injured_decks.clear();
}
void NormalStrategy::Shot(int& o_ver, int& o_hor)
{
	LazyStrategy::Shot(o_ver, o_hor);
}

void NormalStrategy::Correct(int ver, int hor, AttackResult res)
{
	if (res == AttackResult::AR_INJURED)
	{
		m_injured_decks.push_back(std::make_pair(ver, hor));
	}
	else if (res == AttackResult::AR_SUNK)
	{
		auto cur_deck = std::make_pair(ver, hor);
		m_injured_decks.push_back(cur_deck);
		FindShip(cur_deck);
		MarkUnavailableCells();
		m_current_ship.clear();
	}
}
void NormalStrategy::FindShip(std::pair<int, int> cur_deck)
{
	auto iter = std::find(m_injured_decks.begin(), m_injured_decks.end(), cur_deck);
	if (iter != m_injured_decks.end())
	{
		m_current_ship.insert(cur_deck);
		m_injured_decks.erase(iter);
		std::pair<int, int> left = std::make_pair(cur_deck.first - 1, cur_deck.second);
		FindShip(left);
		std::pair<int, int> right = std::make_pair(cur_deck.first + 1, cur_deck.second);
		FindShip(right);
		std::pair<int, int> top = std::make_pair(cur_deck.first, cur_deck.second - 1);
		FindShip(top);
		std::pair<int, int> bottom = std::make_pair(cur_deck.first, cur_deck.second + 1);
		FindShip(bottom);
	}
}

void NormalStrategy::MarkUnavailableCells()
{
	int min_ver = 10, max_ver = -1;
	int min_hor = 10, max_hor = -1;
	for (auto p : m_current_ship)
	{
		if (p.first - 1 < min_ver)
			min_ver = std::max(0, p.first - 1);
		if (p.first + 1 > max_ver)
			max_ver = std::min(9, p.first + 1);
		if (p.second - 1 < min_hor)
			min_hor = std::max(0, p.second - 1);
		if (p.second + 1 > max_hor)
			max_hor = std::min(9, p.second + 1);
	}
	for (int v = min_ver; v <= max_ver; ++v)
		for (int h = min_hor; h <= max_hor; ++h)
		{
			auto iter = std::find(m_random_helper.begin(), m_random_helper.end(), std::make_pair(v, h));
			if (iter != m_random_helper.end())
				m_random_helper.erase(iter);
		}
}
//---------------------------------------------------------------------
AdvancedStrategy::AdvancedStrategy()
{
	Init();
}
void AdvancedStrategy::Init()
{
	NormalStrategy::Init();
	m_is_use_next_shot = false;
}
void AdvancedStrategy::Shot(int& o_ver, int& o_hor)
{
	if (m_is_use_next_shot)
	{
		o_ver = m_next_shot.first;
		o_hor = m_next_shot.second;
		auto it = std::find(m_random_helper.begin(), m_random_helper.end(), m_next_shot);
		if (it != m_random_helper.end())
			m_random_helper.erase(it);
	}
	else
		NormalStrategy::Shot(o_ver, o_hor);
}
void AdvancedStrategy::Correct(int ver, int hor, AttackResult res)
{
	NormalStrategy::Correct(ver, hor, res);
	bool find_next_shot = false;
	if (res == AttackResult::AR_INJURED)
	{
		m_previous_good_shot = std::make_pair(ver, hor);
		find_next_shot = true;
	}
	else if (res == AttackResult::AR_SUNK)
		m_is_use_next_shot = false;
	else if (m_is_use_next_shot)
		find_next_shot = true;

	if (find_next_shot)
		m_is_use_next_shot = FindNextShot();
}

bool AdvancedStrategy::FindNextShot()
{
	bool res = false;
	auto stored = m_injured_decks;
	FindShip(m_previous_good_shot);
	m_injured_decks = stored;
	if (m_current_ship.size() == 1)//4 ways
	{
		auto p = *m_current_ship.begin();
		if (IsPosAvailable(p.first - 1, p.second))
		{
			m_next_shot = std::make_pair(p.first - 1, p.second);
			res = true;
		}
		else if (IsPosAvailable(p.first + 1, p.second))
		{
			m_next_shot = std::make_pair(p.first + 1, p.second);
			res = true;
		}
		else if (IsPosAvailable(p.first, p.second - 1))
		{
			m_next_shot = std::make_pair(p.first, p.second - 1);
			res = true;
		}
		else if (IsPosAvailable(p.first, p.second + 1))
		{
			m_next_shot = std::make_pair(p.first, p.second + 1);
			res = true;
		}
	}
	else//2 ways
	{
		auto beg = m_current_ship.begin();
		auto end = m_current_ship.rbegin();
		bool is_vertical = (beg->first - end->first == 0);
		if (is_vertical)
		{
			if (IsPosAvailable(beg->first, beg->second - 1))
			{
				m_next_shot = std::make_pair(beg->first, beg->second - 1);
				res = true;
			}
			else if (IsPosAvailable(end->first, end->second + 1))
			{
				m_next_shot = std::make_pair(end->first, end->second + 1);
				res = true;
			}
		}
		else
		{
			if (IsPosAvailable(beg->first - 1, beg->second))
			{
				m_next_shot = std::make_pair(beg->first - 1, beg->second);
				res = true;
			}
			else if (IsPosAvailable(end->first + 1, end->second))
			{
				m_next_shot = std::make_pair(end->first + 1, end->second);
				res = true;
			}
		}
	}
	m_current_ship.clear();
	return res;
}