#pragma once

class Coin : public ImageRect {
public:
	Coin();
	~Coin();

	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Animation* animation;
};