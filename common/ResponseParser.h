#pragma once
#include "../common/interfaces.h"


class IResponseImpl :public IResponse
{
public:
	// Inherited via IResponse
	virtual Response_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual int GetGameID() override;
	virtual std::string GetField() override;
	virtual bool IsMyTurn() override;
	virtual AttackResult GetAttackResult() override;
	virtual std::pair<int, int> GetShotPos() override;
	virtual std::string PackToString() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
};

class ErrorResponse :public IResponseImpl
{
public:
	// Inherited via IResponse
	virtual Response_ID GetCommandID() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
};

class RegisterResponse :public IResponseImpl
{
public:
	RegisterResponse();
	RegisterResponse(int player_id);
	// Inherited via IResponse
	virtual Response_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual std::string PackToString() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
protected:
	int m_player_id;
};

class StartGameResponse :public IResponseImpl
{
public:
	StartGameResponse();
	StartGameResponse(int game_id);
	// Inherited via IResponse
	virtual Response_ID GetCommandID() override;
	virtual int GetGameID() override;
	virtual std::string PackToString() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
protected:
	int m_game_id;
};

class GetFieldResponse :public IResponseImpl
{
public:
	GetFieldResponse();
	GetFieldResponse(const std::string& data, bool my_turn);
	// Inherited via IResponse
	virtual Response_ID GetCommandID() override;
	virtual std::string GetField() override;
	virtual bool IsMyTurn() override;
	virtual std::string PackToString() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
protected:
	std::string m_data;
	bool m_is_my_turn;
};

class MakeShotResponse :public IResponseImpl
{
public:
	MakeShotResponse();
	MakeShotResponse(int ver, int hor, AttackResult res);
	// Inherited via IResponse
	virtual Response_ID GetCommandID() override;
	virtual AttackResult GetAttackResult() override;
	virtual std::pair<int, int> GetShotPos() override;
	virtual std::string PackToString() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
protected:
	int m_ver;
	int m_hor;
	AttackResult m_result;

};

class UnderAttackResponse :public MakeShotResponse
{
public:
	UnderAttackResponse();
	UnderAttackResponse(int ver, int hor, AttackResult res);
	// Inherited via IResponse
	virtual Response_ID GetCommandID() override;
	virtual std::string PackToString() override;
};

namespace ResponseParser
{
	IResponsePtr Parse(const std::string& str);
	std::string Pack(IResponsePtr command);
};

