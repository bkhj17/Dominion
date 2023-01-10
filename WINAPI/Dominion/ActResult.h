#pragma once

class ActResult {
public:
	virtual void Clear() = 0;
};

//���� ī�� ���
class GetCardResult : public ActResult {
public:
	deque<Card*> cards;

	void Clear() override;
};

//����ó ����
class GetSupplierResult : public ActResult {
public:
	CardSupplier* supplier = nullptr;
	void Clear() override;
};