#pragma once

class BattleScene :
    public Scene
{
public:
    BattleScene();
    ~BattleScene();
    // Scene을(를) 통해 상속됨
    virtual void Start() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
    virtual void End() override;
private:
    Fighter* fighter; 
    Monster* monster;
};

