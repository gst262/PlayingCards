#ifndef __CARDCONTROLLER_H__
#define __CARDCONTROLLER_H__

#include "configs/CardResConfig.h"
#include <vector>

class CardController {
public:
	struct CardInfo {
		bool face = true;
		int number = 1;
		CardSuitType suit = CST_CLUBS;
	};

	struct InitCards {
		CardInfo bottom;
		CardInfo leftTop;
		CardInfo rightTop;
	};

	/* 初始化游戏，返回底牌和两张顶牌的信息 */
	InitCards initializationGame();
	/* 验证点击的数字是否合法 */
	bool clickVerification(int clickedNumber);
	/* 生成一张牌 */
	CardInfo getCurrentCard() const;
	bool back();

	int getBottomNumber() const;
	int getBackPileSize() const;

private:
	int _bottomNumber = 0;
	std::vector<int> _backPileNumbers;
};

#endif
