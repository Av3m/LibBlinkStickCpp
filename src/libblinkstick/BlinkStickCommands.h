#pragma once
#include <Eigen/Eigen>
#include "BlinkStickColor.h"

class BlinkStickDevice;


class BlinkStickCommand
{
public:
	virtual void operator()(BlinkStickDevice& inDevice, int channel, int index) =0;

};



class Wait: public BlinkStickCommand
{
private:
	int _msec;
public:
	Wait(int msec);

	void operator()(BlinkStickDevice& inDevice, int channel, int index);
};


class LinearGradient: public BlinkStickCommand
{
private:
	int _msec;
	int _numSteps;
	BlinkStickColor _colorStart;
	BlinkStickColor _colorEnd;
public:
	LinearGradient(int msec, int num_steps, BlinkStickColor color_start, BlinkStickColor color_end);

	void operator()(BlinkStickDevice& inDevice, int channel, int index);
};


class Flash: public BlinkStickCommand
{
private:
	int _timePre;
	int _timePost;
	int _timeHigh;
	BlinkStickColor _color;
public:
	Flash(int timePre, int timeHigh, int timePost,  BlinkStickColor color);
	void operator()(BlinkStickDevice& inDevice, int channel, int index);
};


class Pulse: public BlinkStickCommand
{
private:
	LinearGradient _riseCommand;
	LinearGradient _fallCommand;
public:
	Pulse(int timeRise, int numSteps, BlinkStickColor color);
	void operator()(BlinkStickDevice& inDevice, int channel, int index);
};
