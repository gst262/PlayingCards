#ifndef __PLAYINGCARDS_H__
#define __PLAYINGCARDS_H__

#include "cocos2d.h"
#include <string>

class PlayingCards : public cocos2d::Sprite {
public:
    enum class Suit {
        Club,    // Black
        Diamond, // Red
        Heart,   // Red
        Spade    // Black
    };

    static PlayingCards* create(bool cardFace, int cardNumber, Suit cardSuitTyp);

    virtual bool init(bool cardFace, int cardNumber, Suit cardSuitType);

private:
    std::string getSuitTexture(Suit suit);
    std::string getSmallNumberTexture(int number, Suit suit);
    std::string getBigNumberTexture(int number, Suit suit);
    bool isRed(Suit suit);
    std::string getNumberString(int number);

    bool _cardFace;//正反
    int _cardNumber;//值
    Suit _cardSuitType;//花色
};

#endif // __CARD_VIEW_H__
