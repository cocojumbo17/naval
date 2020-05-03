#pragma once
#include "..\common\interfaces.h"
#include <vector>
#include <string>
#include "Ship.h"


enum CellState {
	CS_EMPTY = 1,
	CS_SHIP = 2,
	CS_ATTACKED = 4,
	CS_UNAVAILABLE = 8
};

struct CellData
{
	CellData():m_cell_state(CellState::CS_EMPTY), m_cell_ship_id(-1){}
	CellData(CellState cs) :m_cell_state(cs), m_cell_ship_id(-1) {}
	CellData(CellState cs, int ship_id) :m_cell_state(cs), m_cell_ship_id(ship_id) {}
	int			m_cell_state;
	int			m_cell_ship_id;
	bool operator < (const CellData& other) {
		if (m_cell_state == other.m_cell_state)
			return m_cell_ship_id < other.m_cell_ship_id;
	}
};

class Field : public ISerialize
{
public:
	Field();
	virtual ~Field();
	void Clear();
	CellState GetCellState(int ver, int hor);
	void Attack(int ver, int hor);
	bool IsMortalAttack(int ver, int hor);
//	CellState GetCellState(char ver, int hor);
	std::string ToString(bool for_me);
	void Generate();
	//ISerialize
	void Pack(std::ostringstream& o_stream) const override;
	void Unpack(std::istringstream& i_stream) override;
protected:
	bool IsPossibleToSet(int ver, int hor);
	bool FindPlaceForDeck(int& ver, int& hor);

	bool PutShip(int decks);
	bool PutShip1();
	bool PutShip2();
	bool PutShip3();
	bool PutShip4();

	void RndInit();
	int Rnd();
	void PutDeck(int ver, int hor, int ship_id);

	std::string AsMyEnemyField();
	std::string AsMyField();

protected:
	std::vector<CellData> m_cells;
	std::vector<Ship> m_ship;
private:
	std::vector<int> m_random_helper;
};

typedef std::shared_ptr<Field> FieldPtr;
