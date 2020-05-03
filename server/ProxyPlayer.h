#pragma once
#include "..\common\interfaces.h"
#include "Field.h"

class ProxyPlayer : public IPlayer
{
public:
    ProxyPlayer();
    virtual ~ProxyPlayer();
    FieldPtr GetMyField();

    //IPlayer
    virtual void Init() override;
    virtual std::string GetField() override;
    virtual void MakeShot(int& o_ver, int& o_hor) override;
    virtual void ResultOfYourShot(int i_ver, int i_hor, AttackResult res) override;
    virtual AttackResult AttackYou(int i_ver, int i_hor) override;
    virtual void Finish(bool is_victory) override;
protected:
    FieldPtr m_my_field;
};


