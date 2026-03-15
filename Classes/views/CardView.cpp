#include "CardView.h"
#include <algorithm>

Scene* CardView::createScene() {
	return CardView::create();
}

//注册继续按钮回调
void CardView::setOnContinue(const std::function<void()>& cb) {
	_onContinue = cb;
}

bool CardView::init() {
	if (!Scene::init()) {
		return false;
	}

	_visibleSize = Director::getInstance()->getVisibleSize();
	_origin = Director::getInstance()->getVisibleOrigin();
	auto viewCenter = _origin + _visibleSize / 2;

	//背景初始化
	auto bgLayer = Layer::create();
	this->addChild(bgLayer, 0);
	auto bgSprite = Sprite::create("bg/Crad_bg.png");

	if (bgSprite) {
		bgSprite->setPosition(viewCenter);
		auto bgSize = bgSprite->getContentSize();
		if (bgSize.width > 0 && bgSize.height > 0) {
			float sx = _visibleSize.width / bgSize.width;
			float sy = _visibleSize.height / bgSize.height;
			float s = std::max(sx, sy);
			bgSprite->setScale(s);
		}
		bgLayer->addChild(bgSprite, 0);
	}

	//关卡层初始化
	_cardLayer = Layer::create();
	this->addChild(_cardLayer, 1);

	fristAddCardDeck();
	return true;
}

void CardView::fristAddLeftTopStack() {
	const float margin = 40.0f;
	const Vec2 offset(18.0f, -18.0f);

	auto first = PlayingCards::create(false, 1, CST_CLUBS);
	if (!first) return;

	auto cardSize = first->getContentSize();
	auto start = Vec2(_origin.x + margin + cardSize.width * 0.5f,
		_origin.y + _visibleSize.height - margin - cardSize.height * 0.5f);

	first->setPosition(start);
	_cardLayer->addChild(first, 10);
	_leftTopStack.push(first);
	attachCardTouch(first, StackSide::LeftTop);

	for (int i = 1; i < _leftCardCount; i++) {
		auto card = PlayingCards::create(false, 1, CST_CLUBS);
		if (!card) continue;
		card->setPosition(start + offset * i);
		_cardLayer->addChild(card, 10 + i);
		_leftTopStack.push(card);
		attachCardTouch(card, StackSide::LeftTop);
	}
}

void CardView::fristAddRightTopStack() {
	const float margin = 40.0f;
	const Vec2 offset(-18.0f, -18.0f);

	auto first = PlayingCards::create(false, 1, CST_CLUBS);
	if (!first) return;

	auto cardSize = first->getContentSize();
	auto start = Vec2(_origin.x + _visibleSize.width - margin - cardSize.width * 0.5f,
		_origin.y + _visibleSize.height - margin - cardSize.height * 0.5f);

	first->setPosition(start);
	_cardLayer->addChild(first, 10);
	_rightTopStack.push(first);
	attachCardTouch(first, StackSide::RightTop);

	for (int i = 1; i < _rightCardCount; i++) {
		auto card = PlayingCards::create(false, 1, CST_CLUBS);
		if (!card) continue;
		card->setPosition(start + offset * i);
		_cardLayer->addChild(card, 10 + i);
		_rightTopStack.push(card);
		attachCardTouch(card, StackSide::RightTop);
	}
}

//首次添加卡牌堆
void CardView::fristAddCardDeck() {
	
	fristAddLeftTopStack();
	fristAddRightTopStack();

	auto card = PlayingCards::create(true, 1, CST_CLUBS);
	if (card) {
		auto y = _origin.y + _visibleSize.height * 0.25f;
		card->setPosition(Vec2(_origin.x + _visibleSize.width * 0.5f, y));
		_cardLayer->addChild(card, 50);
		bottomCard = card;
		_bottomPos = bottomCard->getPosition();//获取并保存底部卡牌位置
	}

	auto init = _cardController.initializationGame();
	if (bottomCard) {
		bottomCard->RenderCards(init.bottom.face, init.bottom.number, init.bottom.suit);
		bottomCard->setTag(init.bottom.number);
	}
	if (!_leftTopStack.empty() && _leftTopStack.top()) {
		auto top = _leftTopStack.top();
		top->RenderCards(init.leftTop.face, init.leftTop.number, init.leftTop.suit);
		top->setTag(init.leftTop.number);
	}
	if (!_rightTopStack.empty() && _rightTopStack.top()) {
		auto top = _rightTopStack.top();
		top->RenderCards(init.rightTop.face, init.rightTop.number, init.rightTop.suit);
		top->setTag(init.rightTop.number);
	}

	ensureBackButton();

}

