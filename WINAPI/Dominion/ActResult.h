#pragma once

class ActResult {
public:
	virtual void Clear() = 0;
};

//선택 카드 목록
class GetCardResult : public ActResult {
public:
	deque<Card*> cards;

	void Clear() override;
};

//공급처 선택
class GetSupplierResult : public ActResult {
public:
	CardSupplier* supplier = nullptr;
	void Clear() override;
};