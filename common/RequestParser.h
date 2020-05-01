#pragma once
#include <string>
#include <memory>
#include "../common/tinyxml2.h"

enum class REQUEST_ID
{
	RQ_ERROR = 0,
	RQ_REGISTER,
	RQ_START_GAME,
	RQ_GET_FIELD,
	RQ_MAKE_SHOT
};

enum class PLAYER_LEVEL 
{
	PL_ERROR = 0,
	PL_HUMAN,
	PL_DUMP,
	PL_LAZY,
	PL_NORMAL,
	PL_ADVANCED
};

class IRequest
{
public:
	~IRequest() {};
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) = 0;
	virtual REQUEST_ID GetCommandID() = 0;
	virtual std::string GetPlayerName() = 0;
	virtual int GetPlayerID() = 0;
	virtual int GetGameID() = 0;
	virtual std::string PackToString() = 0;
	virtual PLAYER_LEVEL GetLevel() = 0;
	virtual PLAYER_LEVEL GetOpponentLevel() = 0;
	virtual std::pair<int, int> GetShotPos() = 0;
};
typedef std::shared_ptr<IRequest> IRequestPtr;

class IRequestImpl :public IRequest
{
public:
	// Inherited via IRequest
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
	virtual REQUEST_ID GetCommandID() override;
	virtual std::string GetPlayerName() override;
	virtual int GetPlayerID() override;
	virtual int GetGameID() override;
	virtual std::string PackToString() override;
	virtual PLAYER_LEVEL GetLevel() override;
	virtual PLAYER_LEVEL GetOpponentLevel() override;
	virtual std::pair<int, int> GetShotPos() override;
};

class ErrorRequest :public IRequestImpl
{
public:
	// Inherited via IRequest
	virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) override;
	virtual REQUEST_ID GetCommandID() override;
};

class RegisterRequest :public IRequestImpl
{
public:
	RegisterRequest();
	RegisterRequest(const std::string& player, PLAYER_LEVEL my_level);
	// Inherited via IRequest
	virtual REQUEST_ID GetCommandID() override;
	virtual std::string GetPlayerName() override;
	virtual PLAYER_LEVEL GetLevel() override;
	virtual std::string PackToString() override;
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
	virtual REQUEST_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual std::string PackToString() override;
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
	virtual REQUEST_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual int GetGameID() override;
	virtual std::string PackToString() override;
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
	virtual REQUEST_ID GetCommandID() override;
	virtual int GetPlayerID() override;
	virtual int GetGameID() override;
	virtual std::string PackToString() override;
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

