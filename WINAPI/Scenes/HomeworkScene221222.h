#pragma once
class HomeworkScene221222 : public Scene {
public:
	static const float SPEED;

	class Texture1222 {
	public:
		HDC memDC = nullptr;
		int copyX = 0, copyY = 0, sizeX = 0, sizeY = 0;

		Texture1222(HDC memDC,
			int copyX, int copyY, int sizeX, int sizeY)
			: memDC(memDC)
			, copyX(copyX), copyY(copyY), sizeX(sizeX), sizeY(sizeY) {}
		~Texture1222() { memDC = nullptr; }

		void Render(HDC hdc, Vector2 pos, Vector2 size);
	};

	class LoopTexture1222 : public Texture1222 {
	public:
		LoopTexture1222(HDC memDC,
			int copyX, int copyY, int sizeX, int sizeY, float speed = 0.3f)
			: Texture1222(memDC, copyX, copyY, sizeX, sizeY)
			, xSpeed(speed) {}

		void Update();
		void Render(HDC hdc, Vector2 pos, Vector2 size);
	private:
		float moveX = 0.0f;
		float xSpeed;
	};

	class Bird1222 : public Circle {
	public:
		Bird1222();
		~Bird1222();

		virtual void Update() override;
		virtual void Render(HDC hdc) override;

		void SetTexture(Texture1222* texture) { this->texture = texture; }

		bool IsFall() { return pos.y > WIN_HEIGHT + radius; }
	private:
		float gravity = 300.0f;
		float speed = 0;
		Texture1222 *texture = nullptr;
	};

	class Pipe1222 : public Rect {
	public:
		static const Vector2 DEFAULT_SIZE;

		Pipe1222(Vector2 safePos = {WIN_WIDTH, CENTER_Y}, Vector2 size = DEFAULT_SIZE);
		~Pipe1222() = default;

		void Spawn();
		virtual void Update() override;

		virtual void Render(HDC hdc) override;
		void RenderTexture(HDC hdc,
			Texture1222* topHeadTexture = nullptr,
			Texture1222* bottomHeadTexture = nullptr,
			Texture1222* topTexture = nullptr, 
			Texture1222* bottomTexture = nullptr);

		bool Collision(Circle* circle);
	private:
		void SetPipe();

		//y좌표, 세로 너비
		Vector2 safeArea;	
		float speed = HomeworkScene221222::SPEED;
		bool pass = false;

		Rect topRect;
		Rect bottomRect;
	};

	class PipeManager1222 : public PoolingManager, public Singleton<PipeManager1222>
	{
	private:
		friend class Singleton;
		PipeManager1222();
		~PipeManager1222();

	public:
		static string PIPE_KEY;

		void Update();
		virtual void Render(HDC hdc) override;

		virtual void CreateObjects(string key, UINT poolSize);

		void SpawnObject();
		bool Collision(Circle* circle);

		void SetTopTexture(Texture1222* texture) { this->topTexture = texture; }
		void SetBottomTexture(Texture1222* texture) { this->bottomTexture = texture; }
		void SetTopHeadTexture(Texture1222* texture) { this->topHeadTexture = texture; }
		void SetBottomHeadTexture(Texture1222* texture) { this->bottomHeadTexture = texture; }
	
	private:
		float spawnTime = 3.0f;
		float spawnRate = 3.0f;

		Texture1222* topTexture = nullptr;
		Texture1222* bottomTexture = nullptr;
		Texture1222* topHeadTexture = nullptr;
		Texture1222* bottomHeadTexture = nullptr;
	};

	class ScoreManager : public Singleton<ScoreManager>
	{
	public:
		void ScoreUp();
		UINT GetScore() { return score; }
		void Render(HDC hdc);
	private:
		UINT score = 0;
	};
public:
	HomeworkScene221222();
	~HomeworkScene221222();

	// Scene을(를) 통해 상속됨
	virtual void Init() override {};
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
private:
	Bird1222* bird;
	bool gameOver = false;

	HDC memDC;
	HBITMAP hBitmap;

	LoopTexture1222* bg = nullptr;
	float bg_x = 0.0f;
	float bgSpeed = 2.0f;
};