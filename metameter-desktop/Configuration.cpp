#include "pch.h"
#include "Configuration.h"
#include "MultimeterDevice.h"

using namespace winrt;
using namespace Windows::Devices::Bluetooth;
using namespace metameter_desktop;
using namespace metameter_desktop::implementation;

metameter_desktop::MultimeterDevice State::current_device{ nullptr, 0.f, Mode::None, nullptr };

