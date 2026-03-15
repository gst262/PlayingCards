#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__

//游戏界面，用于显示关卡，并接受用户选择关卡的输入

#include "cocos2d.h"
#include <string>
#include <functional>


USING_NS_CC;
class GameView : public Scene {

public:
	static Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(GameView);

	//创建触发事件
	void setOnLevelSelected(const std::function<void(int)>& cb);

private:
	Layer* levelLayer = nullptr; // 显示关卡的层，默认为空
	std::function<void(int)> _onLevelSelected;// 创建回调槽
	Node* createLevelItem(int levelNumber, float radius);

};

#endif // __GAMEVIEW_H__
