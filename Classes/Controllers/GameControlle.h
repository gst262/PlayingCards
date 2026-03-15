#ifndef __GAMECONTROLLE_H__
#define __GAMECONTROLLE_H__

#include "cocos2d.h"
#include "views/GameView.h"
#include "views/CardView.h"

class GameControlle : private cocos2d::Application
{
public:
    GameControlle();
    virtual ~GameControlle();

    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();

    virtual void applicationDidEnterBackground();

    virtual void applicationWillEnterForeground();

private:
    GameView* _gameView = nullptr; //关卡界面
    cocos2d::Scene* _cardScene = nullptr;//牌面界面
    void showGameView();//显示关卡界面
    void showCardView(int levelNumber);//显示牌面界面
};

#endif // __GAMECONTROLLE_H__
