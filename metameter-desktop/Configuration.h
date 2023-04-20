#pragma once
#include "pch.h"
#include "MultimeterDevice.h"

using namespace winrt::Windows::Devices::Bluetooth;

namespace winrt::metameter_desktop
{
	struct State
	{ 
		static MultimeterDevice current_device;
	};
	
}