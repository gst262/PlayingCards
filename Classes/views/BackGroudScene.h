#ifndef __BACKGROUD_H__
#define __BACKGROUD_H__

#include "cocos2d.h"
#include <iostream>
USING_NS_CC;

class BackGroud :public Scene {
public:
	static Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(BackGroud);
};


#endif // __BACKGROUD_H__
