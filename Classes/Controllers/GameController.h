#ifndef __GAMECONTROLLER_H__
#define __GAMECONTROLLER_H__

#include "Controllers/CardController.h"

class GameController {
public:
	using CardInfo = CardController::CardInfo;
	using InitCards = CardController::InitCards;

	InitCards initializationGame() { return _impl.initializationGame(); }
	bool clickVerification(int clickedNumber) { return _impl.clickVerification(clickedNumber); }
	CardInfo getCurrentCard() const { return _impl.getCurrentCard(); }
	bool back() { return _impl.back(); }

	int getBottomNumber() const { return _impl.getBottomNumber(); }
	int getBackPileSize() const { return _impl.getBackPileSize(); }

private:
	CardController _impl;
};

#endif
