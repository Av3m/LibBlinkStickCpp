#include "StressStatus.h"

std::map<StressStatus::StressLevel, StressStatus*> StressStatus::mStatusMap;
std::map<StressStatus*, StressStatus::StressLevel> StressStatus::mStatusMapReverse;


void QStressAction::triggeredProxy()
{
	emit changedStressLevel(mStressLevel);
}


void StressStatus::createStatuses()
{
	StressStatus* go_away = new StressStatus();
	go_away->mStressLevel = GO_AWAY;
	go_away->mDescription = "Go Away!";
	go_away->mIcon = QIcon(":images/go_away.png");
	go_away->mBlinkCommands.push_back(new Pulse(500, 50, BlinkStickColor::red));
	go_away->_register();

	StressStatus* very_busy = new StressStatus();;
	very_busy->mStressLevel = VERY_BUSY;
	very_busy->mDescription = "Very busy.";
	very_busy->mIcon = QIcon(":images/very_busy.png");
	very_busy->mBlinkCommands.push_back(new Pulse(3000, 100, BlinkStickColor::orange));
	very_busy->_register();


	StressStatus* busy = new StressStatus();;
	busy->mStressLevel = BUSY;
	busy->mDescription = "Busy.";
	busy->mIcon = QIcon(":images/busy.png");
	busy->mBlinkCommands.push_back(new Pulse(3000, 100, BlinkStickColor::yellow));
	busy->_register();


	StressStatus* need_help = new StressStatus();;
	need_help->mStressLevel = NEED_HELP;
	need_help->mDescription = "need help/have a question.";
	need_help->mIcon = QIcon(":images/need_help.png");
	need_help->mBlinkCommands.push_back(new Pulse(1000, 100, BlinkStickColor::blue));
	need_help->_register();


	StressStatus* wtf = new StressStatus();;
	wtf->mStressLevel = WTF;
	wtf->mDescription = "WTF?!?";
	wtf->mIcon = QIcon(":images/wtf.png");
	wtf->mBlinkCommands.push_back(new LinearGradient(100, 10, BlinkStickColor::red, BlinkStickColor::blue));
	wtf->mBlinkCommands.push_back(new LinearGradient(100, 10, BlinkStickColor::blue, BlinkStickColor::red));
	wtf->_register();


	StressStatus* party = new StressStatus();;
	party->mStressLevel = PARTY;
	party->mDescription = "Partey!";
	party->mIcon = QIcon(":images/party.png");
	party->mBlinkCommands.push_back(new LinearGradient(1000, 40, BlinkStickColor::red, BlinkStickColor::blue));
	party->mBlinkCommands.push_back(new LinearGradient(1000, 40, BlinkStickColor::blue, BlinkStickColor::green));
	party->mBlinkCommands.push_back(new LinearGradient(1000, 40, BlinkStickColor::green, BlinkStickColor::magenta));
	party->mBlinkCommands.push_back(new LinearGradient(1000, 40, BlinkStickColor::magenta, BlinkStickColor::yellow));
	party->mBlinkCommands.push_back(new LinearGradient(1000, 40, BlinkStickColor::yellow, BlinkStickColor::cyan));
	party->mBlinkCommands.push_back(new LinearGradient(1000, 40, BlinkStickColor::cyan, BlinkStickColor::red));
	party->_register();


	StressStatus* chatty = new StressStatus();;
	chatty->mStressLevel = CHATTY;
	chatty->mDescription = "Chatty.";
	chatty->mIcon = QIcon(":images/chatty.png");
	chatty->mBlinkCommands.push_back(new Pulse(3000,100,BlinkStickColor::green));
	chatty->_register();

	StressStatus* away = new StressStatus();;
	away->mStressLevel = AWAY;
	away->mDescription = "Away. Have a break.";
	away->mIcon = QIcon(":images/away.png");
	away->mBlinkCommands.push_back(new Flash(3000, 100,0, BlinkStickColor::yellow));
	away->_register();
;
}