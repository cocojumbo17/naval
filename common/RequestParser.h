#pragma once
#include "../common/interfaces.h"



class IRequestImpl :public IRequest
{
public:
	// Inherited via IRequest
	virtual Request_ID GetCommandID() override;
	virtual std::string GetPlayerName() override;
	virtual int GetPlayerID() override;
	virtual int GetGameID() override;
	virtual PLAYER_LEVEL GetLevel() override;
	virtual PLAYER_LEVEL GetOpponentLevel() override;
	virtual std::pair<int, int> GetShotPos() override;
	virtual std::string PackToXML() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
};

class ErrorRequest :public IRequestImpl
{
public:
	// Inherited via IRequest
	virtual Request_ID GetCommandID() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
};

class RegisterRequest :public IRequestImpl
{
public:
	RegisterRequest();
	RegisterRequest(const std::string& player, PLAYER_LEVEL my_level);
	// Inherited via IRequest
	virtual Request_ID GetCommandID() override;
	virtual std::string GetPlayerName() override;
	virtual PLAYER_LEVEL GetLevel() override;
	virtual std::string PackToXML() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
protected:
	std::string m_player;
	PLAYER_LEVEL m_my_level;
};

class StartGameRequest :public IRequestImpl
{
public:
	StartGameRequest();
	StartGameRequest(int player_id, PLAYER_LEVEL opponent_level);
	// Inherited via IRequest
	virtual Request_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual std::string PackToXML() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
	virtual PLAYER_LEVEL GetOpponentLevel() override;
protected:
	int m_player_id;
	PLAYER_LEVEL m_opponent_level;
};

class GetFieldRequest :public IRequestImpl
{
public:
	GetFieldRequest();
	GetFieldRequest(int player_id, int game_id);
	// Inherited via IRequest
	virtual Request_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual int GetGameID() override;
	virtual std::string PackToXML() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
protected:
	int m_player_id;
	int m_game_id;
};

class MakeShotRequest :public IRequestImpl
{
public:
	MakeShotRequest();
	MakeShotRequest(int player_id, int game_id, int ver, int hor);
	// Inherited via IRequest
	virtual Request_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual int GetGameID() override;
	virtual std::string PackToXML() override;
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
	virtual std::pair<int, int> GetShotPos() override;
protected:
	int m_player_id;
	int m_game_id;
	int m_ver;
	int m_hor;

};

namespace RequestParser
{
	IRequestPtr Parse(const std::string& str);
	std::string Pack(IRequestPtr command);
};

