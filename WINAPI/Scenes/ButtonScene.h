#pragma once

class ButtonScene :
    public Scene
{
public:
    ButtonScene();
    ~ButtonScene();
    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    void StartBattleScene();
    void StartDigScene();
private:
    HFONT hFont;

    Button* battleButton;
    Button* digButton;

};

