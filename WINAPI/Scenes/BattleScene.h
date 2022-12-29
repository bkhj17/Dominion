#pragma once

class BattleScene :
    public Scene
{
public:
    BattleScene();
    ~BattleScene();
    // Scene��(��) ���� ��ӵ�
    virtual void Start() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
    virtual void End() override;
private:
    Fighter* fighter; 
    Monster* monster;
};

