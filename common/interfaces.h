#pragma once
#include <string>
#include <memory>
#include "tinyxml2.h"

enum class Request_ID
{
    RQ_ERROR = 0,
    RQ_REGISTER,
    RQ_START_GAME,
    RQ_GET_FIELD,
    RQ_MAKE_SHOT
};

enum class Response_ID
{
    RS_ERROR = 0,
    RS_REGISTER,
    RS_START_GAME,
    RS_GET_FIELD,
    RS_MAKE_SHOT,
    RS_UNDER_ATTACK,
    RS_IS_FINISH
};

enum class PLAYER_LEVEL
{
    PL_ERROR = 0,
    PL_ANY,
    PL_HUMAN,
    PL_DUMP,
    PL_LAZY,
    PL_NORMAL,
    PL_ADVANCED
};

enum class AttackResult
{
    AR_MISS = 0,
    AR_INJURED,
    AR_SUNK,
    AR_ALREADY_ATTACKED,
    AR_ERROR
};

class IStrategy
{
public:
    virtual ~IStrategy() {};
    virtual void Init() = 0;
    virtual void Shot(int& o_ver, int& o_hor) = 0;
    virtual void Correct(int ver, int hor, AttackResult res) = 0;
};

class IPlayer
{
public:
    virtual ~IPlayer() {};
    virtual void Init() = 0;
    virtual std::string GetField() = 0;
    virtual void MakeShot(int& o_ver, int& o_hor) = 0;
    virtual void ResultOfYourShot(int i_ver, int i_hor, AttackResult res) = 0;
    virtual AttackResult AttackYou(int i_ver, int i_hor) = 0;
    virtual void Finish(bool is_victory) = 0;
};

class ISerialize
{
public:
    virtual ~ISerialize() {};
    virtual void Pack(std::ostringstream& o_stream) const = 0;
    virtual void Unpack(std::istringstream& i_stream) = 0;
};

class IRequest
{
public:
    ~IRequest() {};
    virtual Request_ID GetCommandID() = 0;
    virtual std::string GetPlayerName() = 0;
    virtual int GetPlayerID() = 0;
    virtual int GetGameID() = 0;
    virtual PLAYER_LEVEL GetLevel() = 0;
    virtual PLAYER_LEVEL GetOpponentLevel() = 0;
    virtual std::pair<int, int> GetShotPos() = 0;
    virtual std::string PackToXML() = 0;
    virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) = 0;
};

class IResponse
{
public:
    ~IResponse() {};
    virtual Response_ID GetCommandID() = 0;
    virtual int GetPlayerID() = 0;
    virtual int GetGameID() = 0;
    virtual std::string GetField() = 0;
    virtual bool IsMyTurn() = 0;
    virtual AttackResult GetAttackResult() = 0;
    virtual std::pair<int, int> GetShotPos() = 0;
    virtual std::pair<bool, int> IsFinish() = 0;
    virtual std::string PackToString() = 0;
    virtual bool UnpackFromXML(tinyxml2::XMLDocument& doc) = 0;
};

typedef std::shared_ptr<IPlayer> IPlayerPtr;

class IServerGame
{
public:
    virtual ~IServerGame() {};
    virtual std::string GetField(int player_id) = 0;
    virtual AttackResult PlayRound(int attacker_id, int deffender_id, int ver, int hor) = 0;
    virtual bool IsFinish(int& o_winner_id, int& o_looser_id) = 0;
    virtual void GetStats(int& o_round_played, int& o_decks_left) = 0;
};

typedef std::shared_ptr<IStrategy> IStrategyPtr;
typedef std::shared_ptr<IRequest> IRequestPtr;
typedef std::shared_ptr<IResponse> IResponsePtr;
typedef std::shared_ptr<IServerGame> IGamePtr;
