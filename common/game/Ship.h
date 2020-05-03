#pragma once
#include <map>
#include "..\interfaces.h"

class Ship: public ISerialize
{
	friend class Field;
public:
	Ship();
	virtual ~Ship();
	void AddDeck(int ver, int hor);
	bool HitDeck(int ver, int hor);
	bool IsDeckAlive(int ver, int hor);
	int GetNumAliveDecks();
	int DeckNumbers();
	bool operator < (const Ship& other) const;
	
	//ISerialize
	void Pack(std::ostringstream& o_stream) const override;
	void Unpack(std::istringstream& i_stream) override;
protected:
	std::map<std::pair<int, int>, bool> m_decks;
};

