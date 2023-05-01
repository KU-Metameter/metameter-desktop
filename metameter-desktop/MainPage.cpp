#include "pch.h"
#include <sstream>
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "Configuration.h"

using namespace winrt;
using namespace metameter_desktop;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;

namespace winrt::metameter_desktop::implementation
{
    static std::wstring unit = L"V";

    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        if (State::current_device.Device() != nullptr)
        {
            wchar_t out[20];
            swprintf(out, 20, L"%.2f %.2ls", State::current_device.Measurement(), unit.c_str());
            searchStatus().Text(winrt::to_hstring(out));
        }
        State::current_device.PropertyChanged({ get_weak(), &MainPage::OnPropertyChanged });
    }

    void MainPage::OnPropertyChanged(IInspectable sender, PropertyChangedEventArgs e)
    {
        if (e.PropertyName() == L"Device")
            OnDeviceFound();
        else if (e.PropertyName() == L"Measurement")
            OnMeasurementUpdate();
        else if (e.PropertyName() == L"Mode")
            OnModeUpdate();
    }

    void MainPage::OnDeviceFound()
    {
        if (State::current_device.Device() != nullptr)
        {
            wchar_t out[20];
            swprintf(out, 20, L"%.2f %.2ls", State::current_device.Measurement(), unit.c_str());
            searchStatus().Text(winrt::to_hstring(out));
        }
        else
        {
            searchStatus().Text(L"Searching for multimeters...");
        }
    }

    void MainPage::OnMeasurementUpdate()
    {
        if (State::current_device.Device() != nullptr)
        {
            wchar_t out[20];
            swprintf(out, 20, L"%.2f %.2ls", State::current_device.Measurement(), unit.c_str());
            searchStatus().Text(winrt::to_hstring(out));
        }
    }

    void MainPage::OnModeUpdate()
    {
        if (State::current_device.Device() != nullptr)
        {
            switch (State::current_device.Mode())
            {
            case Mode::Milliamps:
                unit = L"mA";
                break;
            case Mode::Diode:
            case Mode::Volts:
                unit = L"V";
                break;
            case Mode::Ohms:
                unit = L"Ω";
                break;
            case Mode::Amps:
                unit = L"A";
                break;
            default:
                unit = L"";
                break;
            }
        }
    }
}
