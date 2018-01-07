#pragma once
#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtCore/QMutex>

#ifndef Q_MOC_RUN
#include "BlinkStick.h"
#endif

class QMainWindow: public QWidget
{
	Q_OBJECT

private:
	QVBoxLayout* _mMainLayout;
	QComboBox* _mComboBoxStatus;

	BlinkStick* _mBlinkStick;
	QSlider* _mDimSlider;
	QSpinBox* _mBoxLeds;

	QSettings* _mSettings;
	QSystemTrayIcon* _mTrayIcon;
	QMenu* _mContextMenu;

	QTimer* _mTimerDeviceSearch;
	QMutex* _mDevicesMutex;


	int _mCurrentBrightness;
	int _mCurrentStatus;
	int _mCurrentNumLeds;

public:
	QMainWindow();
	void closeEvent(QCloseEvent* _event);

	void readSettings();
	void writeSettings();


public slots:
	void setStatus(int index);
	void setBrightness(int brightness);
	void setNumLeds(int num);
	void setLeds();
	void trayActivated(QSystemTrayIcon::ActivationReason reason);
	void quit();
	void searchDevices();

	

	

};
