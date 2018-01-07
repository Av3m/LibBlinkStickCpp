#pragma once
#include "BlinkStickDevice.h"
#include <vector>

class BlinkStick
{
	static const int blinkstick_vendor_id = 0x20A0;
	static const int blinkstick_product_id = 0x41E5;

public:
	std::vector<BlinkStickDevice*> _mBlinkStickDevices;

	BlinkStick();

	std::vector<BlinkStickDevice*> scanForNewDevices();
	std::vector<BlinkStickDevice*> getAllDevices();

	static std::vector<std::string> getPathsFromDeviceInfo(hid_device_info* info);
};

