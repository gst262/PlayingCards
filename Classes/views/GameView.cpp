#include "GameView.h"
#include <algorithm>
Scene* GameView::createScene() {
	return GameView::create();
}

void GameView::setOnLevelSelected(const std::function<void(int)>& cb) {
	_onLevelSelected = cb;
}
//封装渲染按钮，采用node作为封装骨架
Node* GameView::createLevelItem(int levelNumber, float radius) {
	auto node = Node::create();
	node->setContentSize(Size(radius * 2.0f, radius * 2.0f));

	auto draw = DrawNode::create();
	//绘制实心圆
	draw->drawSolidCircle(Vec2(radius, radius), radius, 0.0f, 64, Color4F(0.15f, 0.55f, 0.95f, 0.85f));
	//绘制圆形边框
	draw->drawCircle(Vec2(radius, radius), radius, 0.0f, 64, false, Color4F(1.0f, 1.0f, 1.0f, 0.9f));
	node->addChild(draw);

	auto label = Label::createWithSystemFont(std::to_string(levelNumber), "Arial", radius * 0.7f);
	if (label) {
		label->setPosition(Vec2(radius, radius));
		node->addChild(label);
	}

	return node;
}

bool GameView::init() {
	if (!Scene::init()) {
		return false;
	}

	CCLOG("GameView::init()");

	auto visibleSize = Director::getInstance()->getVisibleSize();//计算可见区域大小 size
	auto origin = Director::getInstance()->getVisibleOrigin();//获取可见区域原点坐标
	auto viewCenter = origin + visibleSize / 2;//计算视图中心点坐标
	//背景初始化
	auto bgLayer = Layer::create();
	this->addChild(bgLayer);
	auto bgSprite = Sprite::create("bg/Gmae_bg.png");

	if (bgSprite) { 
		bgSprite->setPosition(viewCenter);

		// 适配背景图片大小：根据屏幕尺寸和图片尺寸计算缩放比例，保持短边显示完全，确保铺满屏幕
		auto bgSize = bgSprite->getContentSize();
		if (bgSize.width > 0 && bgSize.height > 0) {
			float sx = visibleSize.width / bgSize.width;
			float sy = visibleSize.height / bgSize.height;
			float s = std::max(sx, sy);
			bgSprite->setScale(s);//放入缩放比列实现短边显示
		}
		bgLayer->addChild(bgSprite, 0);
	}
	else {
		//如果背景图片加载失败，绘制一个纯色背景作为替代
		auto draw = DrawNode::create();
		draw->drawSolidRect(origin, origin + visibleSize, Color4F(0.2f, 0.2f, 0.2f, 1.0f));
		draw->drawRect(origin, origin + visibleSize, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
		bgLayer->addChild(draw, 0);
	}


	//关卡层初始化
	levelLayer = Layer::create();
	this->addChild(levelLayer, 2); // zOrder=2，确保在背景之上

	//检测打印，
	if (!_onLevelSelected) {
		_onLevelSelected = [](int level) {
			CCLOG("level_selected=%d", level);
		};
	}

	//设置边距
	float marginX = visibleSize.width * 0.08f;
	float marginY = visibleSize.height * 0.10f;

	//设置行列数
	int rows = 2;
	int cols = 5;

	//计算整体可绘制区域
	float areaW = visibleSize.width - marginX * 2.0f;
	float areaH = visibleSize.height - marginY * 2.0f;

	//计算单个可绘制区域的大小
	float cellW = areaW / cols;
	float cellH = areaH / rows;

	//取短边的35%作为按钮半径，确保能显示完整并且有间距
	float radius = std::min(cellW, cellH) * 0.35f;

	for (int i = 0; i < 10; i++) {
		int levelNumber = i + 1;
		int row = i / cols;
		int col = i % cols;

		//计算当前按钮的中心位置
		float x = origin.x + marginX + cellW * (col + 0.5f);
		float y = origin.y + marginY + cellH * (rows - row - 0.5f);

		auto item = createLevelItem(levelNumber, radius);
		item->setPosition(Vec2(x - radius, y - radius));//设置起始位置
		levelLayer->addChild(item);

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [this, item, levelNumber, radius](Touch* touch, Event* event) {
			auto p = levelLayer->convertToNodeSpace(touch->getLocation());
			auto center = item->getPosition() + Vec2(radius, radius);
			if (p.distance(center) <= radius) {
				if (_onLevelSelected) {
					_onLevelSelected(levelNumber);
				}
				return true;
			}
			return false;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, item);
	}

	return true;
}
