#pragma once

enum class CardKey : int {
	COOPER = 0,		//��ȭ
	SILVER,			//��ȭ
	GOLD,			//��ȭ
	ESTATE,			//������
	DUCHY,			//���۷�
	PROVINCE,		//����
	CURSE,			//����
	REMODEL,		//����
	BUREAUCRAT,		//����
	MINE,			//����
	MONEYLENDER,	//��ݾ���
	SMITHY,			//��������
	LIBRARY,		//������
	WITCH,			//����
	VILLAGE,		//����
	MILITIA,		//�κ���
	MARKET,			//����
	LABORATORY,		//�����
	THRONE_ROOM,	//������
	CHAPEL,			//�����
	CELLAR,			//�����
	GARDENS,		//����
	WORKSHOP,		//�۾���
	FESTIVAL,		//����
	MOAT,			//����
	COUNCILROOM,	//ȸ�ǽ�
	ARTISAN,		//����
	BANDIT,			//��󰭵�
	HARBINGER,		//������
	MERCHANT,		//����
	POACHER,		//�зƲ�
	SENTRY,			//���ʺ�
	VASSAL,			//����
	SIZE
};

enum class CardType : int {
	TREASURE,
	VICTORY,
	CURSE,
	ACTION,
	ATTACK,
	REACT,
	END
};



class CardData {
public:
	CardData() = default;
	~CardData() = default;

	int key = 0;
	string name = "";
	int cost = 0;
	vector<bool> type;
	vector<string> text;
	
	int victory = 0;
	bool sVictory = 0;


	vector<int> acts;
	Texture* texture = nullptr;
	POINT covered = { 0,0 }, frame = { 0,0 };
};

class Card : public ImageRect
{
	friend class CardManager;
public:
	static const Vector2 DEFAULT_SIZE;
	
public:
	Card(CardData* data);
	~Card();

	void Update() override;
	void Render(HDC hdc) override;
	void Render(HDC hdc, Rect* renderPos, bool covered);

	bool IsType(CardType type) { return data->type[(int)type]; }

	bool isVisible = true;
	bool isCovered = false;
	class MyPointMovement* movement;
	CardData* data;

	function<int(int)> SpecialVictory = nullptr;

	void SetSelectable(function<bool(Card*)> condition);
	bool IsSelectable() { return isSelectable; }

	void SetSelected(bool select) { isSelected = select; }
	bool IsSelected() { return isSelected; }
private:
	bool isSelectable = false;
	HPEN selectablePen = nullptr;

	bool isSelected = false;
	HPEN selectedPen = nullptr;
};

