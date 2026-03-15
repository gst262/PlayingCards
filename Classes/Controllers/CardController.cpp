#include "CardController.h"
#include <cstdlib>
#include <ctime>

//边界
static int clampCardNumber(int n) {
	if (n < 1) return 1;
	if (n > 13) return 13;
	return n;
}

//随机种子
static void seedIfNeeded() {
	static bool seeded = false;
	if (seeded) return;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	seeded = true;
}

//是否有效数字
static bool isValidNumber(int n) {
	return n >= 1 && n <= 13;
}

//是否相邻
static bool isAdjacent(int a, int b) {
	int d = a - b;
	if (d < 0) d = -d;
	return d == 1 || d == 0;
}

//随机生成花色
static CardSuitType creatRandoCst() {
	seedIfNeeded();
	int v = (std::rand() % 4) + 1;
	return static_cast<CardSuitType>(v);
}

//随机生成数字
static int creatRandoNumber() {
	seedIfNeeded();
	return clampCardNumber((std::rand() % 13) + 1);
}

CardController::InitCards CardController::initializationGame() {
	_backPileNumbers.clear();
	/* 创建一个合法的底牌数字，不合法时默认 5 */
	int n = creatRandoNumber();
	_bottomNumber = isValidNumber(n) ? n : 5;

	/* 初始化左右两张顶牌（底牌的 ±1） */
	int left = (_bottomNumber == 1) ? 2 : (_bottomNumber - 1);
	int right = (_bottomNumber == 13) ? 12 : (_bottomNumber + 1);

	InitCards init;
	init.bottom = { true, _bottomNumber, creatRandoCst() };
	init.leftTop = { true, left, creatRandoCst() };
	init.rightTop = { true, right, creatRandoCst() };
	return init;
}
/* 点击校验：合法则把旧底牌推入回退堆，并更新底牌 */
bool CardController::clickVerification(int clickedNumber) {
	clickedNumber = clampCardNumber(clickedNumber);
	if (!isValidNumber(_bottomNumber)) return false;
	if (!isAdjacent(clickedNumber, _bottomNumber)) return false;

	/* 更新回退堆并更新底牌 */
	_backPileNumbers.push_back(_bottomNumber);
	_bottomNumber = clickedNumber;
	return true;
}
/* 生成一张牌 */
CardController::CardInfo CardController::getCurrentCard() const {
	seedIfNeeded();
	/* 构建候选数字列表 */
	bool present[14] = { false };

	auto markBase = [&](int base) {
		for (int d = -1; d <= 1; d++) {
			int n = base + d;
			if (isValidNumber(n)) present[n] = true;
		}
	};

	markBase(_bottomNumber);
	for (size_t i = 0; i < _backPileNumbers.size(); i++) {
		markBase(_backPileNumbers[i]);
	}

	int count = 0;
	for (int n = 1; n <= 13; n++) {
		if (present[n]) count++;
	}
	if (count == 0) {
		return { true, clampCardNumber(_bottomNumber), creatRandoCst() };
	}

	/* 从候选列表中随机取一个数字 */
	int pick = (std::rand() % count);
	for (int n = 1; n <= 13; n++) {
		if (!present[n]) continue;
		if (pick == 0) return { true, n, creatRandoCst() };
		pick--;
	}

	return { true, clampCardNumber(_bottomNumber), creatRandoCst() };
}
/* 回退：从回退堆取出 top 作为新的底牌 */
bool CardController::back() {
	if (_backPileNumbers.empty()) return false;
	_bottomNumber = clampCardNumber(_backPileNumbers.back());
	_backPileNumbers.pop_back();
	return true;
}

int CardController::getBottomNumber() const {
	return _bottomNumber;
}

int CardController::getBackPileSize() const {
	return static_cast<int>(_backPileNumbers.size());
}
