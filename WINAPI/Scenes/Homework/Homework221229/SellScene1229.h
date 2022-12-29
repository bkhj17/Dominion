#pragma once
class SellButton1229 : public Button {
public:

    SellButton1229(Texture* texture) : Button(texture) {}

    int index = 0;
};

class SellScene1229 :
    public Scene
{
public:
    SellScene1229();
    ~SellScene1229();
    // Scene��(��) ���� ��ӵ�
    virtual void Start() override;
    virtual void End() override;

    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:
    vector<SellButton1229*> sellButtons;

    Button* backButton;


    Texture* normal;
    Texture* over;
    Texture* down;
};

