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
            searchStatus().Text(L"0V");
        State::current_device.PropertyChanged({ get_weak(), &MainPage::OnPropertyChanged });
    }

    void MainPage::OnPropertyChanged(IInspectable sender, PropertyChangedEventArgs e)
    {
        if (e.PropertyName() == L"Device")
            OnDeviceFound();
        else if (e.PropertyName() == L"Voltage")
            OnVoltageUpdate();
    }

    void MainPage::OnDeviceFound()
    {
        if (State::current_device.Device() != nullptr)
            searchStatus().Text(L"0V");
    }

    void MainPage::OnVoltageUpdate()
    {
        if (State::current_device.Device() != nullptr)
        {
            std::wostringstream wostringstream;
            wostringstream << State::current_device.Voltage() << L"V";
            searchStatus().Text(wostringstream.str());
        }
    }
}
