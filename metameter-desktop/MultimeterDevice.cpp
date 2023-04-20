#include "pch.h"
#include "MultimeterDevice.h"
#include "MultimeterDevice.g.cpp"

using namespace winrt::Windows::Devices::Bluetooth;

namespace winrt::metameter_desktop::implementation
{
    MultimeterDevice::MultimeterDevice(BluetoothLEDevice Device, float Voltage) : m_device{ Device }, m_voltage{ Voltage }
    {
    }
    BluetoothLEDevice MultimeterDevice::Device()
    {
        return m_device;
    }
    void MultimeterDevice::Device(BluetoothLEDevice value)
    {
        if (m_device != value)
        {
            m_device = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Device" });
        }
    }
    float MultimeterDevice::Voltage()
    {
        return m_voltage;
    }
    void MultimeterDevice::Voltage(float value)
    {
        m_voltage = value;
        m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Voltage" });
    }
    winrt::event_token MultimeterDevice::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }
    void MultimeterDevice::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }
}
