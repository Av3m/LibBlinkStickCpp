#include "BlinkStickCommands.h"
#include "BlinkStickDevice.h"
#include "BlinkStickException.h"

#include <vector>
#include <iostream>
BlinkStickDevice::BlinkStickDevice(hid_device_info& inDeviceInfo, int inNumLeds)
{
	_mHidDeviceInfo = inDeviceInfo;
	_mHidDevice = hid_open_path(inDeviceInfo.path);
	_mExecutionThreadSync= NULL;
	_mDimLevel = 1.0;

	setNumLeds(inNumLeds);

	if ( _mHidDevice == NULL)
	{
		
		throw BlinkStickDeviceOpenException((const char*) hid_error(_mHidDevice));
	}

	hid_set_nonblocking(_mHidDevice,0);
}

BlinkStickDevice::~BlinkStickDevice()
{
	hid_close(_mHidDevice);
}

bool BlinkStickDevice::setColor(int channel, int index, int r, int g, int b)
{
	std::vector<unsigned char> msg;

	if ( channel == 0 && index == 0)
	{
		msg.push_back(0x1);
		msg.push_back(r & 0xFF);
		msg.push_back(g & 0xFF);
		msg.push_back(b & 0xFF);
	}
	else
	{

		msg.push_back(0x05);
		msg.push_back(channel);
		msg.push_back(index);
		msg.push_back(r & 0xFF);
		msg.push_back(g & 0xFF);
		msg.push_back(b & 0xFF);
	}
	
	int cnt = 0;

	_mCommandMutex.lock();
	while(!sendCommand(msg) && cnt < 3)
	{
		cnt++;
	}
	_mCommandMutex.unlock();

	if ( cnt < 3) return true;
	else return false;
	
}

bool BlinkStickDevice::setColor(int channel, int index, BlinkStickColor color)
{
	BlinkStickColor c = color.dim(_mDimLevel);

	return setColor(channel,index,c.r(),c.g(),c.b());
}

int BlinkStickDevice::getNumLeds() 
{
	return _mNumLeds;
}

void BlinkStickDevice::setNumLeds(int num)
{
	_mCommands = std::vector<std::vector<BlinkStickCommand*>>(num);
	_mNumLeds = num;
}

void BlinkStickDevice::setDimLevel(float level)
{
	_mDimLevel = level;
}

void BlinkStickDevice::switchOff()
{
	for(int i=0;i<_mNumLeds;i++)
	{
		setColor(0, i, BlinkStickColor::black);
	}
}


void BlinkStickDevice::waitForFinishExecution()
{
	for(auto it=_mExecutionThreads.begin();it!=_mExecutionThreads.end();it++)
	{
		(*it)->join();
	}
}

std::string BlinkStickDevice::getPath()
{
	return std::string(_mHidDeviceInfo.path);
}

bool BlinkStickDevice::sendCommand(std::vector<unsigned char>& inCmd)
{
	int cnt = 0;
	int ret = 0;
	ret = hid_send_feature_report(_mHidDevice,inCmd.data(),inCmd.size());

	if ( ret < 0)
	{
		//std::cout << "send feature report failed. reason: ";
		//printf_s("%ls", hid_error(_mHidDevice) );
		return false;
	}

	return true;
}


bool BlinkStickDevice::receiveCommand(std::vector<unsigned char>& outCmd, int& bytes_written)
{

	int ret = hid_get_feature_report(_mHidDevice,outCmd.data(),outCmd.size());

	bytes_written = ret;
	if ( ret < 0)
	{
		std::cout << "receive feature report failed. reason: ";
		printf_s("%ls", hid_error(_mHidDevice) );
		return false;
	}

	return true;
}

bool BlinkStickDevice::operator==(const BlinkStickDevice& other)
{
	if ( this->_mHidDeviceInfo.path == other._mHidDeviceInfo.path ) return true;
	else return false;
}

bool BlinkStickDevice::operator!=(const BlinkStickDevice& other)
{
	return ! (*this == other);
}


void BlinkStickDevice::startExecution(int cycles)
{
	for(int i=0;i<_mNumLeds;i++)
		_mExecutionThreads.push_back(new boost::thread(boost::bind(&BlinkStickDevice::_excecuteCommands,this,cycles,0,i)));
}

void BlinkStickDevice::startExecutionSync(int cycles)
{
	_mExecutionThreadSync = new boost::thread(boost::bind(&BlinkStickDevice::_excecuteCommandsSync,this,cycles));
	
}

void BlinkStickDevice::stopExecution()
{
	if ( _mExecutionThreadSync != NULL)
	{
		_mExecutionThreadSync->interrupt();
		_mExecutionThreadSync->join();
		_mExecutionThreadSync = NULL;

	}
	for(auto it=_mExecutionThreads.begin();it!=_mExecutionThreads.end();it++)
	{
		
		(*it)->interrupt();
		
	}
	waitForFinishExecution();

	_mExecutionThreads.clear();
	for(auto it=_mCommands.begin();it!=_mCommands.end();it++)
	{
		
		it->clear();
	}
}

void BlinkStickDevice::_excecuteCommands(int cycles,int channel, int index)
{
	int num_cylce = cycles;


	
	while(cycles)
	{
		std::vector<BlinkStickCommand*> commands = _mCommands[index];
		if ( commands.size() == 0) break;

		for(auto it=commands.begin();it!=commands.end();++it)
		{
			(**it)(*this,channel,index);
			boost::this_thread::sleep_for(boost::chrono::microseconds(10));
		}
			
			
		if ( cycles != -1)
			cycles--;
	}
}

void BlinkStickDevice::_excecuteCommandsSync(int cycles)
{
	int num_cylce = cycles;
	while(num_cylce)
	{
		for(int i=0;i<_mNumLeds;i++)
			_mExecutionThreads.push_back(new boost::thread(boost::bind(&BlinkStickDevice::_excecuteCommands,this,1,0,i)));
		waitForFinishExecution();
		if ( cycles != -1)
			num_cylce--;
	}
}

