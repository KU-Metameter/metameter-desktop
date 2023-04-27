#include "pch.h"
#include "MultimeterDevice.h"
#include "MultimeterDevice.g.cpp"

using namespace winrt::Windows::Devices::Bluetooth;

namespace winrt::metameter_desktop::implementation
{
    MultimeterDevice::MultimeterDevice(BluetoothLEDevice Device, float Measurement, winrt::metameter_desktop::Mode Mode) : m_device{ Device }, m_measurement{ Measurement }, m_mode{ Mode }
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
    float MultimeterDevice::Measurement()
    {
        return m_measurement;
    }
    void MultimeterDevice::Measurement(float value)
    {
        m_measurement = value;
        m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Measurement" });
    }
    winrt::metameter_desktop::Mode MultimeterDevice::Mode()
    {
        return m_mode;
    }
    void MultimeterDevice::Mode(winrt::metameter_desktop::Mode const& value)
    {
        m_mode = value;
        m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Mode" });
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
