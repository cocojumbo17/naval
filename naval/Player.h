#pragma once
#include "..\common\interfaces.h"
#include "..\common\game\Field.h"

class Player : public IPlayer
{
public:
    Player(const std::string& name, IStrategyPtr p_strategy);
    virtual ~Player();
    FieldPtr GetMyField();

    //IPlayer
    virtual void Init() override;
    virtual std::string GetField() override;
    void MakeShot(int& o_ver, int& o_hor) override;
    void ResultOfYourShot(int i_ver, int i_hor, AttackResult res) override;
    AttackResult AttackYou(int i_ver, int i_hor) override;
    void Finish(bool is_victory) override;

protected:
    FieldPtr m_my_field;
    FieldPtr m_opponent_field;
    std::string m_name;
    IStrategyPtr mp_strategy;
};
typedef std::shared_ptr<Player> PlayerPtr;