void CardView::attachCardTouch(PlayingCards* card, StackSide side) {
	if (!card || !_cardLayer) return;

	auto listener = EventListenerTouchOneByOne::create();
	// 触摸事件吞噬，防止穿透
	listener->setSwallowTouches(true);

	//使用点击事件的第一个生命周期进行回调
	listener->onTouchBegan = [this, card, side](Touch* touch, Event* event) {

		//效验点击位置是否在卡牌范围内
		auto p = _cardLayer->convertToNodeSpace(touch->getLocation());
		if (!card->getBoundingBox().containsPoint(p)) return false;

		//判断点击的卡牌是否在对应堆顶
		switch (side) {
			case StackSide::LeftTop:
				if (_leftTopStack.empty() || _leftTopStack.top() != card) return false;
				break;
			case StackSide::RightTop:
				if (_rightTopStack.empty() || _rightTopStack.top() != card) return false;
				break;
		}

		int clickedNumber = card->getTag();
		if (!_cardController.clickVerification(clickedNumber)) return false;

		onTopCardSelected(card, side);
		return true;
	};

	//绑定钩子
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, card);
}

//执行移动动画
void CardView::onTopCardSelected(PlayingCards* card, StackSide side) {
	if (!card) return;

	//防手贱
	switch (side) {
		case StackSide::LeftTop:
			if (_leftTopStack.empty() || _leftTopStack.top() != card) return;
			_leftTopStack.pop();
			break;
		case StackSide::RightTop:
			if (_rightTopStack.empty() || _rightTopStack.top() != card) return;
			_rightTopStack.pop();
			break;
	}

	auto oldBottom = bottomCard;
	if (oldBottom) 
	{
		int index = static_cast<int>(_bottomLeftPile.size());
		_bottomLeftPile.push_back(oldBottom);
		//强制停止底部卡牌的所有动作，设置层级准备移动
		oldBottom->stopAllActions();
		oldBottom->setLocalZOrder(60 + index);

		//创建动作序列
		auto move = MoveTo::create(0.35f, bottomLeftPilePositionForIndex(index));
		auto flip = Sequence::create(ScaleTo::create(0.12f, 0.0f, 1.0f), ScaleTo::create(0.12f, 1.0f, 1.0f), nullptr);
		oldBottom->runAction(Spawn::create(move, flip, nullptr));
	}

	card->stopAllActions();
	card->setLocalZOrder(120);

	auto move = MoveTo::create(0.35f, _bottomPos);
	auto rot = RotateBy::create(0.35f, 360.0f);//翻转180
	auto done = CallFunc::create([this, card]() {
		bottomCard = card;
		bottomCard->setRotation(0.0f);//重置旋转角度
		updateBackButtonPosition();
	});
	card->runAction(Sequence::create(Spawn::create(move, rot, nullptr), done, nullptr));

	if (side == StackSide::LeftTop) {
		if (!_leftTopStack.empty() && _leftTopStack.top()) {
			auto next = _cardController.getCurrentCard();
			auto top = _leftTopStack.top();
			top->RenderCards(next.face, next.number, next.suit);
			top->setTag(next.number);
		}
	}
	else {
		if (!_rightTopStack.empty() && _rightTopStack.top()) {
			auto next = _cardController.getCurrentCard();
			auto top = _rightTopStack.top();
			top->RenderCards(next.face, next.number, next.suit);
			top->setTag(next.number);
		}
	}

	maybeShowWin();
}

void CardView::ensureBackButton() {
	if (!_cardLayer) return;

	if (_backButton) {
		_backButton->removeFromParent();
		_backButton = nullptr;
	}

	auto label = Label::createWithSystemFont("Back", "Arial", 36);
	auto item = MenuItemLabel::create(label, [this](Ref*) { onBackClicked(); });
	auto menu = Menu::create(item, nullptr);
	menu->setPosition(Vec2::ZERO);
	_cardLayer->addChild(menu, 200);
	_backButton = item;
	updateBackButtonPosition();
}

