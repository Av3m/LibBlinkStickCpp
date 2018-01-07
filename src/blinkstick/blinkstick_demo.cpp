#include "blinkstick_demo.h"
#include <BlinkStick.h>
#include <Windows.h>
#include "BlinkStickCommands.h"


int main(int argc, const char** argv)
{
	auto blinkstick = BlinkStick();

	while(true)
	{
		auto new_devices = blinkstick.scanForNewDevices();
		for(auto it=new_devices.begin();it!=new_devices.end();++it)
		{
			(*it)->setNumLeds(4);

			float dim_value = 0.2;
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::black.dim(dim_value),BlinkStickColor::red.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::red.dim(dim_value),BlinkStickColor::blue.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::blue.dim(dim_value),BlinkStickColor::green.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::green.dim(dim_value),BlinkStickColor::red.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::red.dim(dim_value),BlinkStickColor::magenta.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::magenta.dim(dim_value),BlinkStickColor::cyan.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::cyan.dim(dim_value),BlinkStickColor::yellow.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::yellow.dim(dim_value),BlinkStickColor::white.dim(dim_value)));
			(*it)->addCommandAllLeds(LinearGradient(2000,25,BlinkStickColor::white.dim(dim_value),BlinkStickColor::black.dim(dim_value)));



			(*it)->startExecutionSync();
		}

		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
	}
	

	/*for(auto it=devices.begin();it!=devices.end();++it)
	{
		it->setColor(0,0,0,255,0);
		it->setColor(0,1,255,0,0);
	}*/
	
    return 0;
}
