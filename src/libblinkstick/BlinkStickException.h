#pragma once
#include <exception>
#include <string>


class BlinkStickException: public std::exception
{
public:
	BlinkStickException(const char* inMsg)
	:std::exception(inMsg){}
};

class BlinkStickDeviceOpenException: public BlinkStickException
{
public:
	BlinkStickDeviceOpenException(const char* inMsg)
	:BlinkStickException(inMsg){}
};