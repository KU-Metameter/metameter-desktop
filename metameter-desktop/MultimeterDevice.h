#pragma once
#include "pch.h"
#include "MultimeterDevice.g.h"

using namespace winrt::Windows::Devices::Bluetooth;

namespace winrt::metameter_desktop::implementation
{

    struct MultimeterDevice : MultimeterDeviceT<MultimeterDevice>
    {
        MultimeterDevice() = delete;

        MultimeterDevice(BluetoothLEDevice Device, float Measurement, winrt::metameter_desktop::Mode Mode, GenericAttributeProfile::GattCharacteristic ModeCharacteristic);
        BluetoothLEDevice Device();
        void Device(BluetoothLEDevice value);
        float Measurement();
        void Measurement(float value);
        Mode Mode();
        void Mode(winrt::metameter_desktop::Mode const& value);
        GenericAttributeProfile::GattCharacteristic ModeCharacteristic();
        void ModeCharacteristic(GenericAttributeProfile::GattCharacteristic const& value);
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        BluetoothLEDevice m_device;
        float m_measurement;
        winrt::metameter_desktop::Mode m_mode;
        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        GenericAttributeProfile::GattCharacteristic m_modeCharacteristic;
    };
}
namespace winrt::metameter_desktop::factory_implementation
{
    struct MultimeterDevice : MultimeterDeviceT<MultimeterDevice, implementation::MultimeterDevice>
    {
    };
}
