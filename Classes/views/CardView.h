#ifndef __CARDIEW_H__
#define __CARDIEW_H__

//卡牌界面，游戏的交互的主要场景

#include "cocos2d.h"
#include <string>
#include "views/PlayingCards.h"
#include <stack>
#include <vector>
#include <functional>
#include "Controllers/CardController.h"


USING_NS_CC;
class CardView : public Scene {

public:
	static Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(CardView);
	void setOnContinue(const std::function<void()>& cb);

	
private:
	enum class StackSide {
		LeftTop,
		RightTop
	};
	//卡牌数据
	int _leftCardCount = 6; // 左上角卡牌数量
	int _rightCardCount = 6; // 右上角卡牌数量
	std::stack<PlayingCards*> _leftTopStack; // 左上角卡牌堆
	std::stack<PlayingCards*> _rightTopStack; // 右上角卡牌堆
	std::vector<PlayingCards*> _bottomLeftPile;// 底部左侧回退缓存卡牌堆
	PlayingCards* bottomCard = nullptr; // 底部卡牌
	Size _visibleSize; // 可见区域大小
	Vec2 _origin; // 可见区域原点坐标
	Vec2 _bottomPos;//底部卡牌位置坐标
	Node* _backButton = nullptr;
	CardController _cardController;
	std::function<void()> _onContinue;//注册回调
	Node* _winOverlay = nullptr;
	bool _winShown = false;//是否已显示胜利界面
	//界面元素
	Layer* _cardLayer = nullptr; // 卡牌层
	const float kBoundary = 0.7; // 分割常量，以top为基
	const float kSpacing = 60.0f; // 分割常量，以top为基


	void fristAddCardDeck();//首次添加卡牌堆
	void fristAddLeftTopStack();//首次添加左上角卡牌堆
	void fristAddRightTopStack();//首次添加右上角卡牌堆


	void attachCardTouch(PlayingCards* card, StackSide side);//绑定卡牌点击事件
	void onTopCardSelected(PlayingCards* card, StackSide side);
	void onBackClicked();//点击回退按钮事件
	Vec2 bottomLeftPilePositionForIndex(int index) const;//获取将要移动至回退堆的位置
	void layoutBottomLeftPile(bool animated);//移动底牌至回退牌堆
	void ensureBackButton();
	void updateBackButtonPosition();
	void maybeShowWin();
};

#endif // __CARDIEW_H__
