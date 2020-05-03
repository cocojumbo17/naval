#include "Field.h"
#include "string.h"
#include <sstream>
#include <iomanip>
#include "Ship.h"

Field::Field()
{
	Clear();
}


Field::~Field()
{
}
/*
//ver is a,b,c,d,e,f,g,h,i,j
//hor is 1,2,3,4,5,6,7,8,9,10
CellState Field::GetCellState(char ver, int hor)
{
	int v = ver - 'a';
	int h = hor - 1;
	if ((v < 0) || (v > 9) || (h < 0) || (h > 9))
		return CellState::Error;
	else
		return GetCellState(v, h);
}
*/
CellState Field::GetCellState(int ver, int hor)
{
	return (CellState)m_cells[hor * 10 + ver].m_cell_state;
}

void Field::Attack(int ver, int hor)
{
	CellData& cd = m_cells[hor * 10 + ver];
	cd.m_cell_state |= CellState::CS_ATTACKED;
	if (cd.m_cell_ship_id != -1)
		m_ships[cd.m_cell_ship_id].HitDeck(ver, hor);
}

void Field::AttackToDeck(int ver, int hor)
{
	CellData& cd = m_cells[hor * 10 + ver];
	cd.m_cell_state = CellState::CS_SHIP|CellState::CS_ATTACKED;
}

bool Field::IsMortalAttack(int ver, int hor)
{
	CellData cd = m_cells[hor * 10 + ver];
	if (cd.m_cell_state & CellState::CS_SHIP)
	{
		Ship& sh = m_ships[cd.m_cell_ship_id];
		if ((sh.GetNumAliveDecks() == 1) && sh.IsDeckAlive(ver, hor))
			return true;
	}
	return false;
}

bool Field::IsPossibleToSet(int ver, int hor)
{
	if ((ver < 0) || (ver > 9) || (hor < 0) || (hor > 9))
		return false;
	if (GetCellState(ver, hor) == CellState::CS_SHIP)
		return false;
	for (int v = -1; v < 2; ++v)
	{
		int check_v = ver + v;
		if ((check_v < 0) || (check_v > 9))
			continue;
		for (int h = -1; h < 2; ++h)
		{
			int check_h = hor + h;
			if ((check_h < 0) || (check_h > 9))
				continue;
			if ((check_h == hor) && (check_v == ver))
				continue;
			if (GetCellState(check_v, check_h) == CellState::CS_SHIP)
				return false;
		}
	}
	return true;
}

std::string Field::ToString()
{
	std::ostringstream s;
	s << "  |ABCDEFGHIJ|" << std::endl;
	s << "--|----------|" << std::endl;
	for (int h = 0; h < 10; ++h)
	{
		s << std::setw(2) << h << "|";
		for (int v = 0; v < 10; ++v)
		{
			CellState state = GetCellState(v, h);
			if (state & CellState::CS_ATTACKED)
			{
				if (state & CellState::CS_EMPTY)
					s << "*";
				else
					s << "X";
			}
			else if (state & CellState::CS_SHIP)
				s << "#";
			else
				s << " ";
		}
		s << "|" << std::endl;
	}
	s << "--------------" << std::endl;
	return s.str();
}

void Field::RndInit()
{
	m_random_helper.clear();
	for (int i = 0; i < 100; ++i)
	{
		if ((m_cells[i].m_cell_state & CellState::CS_SHIP) == 0 )
			m_random_helper.push_back(i);
	}
}

int Field::Rnd()
{
	int r = rand() % m_random_helper.size();
	return m_random_helper[r];
}

void Field::Clear()
{
	m_cells.clear();
	m_cells.assign(100, CellData());
	m_ships.clear();
}

void Field::Generate()
{
	Clear();
	RndInit();
	PutShip(4);
	PutShip(3);
	PutShip(2);
	PutShip(1);
}

void Field::PutDeck(int ver, int hor, int ship_id)
{
	int pos = hor * 10 + ver;
	m_cells[pos] = CellData(CellState::CS_SHIP, ship_id);
	auto iter = std::find(m_random_helper.begin(), m_random_helper.end(), pos);
	if (iter != m_random_helper.end())
		m_random_helper.erase(iter);
}

bool Field::FindPlaceForDeck(int& ver, int& hor)
{
	bool found = false;
	int protector = 1000;
	do {
		--protector;
		int pos = Rnd();
		hor = pos / 10;
		ver = pos % 10;
		if (IsPossibleToSet(ver, hor))
			found = true;
	} while (!found && (protector > 0));
	return found;
}

bool Field::PutShip1()
{
	int v, h;
	if (FindPlaceForDeck(v, h))
	{
		PutDeck(v, h, m_ships.size());
		Ship s;
		s.AddDeck(v, h);
		m_ships.push_back(s);
		return true;
	}
	return false;
}

bool Field::PutShip2()
{
	int v, h;
	bool is_found = false;
	int protector = 1000;
	while (!is_found && (protector > 0))
	{
		--protector;
		if (FindPlaceForDeck(v, h))
		{
			bool is_vertically = (rand() % 2 == 0);
			if (is_vertically)
			{
				if (IsPossibleToSet(v, h + 1))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v, h + 1, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v, h + 1);
					m_ships.push_back(s);
					is_found = true;
				}
				else if (IsPossibleToSet(v, h - 1))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v, h - 1, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v, h - 1);
					m_ships.push_back(s);
					is_found = true;
				}
			}
			else
			{
				if (IsPossibleToSet(v + 1, h))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v + 1, h, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v + 1, h);
					m_ships.push_back(s);
					is_found = true;
				}
				else if (IsPossibleToSet(v - 1, h))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v - 1, h, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v - 1, h);
					m_ships.push_back(s);
					is_found = true;
				}
			}
		}
	}
	return is_found;
}

