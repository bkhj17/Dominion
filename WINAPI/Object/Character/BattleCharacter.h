#pragma once
#include "Character.h"
class BattleCharacter : public Character
{
protected:

public:
    BattleCharacter(wstring file, int frameX, int frameY, COLORREF trans = MAGENTA);
    ~BattleCharacter();

    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    Rect* GetAttackCollider() { return attackCollider; }
protected:
    virtual void Move();
    virtual void Attack() {}
    void EndAttack();
    virtual void SetAnimation() = 0;


private:
    virtual void CreateAnimations() = 0;

protected:
    Vector2 velocity;

    bool isAttack = false;
    Rect* attackCollider = nullptr;
};