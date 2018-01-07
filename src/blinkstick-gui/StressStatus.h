#pragma once
#include <QtGui/QIcon>
#include <QtCore/QString>
#include <QtGui/QAction>
#include "BlinkStickCommands.h"
#include <map>

class QStressAction: public QAction
{
	Q_OBJECT;

	int mStressLevel;
public:
	QStressAction(QIcon& icon, const QString&text, QObject* parent, int stresslevel) : QAction(icon, text, parent)
	{
		mStressLevel = stresslevel;

		connect(this, SIGNAL(triggered()), this, SLOT(triggeredProxy()));
	}
signals:
	void changedStressLevel(int);

public slots:
	void triggeredProxy();

};

class StressStatus
{
private:

	StressStatus()
	{
	}

	void _register()
	{
		mStatusMap[mStressLevel] = this;
		mStatusMapReverse[this] = mStressLevel;
	}
public:
	enum StressLevel
	{
		GO_AWAY = 0,
		VERY_BUSY,
		BUSY,
		NEED_HELP,
		WTF,
		CHATTY,
		PARTY,
		AWAY
	};
	StressLevel mStressLevel;
	QString mDescription;
	QIcon mIcon;
	std::vector<BlinkStickCommand*> mBlinkCommands;

	bool operator<(StressStatus& other) {
		return mStressLevel < other.mStressLevel;
	}

	bool operator>(StressStatus& other) {
		return mStressLevel > other.mStressLevel;
	}


	static std::map<StressLevel, StressStatus*> mStatusMap;
	static std::map<StressStatus*, StressLevel> mStatusMapReverse;
	static void createStatuses();
};
