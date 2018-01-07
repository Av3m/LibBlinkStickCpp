 #pragma once
#include <hidapi/hidapi.h>
#include <vector>
#include <string>
#include <Eigen/Eigen>

#include "BlinkStickCommands.h"
#include "BlinkStickColor.h"
#include <boost/thread.hpp>

class BlinkStickDevice
{
protected:
	hid_device* _mHidDevice;
	hid_device_info _mHidDeviceInfo;
	
	std::vector<std::vector<BlinkStickCommand*>> _mCommands;

	std::vector<boost::thread*> _mExecutionThreads;
	boost::thread* _mExecutionThreadSync;
	int _mNumLeds;
	float _mDimLevel;

	boost::mutex _mCommandMutex;

	void _excecuteCommands(int cycles, int channel, int index);
	void _excecuteCommandsSync(int cycles);
public:
	BlinkStickDevice(hid_device_info& inDeviceInfo, int inNumLeds=8);
	virtual ~BlinkStickDevice();
	 
	bool setColor(int channel, int index, int r, int g, int b);
	bool setColor(int channel, int index, BlinkStickColor color);

	int getNumLeds();
	void setNumLeds(int num);

	void setDimLevel(float level);

	void switchOff();


	std::string getPath();


	bool sendCommand(std::vector<unsigned char>& inCmd);

	bool receiveCommand(std::vector<unsigned char>& outCmd, int& bytes_written);

	bool operator==(const BlinkStickDevice& other);
	bool operator!=(const BlinkStickDevice& other);

	template<class T>
	void addCommand(int index, T cmd);
	
	template<class T>
	void addCommandAllLeds( T cmd);


	void addCommandAllLeds(std::vector<BlinkStickCommand*>& inCommands)
	{
		for(auto it = _mCommands.begin();it!=_mCommands.end();++it)
		{
			*it = inCommands;
		}
	}

	void startExecution(int cycles=-1);
	void startExecutionSync(int cycles=-1);
	void stopExecution(); 
	void waitForFinishExecution();

	
};

template <class T>
void BlinkStickDevice::addCommand(int index, T cmd)
{
	T* _cmd = new T(cmd);

	_mCommands[index].push_back(_cmd);
}

template <class T>
void BlinkStickDevice::addCommandAllLeds(T cmd)
{
	for(int i=0;i<_mNumLeds;++i)
	{
		addCommand<T>(i,cmd);
	}
}
