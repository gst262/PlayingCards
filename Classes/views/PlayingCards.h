#ifndef __PLAYINGCARDS_H__
#define __PLAYINGCARDS_H__

#include "cocos2d.h"
#include <string>
#include "configs/CardResConfig.h"

class PlayingCards : public cocos2d::Sprite {
public:
    static PlayingCards* create(bool cardFace, int cardNumber, CardSuitType cardSuitType);
    virtual bool init(bool cardFace, int cardNumber, CardSuitType cardSuitType);
    void RenderCards(bool cardFace, int cardNumber, CardSuitType cardSuitType);//渲染
   
private:
    std::string getSuitTexture(CardSuitType suit);
    std::string getSmallNumberTexture(int number, CardSuitType suit);
    std::string getBigNumberTexture(int number, CardSuitType suit);
    bool isRed(CardSuitType suit);
    std::string getNumberString(int number);

    bool _cardFace;//正反
    int _cardNumber;//值
    CardSuitType _cardSuitType;//花色
};

#endif // __CARD_VIEW_H__
