#pragma once
#include "App.xaml.g.h"

namespace winrt::metameter_desktop::implementation
{
    struct App : AppT<App>
    {
        App();
        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

        void StartBleDeviceWatcher();
        void StopBleDeviceWatcher();

        fire_and_forget DeviceWatcher_Added(Windows::Devices::Enumeration::DeviceWatcher sender, Windows::Devices::Enumeration::DeviceInformation deviceInfo);
        fire_and_forget App::Characteristic_MeasurementChanged(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic const&, Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs args);
        fire_and_forget App::Characteristic_ModeChanged(Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic const&, Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs args);
        fire_and_forget App::ConnectionStatusChanged(Windows::Devices::Bluetooth::BluetoothLEDevice sender, Windows::Foundation::IUnknown);


        Windows::Devices::Enumeration::DeviceWatcher deviceWatcher{ nullptr };
        event_token deviceWatcherAddedToken;
        event_token deviceWatcherUpdatedToken;
        event_token deviceWatcherRemovedToken;
        event_token deviceWatcherEnumerationCompletedToken;
        event_token deviceWatcherStoppedToken;
        event_token connectionStatusToken;

        Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic measurementCharacteristic{ nullptr };
        Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic modeCharacteristic{ nullptr };
        event_token notificationsToken;
        event_token indicationsToken;



        winrt::Windows::UI::Xaml::Window window{ nullptr };
    };
}
