#include "BackGroudScene.h"

Scene* BackGroud::createScene() {
	return BackGroud::create();
}

bool BackGroud::init() {
	if (!Scene::init()) {
		return false;
	}

	auto bgLayer = Layer::create();
	this->addChild(bgLayer);
	auto bgSprite = Sprite::create("bg/Gmae_bg.png");

	if (bgSprite) { // 判空：避免图片缺失导致崩溃
		// 获取屏幕尺寸（适配不同分辨率）
		Size winSize = Director::getInstance()->getWinSize();

		// 关键1：让背景图片铺满屏幕
		bgSprite->setPosition(winSize / 2); // 背景居中
		bgSprite->setContentSize(winSize);  // 大小适配屏幕

		// 关键2：设置背景为最底层（zOrder=0，后续元素z值更大，显示在上方）
		bgLayer->addChild(bgSprite, 1);
	}


	return true;
}
