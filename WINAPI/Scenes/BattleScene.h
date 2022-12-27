#pragma once

class BattleScene :
    public Scene
{
public:
    BattleScene();
    ~BattleScene();
    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:
    Fighter* fighter; 
    Monster* monster;
};

