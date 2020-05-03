#pragma once
#include "..\interfaces.h"
#include <vector>
#include <set>
//----------------------------------------------------------------------
//don't remember cells which already were under attack
class DumpStrategy :
	public IStrategy
{
public:
	// Inherited via IStrategy
	virtual void Init() override;
	virtual void Shot(int& o_ver, int& o_hor) override;
	virtual void Correct(int ver, int hor, AttackResult res) override;
};
//----------------------------------------------------------------------
//don't shot into already attacked cell
class LazyStrategy :
	public IStrategy
{
public:
	LazyStrategy();
	// Inherited via IStrategy
	virtual void Init() override;
	virtual void Shot(int& o_ver, int& o_hor) override;
	virtual void Correct(int ver, int hor, AttackResult res) override;

	bool IsPosAvailable(int ver, int hor);
protected:
	std::vector<std::pair<int, int>> m_random_helper;
};
//----------------------------------------------------------------------
//Mark cells around sunk ship as unavailable
class NormalStrategy :
	public LazyStrategy
{
public:
	NormalStrategy();
	// Inherited via IStrategy
	virtual void Init() override;
	virtual void Shot(int& o_ver, int& o_hor) override;
	virtual void Correct(int ver, int hor, AttackResult res) override;
protected:
	void MarkUnavailableCells();
	void FindShip(std::pair<int, int> cur_deck);
protected:
	std::vector<std::pair<int, int>> m_injured_decks;
	std::set<std::pair<int, int>> m_current_ship;
};
//----------------------------------------------------------------------
//Try to kill injured ship
class AdvancedStrategy :
	public NormalStrategy
{
public:
	AdvancedStrategy();
	// Inherited via IStrategy
	virtual void Init() override;
	virtual void Shot(int& o_ver, int& o_hor) override;
	virtual void Correct(int ver, int hor, AttackResult res) override;
protected:
	bool FindNextShot();
protected:
	bool m_is_use_next_shot;
	std::pair<int, int> m_next_shot;
	std::pair<int, int> m_previous_good_shot;
};
//----------------------------------------------------------------------
//Human
class HumanStrategy :
	public IStrategy
{
public:
	HumanStrategy();
	// Inherited via IStrategy
	virtual void Init() override;
	virtual void Shot(int& o_ver, int& o_hor) override;
	virtual void Correct(int ver, int hor, AttackResult res) override;
};
