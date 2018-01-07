#include "QMainWindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include "StressStatus.h"

QMainWindow::QMainWindow(): _mBlinkStick(new BlinkStick())
{
	setContextMenuPolicy(Qt::CustomContextMenu);

	StressStatus::createStatuses();
	_mBlinkStick->scanForNewDevices();
	_mComboBoxStatus = new QComboBox();
	_mMainLayout = new QVBoxLayout();
	_mDimSlider = new QSlider(Qt::Orientation::Horizontal);
	_mBoxLeds = new QSpinBox();
	_mSettings = new QSettings("av3m", "Blinkstick-StressLights");
	_mContextMenu = new QMenu();
	_mTrayIcon = new QSystemTrayIcon();
	_mTimerDeviceSearch = new QTimer();
	_mDevicesMutex = new QMutex();
	_mTrayIcon->setContextMenu(_mContextMenu);
	_mTrayIcon->show();
	

	_mBoxLeds->setMinimum(1);
	_mBoxLeds->setMaximum(8);
	_mBoxLeds->setValue(8);

	_mDimSlider->setMinimum(0);
	_mDimSlider->setMaximum(255);
	_mDimSlider->setValue(100);

	_mTimerDeviceSearch->setInterval(2000);
	_mTimerDeviceSearch->start();
	
	QHBoxLayout* led_box = new QHBoxLayout();
	led_box->addWidget(new QLabel("Number LEDs of Device"));
	led_box->addWidget(_mBoxLeds);

	QHBoxLayout* dim_box = new QHBoxLayout();
	dim_box->addWidget(new QLabel("Brightness:"));
	dim_box->addWidget(_mDimSlider);
	_mMainLayout->addLayout(led_box);
	_mMainLayout->addLayout(dim_box);
	_mMainLayout->addWidget(_mComboBoxStatus);
	this->setLayout(_mMainLayout);

	_mComboBoxStatus->setIconSize(QSize(48,48));
	_mComboBoxStatus->setStyleSheet("font-size:16px;");

	for(auto it=StressStatus::mStatusMap.begin();it!= StressStatus::mStatusMap.end();++it)
	{
		_mComboBoxStatus->addItem(it->second->mIcon, it->second->mDescription, it->second->mStressLevel);
	}

	for (auto it = StressStatus::mStatusMap.begin(); it != StressStatus::mStatusMap.end(); ++it)
	{
		QAction* action = new QStressAction(it->second->mIcon, it->second->mDescription, this,it->second->mStressLevel);
		_mContextMenu->addAction(action);
		connect(action, SIGNAL(changedStressLevel(int)), this, SLOT(setStatus(int)));
	}

	_mContextMenu->addSeparator();
	QAction* quit = new QAction("Quit",this);
	_mContextMenu->addAction(quit);
	connect(quit, SIGNAL(triggered()), this, SLOT(quit()));


	

	connect(_mComboBoxStatus,SIGNAL(currentIndexChanged(int)),this,SLOT(setStatus(int)));
	connect(_mDimSlider,SIGNAL(valueChanged(int)),this,SLOT(setBrightness(int)));
	connect(_mBoxLeds,SIGNAL(valueChanged(int)),this,SLOT(setNumLeds(int)));
	connect(_mTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
	connect(_mTimerDeviceSearch, SIGNAL(timeout()), this, SLOT(searchDevices()));
	readSettings();
}

void QMainWindow::setStatus(int index)
{
	_mCurrentStatus = index;
	_mTrayIcon->setIcon(_mComboBoxStatus->itemIcon(index));
	setLeds();
}

void QMainWindow::setBrightness(int brightness)
{
	_mCurrentBrightness = brightness;
	setLeds();
}

void QMainWindow::setNumLeds(int num)
{
	_mCurrentNumLeds = num;
	setLeds();
}

void QMainWindow::setLeds()
{
	_mDevicesMutex->lock();
	float brightness = _mCurrentBrightness / 255.0f;
	auto devices = _mBlinkStick->getAllDevices();

	
	for (auto it = devices.begin(); it != devices.end(); ++it)
	{
		(*it)->stopExecution();
		(*it)->setNumLeds(_mCurrentNumLeds);
		(*it)->setDimLevel(brightness);
	}


	StressStatus* stress_status = StressStatus::mStatusMap.at(static_cast<StressStatus::StressLevel>(_mComboBoxStatus->itemData(_mCurrentStatus).toInt()));
	
	if ( stress_status != NULL)
	{
		for (auto it = devices.begin(); it != devices.end(); ++it)
		{
			(*it)->addCommandAllLeds(stress_status->mBlinkCommands);
		}

		for (auto it = devices.begin(); it != devices.end(); ++it)
		{
			(*it)->startExecutionSync();
		}
	}

	_mDevicesMutex->unlock();
	
}

void QMainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{

	switch(reason) {
	case QSystemTrayIcon::Trigger:
		show();
		break;
	case QSystemTrayIcon::DoubleClick:
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	default:
		break;
	}
}

void QMainWindow::quit()
{
	_mTrayIcon->hide();
	this->close();
}

void QMainWindow::searchDevices()
{
	_mDevicesMutex->lock();
	std::vector<BlinkStickDevice*> devices = _mBlinkStick->scanForNewDevices();
	_mDevicesMutex->unlock();

	if (devices.size() > 0)
		setLeds();
}

void QMainWindow::closeEvent(QCloseEvent* _event)
{

#ifdef Q_OS_OSX
	if (!event->spontaneous() || !isVisible()) {
		return;
	}
#endif
	if (_mTrayIcon->isVisible()) {
		QMessageBox::information(this, tr("Systray"),
			tr("The program will keep running in the "
				"system tray. To terminate the program, "
				"choose <b>Quit</b> in the context menu "
				"of the system tray entry."));
		hide();
		_event->ignore();
	}
	else
	{
		writeSettings();
		auto devices = _mBlinkStick->getAllDevices();
		for (auto it = devices.begin(); it != devices.end(); ++it)
		{
			(*it)->stopExecution();
			(*it)->switchOff();
		}
		QWidget::closeEvent(_event);
	}
	
}

void QMainWindow::readSettings()
{
	_mCurrentBrightness = _mSettings->value("brightnessLevel", 100).toInt();
	_mCurrentStatus = _mSettings->value("Status", 6).toInt();
	_mCurrentNumLeds = _mSettings->value("NumLEDs", 6).toInt();

	_mDimSlider->setValue(_mSettings->value("brightnessLevel", 100).toInt());
	_mComboBoxStatus->setCurrentIndex(_mSettings->value("Status", 6).toInt());
	_mBoxLeds->setValue(_mSettings->value("NumLEDs", 6).toInt());
}

void QMainWindow::writeSettings()
{

	_mSettings->setValue("brightnessLevel", _mDimSlider->value());
	_mSettings->setValue("Status", _mComboBoxStatus->currentIndex());
	_mSettings->setValue("NumLEDs", _mBoxLeds->value());
}