bool Field::PutShip3()
{
	int v, h;
	bool is_found = false;
	int protector = 1000;
	while (!is_found && (protector > 0))
	{
		--protector;
		if (FindPlaceForDeck(v, h))
		{
			bool is_vertically = (rand() % 2 == 0);
			if (is_vertically)
			{
				if (IsPossibleToSet(v, h + 1) &&
					IsPossibleToSet(v, h + 2))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v, h + 1, m_ships.size());
					PutDeck(v, h + 2, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v, h + 1);
					s.AddDeck(v, h + 2);
					m_ships.push_back(s);
					is_found = true;
				}
				else if (IsPossibleToSet(v, h - 1)&&
					IsPossibleToSet(v, h - 2))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v, h - 1, m_ships.size());
					PutDeck(v, h - 2, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v, h - 1);
					s.AddDeck(v, h - 2);
					m_ships.push_back(s);
					is_found = true;
				}
			}
			else
			{
				if (IsPossibleToSet(v + 1, h) &&
					IsPossibleToSet(v + 2, h))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v + 1, h, m_ships.size());
					PutDeck(v + 2, h, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v + 1, h);
					s.AddDeck(v + 2, h);
					m_ships.push_back(s);
					is_found = true;
				}
				else if (IsPossibleToSet(v - 1, h) &&
					IsPossibleToSet(v - 2, h))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v - 1, h, m_ships.size());
					PutDeck(v - 2, h, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v - 1, h);
					s.AddDeck(v - 2, h);
					m_ships.push_back(s);
					is_found = true;
				}
			}
		}
	}
	return is_found;
}

bool Field::PutShip4()
{
	int v, h;
	bool is_found = false;
	int protector = 1000;
	while (!is_found && (protector > 0))
	{
		--protector;
		if (FindPlaceForDeck(v, h))
		{
			bool is_vertically = (rand() % 2 == 0);
			if (is_vertically)
			{
				if (IsPossibleToSet(v, h + 1) &&
					IsPossibleToSet(v, h + 2) &&
					IsPossibleToSet(v, h + 3))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v, h + 1, m_ships.size());
					PutDeck(v, h + 2, m_ships.size());
					PutDeck(v, h + 3, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v, h + 1);
					s.AddDeck(v, h + 2);
					s.AddDeck(v, h + 3);
					m_ships.push_back(s);
					is_found = true;
				}
				else if (IsPossibleToSet(v, h - 1) &&
					IsPossibleToSet(v, h - 2) &&
					IsPossibleToSet(v, h - 3))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v, h - 1, m_ships.size());
					PutDeck(v, h - 2, m_ships.size());
					PutDeck(v, h - 3, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v, h - 1);
					s.AddDeck(v, h - 2);
					s.AddDeck(v, h - 3);
					m_ships.push_back(s);
					is_found = true;
				}
			}
			else
			{
				if (IsPossibleToSet(v + 1, h) &&
					IsPossibleToSet(v + 2, h) &&
					IsPossibleToSet(v + 3, h))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v + 1, h, m_ships.size());
					PutDeck(v + 2, h, m_ships.size());
					PutDeck(v + 3, h, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v + 1, h);
					s.AddDeck(v + 2, h);
					s.AddDeck(v + 3, h);
					m_ships.push_back(s);
					is_found = true;
				}
				else if (IsPossibleToSet(v - 1, h) &&
					IsPossibleToSet(v - 2, h) &&
					IsPossibleToSet(v - 3, h))
				{
					PutDeck(v, h, m_ships.size());
					PutDeck(v - 1, h, m_ships.size());
					PutDeck(v - 2, h, m_ships.size());
					PutDeck(v - 3, h, m_ships.size());
					Ship s;
					s.AddDeck(v, h);
					s.AddDeck(v - 1, h);
					s.AddDeck(v - 2, h);
					s.AddDeck(v - 3, h);
					m_ships.push_back(s);
					is_found = true;
				}
			}
		}
	}
	return is_found;
}

bool Field::PutShip(int decks)
{
	if (decks == 1)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (!PutShip1())
				return false;
		}
	}
	else if (decks == 2)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (!PutShip2())
				return false;
		}
	}
	else if (decks == 3)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (!PutShip3())
				return false;
		}
	}
	else if (decks == 4)
	{
		if (!PutShip4())
			return false;
	}
	return true;
}

void Field::Pack(std::ostringstream& o_stream) const
{
	o_stream << m_ships.size() << ":";
	for (auto it : m_ships)
	{
		it.Pack(o_stream);
		o_stream << "|";
	}
}

void Field::Unpack(std::istringstream& i_stream)
{
	m_ships.clear();
	m_cells.clear();
	m_cells.assign(100, CellData());
	int size = 0;
	char sep;
	i_stream >> size >> sep;
	for (int i = 0; i < size; ++i)
	{
		Ship ship;
		ship.Unpack(i_stream);
		i_stream >> sep;
		m_ships.push_back(ship);
	}
	ShipsToCells();
}

void Field::ShipsToCells()
{
	int cur_ship_index = 0;
	for (auto s : m_ships)
	{
		for (auto d : s.m_decks)
		{
			int ver = d.first.first;
			int hor = d.first.second;
			m_cells[hor * 10 + ver] = CellData(CellState::CS_SHIP, cur_ship_index);			
		}
		++cur_ship_index;
	}
}