void CardView::updateBackButtonPosition() {
	if (!_backButton) return;
	_backButton->setVisible(bottomCard != nullptr);
	if (!bottomCard) return;
	auto cardSize = bottomCard->getContentSize();
	_backButton->setPosition(bottomCard->getPosition() + Vec2(cardSize.width * 0.9f, 0.0f));
}

//显示胜利界面
void CardView::maybeShowWin() {
	if (_winShown) return;
	if (!_leftTopStack.empty() || !_rightTopStack.empty()) return;

	_winShown = true;

	auto overlay = LayerColor::create(Color4B(0, 0, 0, 160));
	overlay->setContentSize(_visibleSize);
	overlay->setPosition(_origin);

	auto title = Label::createWithSystemFont("Success", "Arial", 72);
	if (title) {
		title->setPosition(_origin + Vec2(_visibleSize.width * 0.5f, _visibleSize.height * 0.58f));
		overlay->addChild(title);
	}

	auto tip = Label::createWithSystemFont("点击继续", "Arial", 28);
	if (tip) {
		tip->setPosition(_origin + Vec2(_visibleSize.width * 0.5f, _visibleSize.height * 0.48f));
		overlay->addChild(tip);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch*, Event*) { return true; };
	listener->onTouchEnded = [this](Touch*, Event*) {
		auto cb = _onContinue;
		_onContinue = nullptr;
		if (cb) cb();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, overlay);

	this->addChild(overlay, 1000);
	_winOverlay = overlay;
}

//获取将要移动至回退堆的位置
Vec2 CardView::bottomLeftPilePositionForIndex(int index) const {
	float cardWidth = 0.0f;

	if (bottomCard) {
		cardWidth = bottomCard->getContentSize().width;
	}
	else if (!_bottomLeftPile.empty()) {
		cardWidth = _bottomLeftPile.back()->getContentSize().width;
	}
	if (cardWidth <= 0.0f) cardWidth = 180.0f;

	float overlap = std::min(26.0f, cardWidth * 0.18f);
	auto base = _bottomPos + Vec2(-cardWidth * 1.2f, 0.0f);
	return base + Vec2(-overlap * index, 0.0f);
}

//移动底牌至回退牌堆
void CardView::layoutBottomLeftPile(bool animated) {
	for (int i = 0; i < static_cast<int>(_bottomLeftPile.size()); i++) {
		auto card = _bottomLeftPile[i];
		if (!card) continue;
		auto pos = bottomLeftPilePositionForIndex(i);
		card->setLocalZOrder(60 + i);
		if (animated) {
			card->stopAllActions();
			card->runAction(MoveTo::create(0.2f, pos));
		}
		else {
			card->setPosition(pos);
		}
	}
}

void CardView::onBackClicked() {
	bool canBack = true;
	if (!canBack) return;
	if (!bottomCard) return;

	if (_bottomLeftPile.empty()) return;
	if (!_cardController.back()) return;

	auto removing = bottomCard;
	bottomCard = nullptr;
	updateBackButtonPosition();

	removing->stopAllActions();
	removing->runAction(Sequence::create(ScaleTo::create(0.12f, 0.0f), RemoveSelf::create(), nullptr));

	auto card = _bottomLeftPile.back();
	_bottomLeftPile.pop_back();
	layoutBottomLeftPile(true);

	if (!card) return;
	card->RenderCards(true, _cardController.getBottomNumber(), CST_CLUBS);
	card->setTag(_cardController.getBottomNumber());
	card->stopAllActions();
	card->setLocalZOrder(120);

	auto move = MoveTo::create(0.35f, _bottomPos);
	auto rot = RotateBy::create(0.35f, 360.0f);
	auto done = CallFunc::create([this, card]() {
		bottomCard = card;
		bottomCard->setRotation(0.0f);
		updateBackButtonPosition();
	});
	card->runAction(Sequence::create(Spawn::create(move, rot, nullptr), done, nullptr));
}
