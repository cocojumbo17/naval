#pragma once

enum AttackResult
{
    AR_MISS=0,
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
    virtual void MakeShot(int& o_ver, int& o_hor) = 0;
    virtual void ResultOfYourShot(int i_ver, int i_hor, AttackResult res) = 0;
    virtual AttackResult AttackYou(int i_ver, int i_hor) = 0;
    virtual void Finish(bool is_victory) = 0;
};

typedef std::shared_ptr<IPlayer> IPlayerPtr;
typedef std::shared_ptr<IStrategy> IStrategyPtr;
