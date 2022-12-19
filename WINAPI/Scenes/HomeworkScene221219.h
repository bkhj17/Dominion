#pragma once

class HomeworkScene221219 :
    public Scene
{
    static const int MAX_RECT = 30;

    class VanishingRect {

    public:
        VanishingRect();
        ~VanishingRect();
        enum class SHAPE {
            RECT,
            ELLIPSE,
            TRIANGE
        };

        void Init();

        void Update();
        void Render(HDC hdc);
    private:

        RECT winSize;
        SHAPE shape;
        Rect rect;
        HBRUSH brush;
    };


public:
    HomeworkScene221219();
    ~HomeworkScene221219();


    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;


private:
    vector<VanishingRect*> vRect;

    int nextShowRect = 0;
    clock_t nextShowTime = 0;

    int nextVanishRect = 0;
    clock_t nextVanishTime = 0;
};

