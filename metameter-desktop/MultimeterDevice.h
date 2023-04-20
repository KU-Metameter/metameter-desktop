#pragma once
#include "pch.h"
#include "MultimeterDevice.g.h"

using namespace winrt::Windows::Devices::Bluetooth;

namespace winrt::metameter_desktop::implementation
{
    struct MultimeterDevice : MultimeterDeviceT<MultimeterDevice>
    {
        MultimeterDevice() = delete;

        MultimeterDevice(BluetoothLEDevice Device, float Voltage);
        BluetoothLEDevice Device();
        void Device(BluetoothLEDevice value);
        float Voltage();
        void Voltage(float value);
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        BluetoothLEDevice m_device;
        float m_voltage;
        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}
namespace winrt::metameter_desktop::factory_implementation
{
    struct MultimeterDevice : MultimeterDeviceT<MultimeterDevice, implementation::MultimeterDevice>
    {
    };
}
