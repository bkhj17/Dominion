#pragma once

class HomeworkScene221219 : public Scene
{
    static const int MAX_RECT = 300;
    static const int MIN_SHOW_TIME = 0;
    static const int MAX_SHOW_TIME = 50;
    class VanishingRect {
        static const int MIN_VANISH_TIME = 0;
        static const int MAX_VANISH_TIME = 1000;
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

        const clock_t& VanishTime() { return vanishTime; }
    private:
        RECT winSize = RECT();
        SHAPE shape = SHAPE::RECT;
        Rect lane = Rect();
        HBRUSH brush = nullptr;

        clock_t vanishTime = 0;
    };

public:
    HomeworkScene221219();
    ~HomeworkScene221219();

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

private:
    int nShow = 0;

    vector<VanishingRect*> vRect;

    int nextShowRect = 0;
    clock_t nextShowTime = 0;

    int nextVanishRect = 0;
    clock_t nextVanishTime = 0;
};

