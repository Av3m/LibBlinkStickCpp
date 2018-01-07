#include "BlinkStickCommands.h"
#include "BlinkStickDevice.h"


Wait::Wait(int msec)
{
	_msec = msec*1000;
}

void Wait::operator()(BlinkStickDevice& inDevice, int channel, int index)
{
	boost::this_thread::sleep(boost::posix_time::microseconds(_msec));
}

LinearGradient::LinearGradient(int msec, int num_steps, BlinkStickColor color_start, BlinkStickColor color_end)
{
	_msec = msec* 1000;
	_numSteps = num_steps;
	_colorStart = color_start;
	_colorEnd = color_end;
}

void LinearGradient::operator()(BlinkStickDevice& inDevice, int channel, int index)
{
	int delta_msec = _msec / _numSteps;

	float t = 1.0/(float) _msec;

	int time = 0;

	while (time <= _msec)
	{

		inDevice.setColor(channel,index, _colorStart.transition(_colorEnd,t*time));
		boost::this_thread::sleep(boost::posix_time::microseconds(delta_msec));
		time += delta_msec;
	}
}

Flash::Flash(int timePre, int timeHigh, int timePost, BlinkStickColor color)
{
	_timePre = timePre;
	_timeHigh = timeHigh;
	_timePost = timePost;
	_color = color;
}

void Flash::operator()(BlinkStickDevice& inDevice, int channel, int index)
{
	inDevice.setColor(channel,index, 0,0,0);
	boost::this_thread::sleep(boost::posix_time::microseconds(_timePre*1000));
	inDevice.setColor(channel,index, _color);
	boost::this_thread::sleep(boost::posix_time::microseconds(_timeHigh*1000));
	inDevice.setColor(channel,index, 0,0,0);
	boost::this_thread::sleep(boost::posix_time::microseconds(_timePost*1000));
}

Pulse::Pulse(int timeRise, int numSteps, BlinkStickColor color)
	:_riseCommand(timeRise,numSteps,BlinkStickColor(0,0,0),color),
	 _fallCommand(timeRise,numSteps,color,BlinkStickColor(0,0,0))
{
}

void Pulse::operator()(BlinkStickDevice& inDevice, int channel, int index)
{
	_riseCommand(inDevice,channel,index);
	_fallCommand(inDevice,channel,index);
}
