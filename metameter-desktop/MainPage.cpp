#include "pch.h"
#include <sstream>
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "Configuration.h"

using namespace winrt;
using namespace metameter_desktop;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
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

    void MainPage::UpdateMeasurement()
    {
        if (State::current_device.Device() != nullptr)
        {
            wchar_t out[20];
            if (isinf(State::current_device.Measurement()))
                swprintf(out, 20, L"Out of Range");
            else
                swprintf(out, 20, L"%.2f", State::current_device.Measurement());
            measurement().Text(winrt::to_hstring(out));
        }
    }

    void MainPage::UpdateMode()
    {
        if (State::current_device.Device() != nullptr)
        {
            hstring mode_str;
            switch (State::current_device.Mode())
            {
            case Mode::Milliamps:
                mode_str = L"Milliamps";
                break;
            case Mode::Diode:
                continuity().Visibility(Visibility::Visible);
            case Mode::Volts:
                mode_str = L"Volts";
                break;
            case Mode::Ohms:
                mode_str = L"Ohms";
                break;
            case Mode::Amps:
                mode_str = L"Amps";
                break;
            default:
                mode_str = L"";
                break;
            }
            mode().Text(mode_str);
            if (State::current_device.Mode() != Mode::Diode)
            {
                continuity().Visibility(Visibility::Collapsed);
                continuity().Text(L"");
            }
        }
    }

    void MainPage::UpdateContinuity()
    {
        if (State::current_device != nullptr && State::current_device.Mode() == Mode::Diode)
        {
            hstring continuity_string;
            switch (State::current_device.Continuity())
            {
            case Continuity::Discontinuous:
                continuity_string = L"Probably not a Diode";
                mode().Text(L"Volts");
                break;
            case Continuity::Cont_Diode:
                continuity_string = L"Probably a Diode";
                mode().Text(L"Volts");
                break;
            case Continuity::Continuous:
                continuity_string = L"Probably Continuous";
                mode().Text(L"Ohms");
                break;
            }
            continuity().Text(continuity_string);
        }
    }

    void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        State::current_device.PropertyChanged({ get_weak(), &MainPage::OnPropertyChanged });
        UpdateMeasurement();
        UpdateMode();
    }

    void MainPage::OnPropertyChanged(IInspectable sender, PropertyChangedEventArgs e)
    {
        if (e.PropertyName() == L"Device")
            OnDeviceFound();
        else if (e.PropertyName() == L"Measurement")
            UpdateMeasurement();
        else if (e.PropertyName() == L"Mode")
            OnModeUpdate();
        else if (e.PropertyName() == L"Continuity")
            OnContinuityUpdate();
    }

    void MainPage::OnDeviceFound()
    {
        if (State::current_device.Device() != nullptr)
        {
            UpdateMeasurement();
            UpdateMode();
        }
        else
        {
            // searchStatus().Text(L"Searching for multimeters...");
        }
    }

    void MainPage::OnModeUpdate()
    {
        UpdateMode();
        UpdateMeasurement();
    }

    void MainPage::OnContinuityUpdate()
    {
        UpdateContinuity();
        UpdateMeasurement();
    }

    fire_and_forget MainPage::ModeButtonClicked(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        Button button = sender.as<Button>();
        Mode requestedMode;
        if (button.Name() == L"voltsButton")
            requestedMode = Mode::Volts;
        else if (button.Name() == L"ohmsButton")
            requestedMode = Mode::Ohms;
        else if (button.Name() == L"ampsButton")
            requestedMode = Mode::Amps;
        else if (button.Name() == L"diodeButton")
            requestedMode = Mode::Diode;
        else if (button.Name() == L"milliAmpsButton")
            requestedMode = Mode::Milliamps;
        else
            co_return;

        if (State::current_device.ModeCharacteristic() != nullptr)
        {
            uint8_t encoded = static_cast<uint8_t>(requestedMode);
            Windows::Storage::Streams::DataWriter writer;
            writer.WriteByte(encoded);
            GattCommunicationStatus status = co_await State::current_device.ModeCharacteristic().WriteValueAsync(writer.DetachBuffer());
            if (status == GattCommunicationStatus::Success)
            {
                State::current_device.Mode(requestedMode);
            }
        }
    }
}
