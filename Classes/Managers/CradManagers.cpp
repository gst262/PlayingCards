#include "PlayingCards.h"

USING_NS_CC;

PlayingCards* PlayingCards::create(bool cardFace, int cardNumber, Suit cardSuitType) {
    PlayingCards* pRet = new(std::nothrow) PlayingCards();
    if (pRet && pRet->init(cardFace, cardNumber, cardSuitType)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

//构造函数，初始化卡牌视图
bool PlayingCards::init(bool cardFace, int cardNumber, Suit cardSuitType) {
    // Initialize with background image
    if (!Sprite::initWithFile("res/card_general.png")) {
        return false;
    }

    //卡牌分为左上，右上和下三部分，分别显示小数字，花色和大数字
    _cardFace = cardFace;
    _cardNumber = cardNumber;
    _cardSuitType = cardSuitType;

    if (_cardFace) {
        Size size = this->getContentSize();

        // 顶部区域
        float topSectionHeight = size.height / 3.0f;

        //小数字
        std::string smallNumPath = getSmallNumberTexture(_cardNumber, _cardSuitType);
        auto smallNumSprite = Sprite::create(smallNumPath);
        if (smallNumSprite) {
            smallNumSprite->setPosition(Vec2(size.width * 0.25f, size.height - (topSectionHeight / 2.0f)));
            this->addChild(smallNumSprite);
        }
        //花色
        std::string suitPath = getSuitTexture(_cardSuitType);
        auto suitSprite = Sprite::create(suitPath);
        if (suitSprite) {
            suitSprite->setPosition(Vec2(size.width * 0.75f, size.height - (topSectionHeight / 2.0f)));
            this->addChild(suitSprite);
        }
        //大数字
        std::string bigNumPath = getBigNumberTexture(_cardNumber, _cardSuitType);
        auto bigNumSprite = Sprite::create(bigNumPath);
        if (bigNumSprite) {
            bigNumSprite->setPosition(Vec2(size.width * 0.5f, size.height / 3.0f));
            this->addChild(bigNumSprite);
        }
    }

    return true;
}

std::string PlayingCards::getSuitTexture(Suit suit) {
    std::string filename;
    switch (suit) {
    case Suit::Club: filename = "club.png"; break;
    case Suit::Diamond: filename = "diamond.png"; break;
    case Suit::Heart: filename = "heart.png"; break;
    case Suit::Spade: filename = "spade.png"; break;
    }
    return "res/suits/" + filename;
}

//根据数字和花色获取左上纹理路径
std::string PlayingCards::getSmallNumberTexture(int number, Suit suit) {
    std::string color = isRed(suit) ? "red" : "black";
    return "res/number/small_" + color + "_" + getNumberString(number) + ".png";
}
//根据数字和花色获取底部纹理路径
std::string PlayingCards::getBigNumberTexture(int number, Suit suit) {
    std::string color = isRed(suit) ? "red" : "black";
    return "res/number/big_" + color + "_" + getNumberString(number) + ".png";
}
//根据花色判断是否为红色
bool PlayingCards::isRed(Suit suit) {
    return (suit == Suit::Diamond || suit == Suit::Heart);
}
//根据数字获取字符串，1-10为数字，11-13分别为A,J,Q,K   此内容在生成纹理路径时使用
std::string PlayingCards::getNumberString(int number) {
    switch (number) {
    case 1: return "A";
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    default: return std::to_string(number);
    }
}
