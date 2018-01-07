#include "BlinkStick.h"
#include "BlinkStickDevice.h"
#include <hidapi/hidapi.h>
#include <vector>
#include "BlinkStickException.h"
#include <iostream>

BlinkStick::BlinkStick()
{
	hid_init();
}

std::vector<BlinkStickDevice*> BlinkStick::scanForNewDevices()
{

	std::vector<BlinkStickDevice*> devices;
	hid_device_info* device_info = hid_enumerate(blinkstick_vendor_id,blinkstick_product_id);

	//get array of device paths
	std::vector<std::string> device_paths = getPathsFromDeviceInfo(device_info);

	//remove devices which do not exist anymore
	std::vector<std::vector<BlinkStickDevice*>::iterator> nonExistingDevices;
	for (auto it = _mBlinkStickDevices.begin(); it != _mBlinkStickDevices.end(); ++it)
	{
		if (std::find(device_paths.begin(), device_paths.end(), (*it)->getPath()) == device_paths.end())
		{
			nonExistingDevices.push_back(it);
		}
	}
	for(auto it=nonExistingDevices.begin();it!=nonExistingDevices.end();++it)
	{
		(**it)->stopExecution();
		delete **it;
		**it = NULL;
		_mBlinkStickDevices.erase(*it);
	}

	hid_device_info* current_device_info = device_info;

	//search for new devices
	while(current_device_info)
	{
		bool already_found = false;
		for(auto it=_mBlinkStickDevices.begin();it!=_mBlinkStickDevices.end();++it)
		{
			if ( (*it)->getPath() == std::string(current_device_info->path) )
			{
				already_found = true;
				break;
			}
			
		}

		if ( !already_found)
		{
			try
			{
				devices.push_back(new BlinkStickDevice(*current_device_info));
			}
			catch(BlinkStickException& e)
			{
				std::cout << e.what() << std::endl;
			}
		}

		current_device_info = current_device_info->next;
	}
	_mBlinkStickDevices.insert(_mBlinkStickDevices.end(),devices.begin(),devices.end());
	//hid_free_enumeration(device_info);
	return devices;

}

std::vector<BlinkStickDevice*> BlinkStick::getAllDevices()
{
	return _mBlinkStickDevices;
}

std::vector<std::string> BlinkStick::getPathsFromDeviceInfo(hid_device_info* info)
{
	std::vector<std::string> ret;
	hid_device_info* current_info = info;
	while (current_info)
	{
		ret.push_back(std::string(current_info->path));
		current_info = current_info->next;
	}

	return ret;
}
