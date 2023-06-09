﻿#include "pch.h"

#include "App.h"
#include "MainPage.h"
#include "ScanningPage.h"
#include "Configuration.h"
#include <sstream>

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace metameter_desktop;
using namespace metameter_desktop::implementation;

/// <summary>
/// Creates the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    Suspending({ this, &App::OnSuspending });

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
    // Additional properties we would like about the device.
    // Property strings are documented here https://msdn.microsoft.com/en-us/library/windows/desktop/ff521659(v=vs.85).aspx
    auto requestedProperties = single_threaded_vector<hstring>({ L"System.Devices.Aep.DeviceAddress", L"System.Devices.Aep.IsConnected", L"System.Devices.Aep.Bluetooth.Le.IsConnectable" });

    // BT_Code: Example showing paired and non-paired in a single query.
    hstring aqsAllBluetoothLEDevices = L"(System.Devices.Aep.ProtocolId:=\"{bb7bb05e-5972-42b5-94fc-76eaa7084d49}\")";
    deviceWatcher =
        DeviceInformation::CreateWatcher(
            aqsAllBluetoothLEDevices,
            requestedProperties,
            DeviceInformationKind::AssociationEndpoint);
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const& e)
{
    // set window member variable
    window = Window::Current();
    StartBleDeviceWatcher();
    Frame rootFrame{ nullptr };
    auto content = Window::Current().Content();
    if (content)
    {
        rootFrame = content.try_as<Frame>();
    }
    ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::PreferredLaunchViewSize);
    ApplicationView::PreferredLaunchViewSize({ 320, 560 });
    ApplicationView::GetForCurrentView().SetPreferredMinSize({ 320, 560 });
    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (rootFrame == nullptr)
    {
        // Create a Frame to act as the navigation context and associate it with
        // a SuspensionManager key
        rootFrame = Frame();

        rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });

        if (e.PreviousExecutionState() == ApplicationExecutionState::Terminated)
        {
            // Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete
        }

        if (e.PrelaunchActivated() == false)
        {
            if (rootFrame.Content() == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame.Navigate(xaml_typename<metameter_desktop::ScanningPage>(), box_value(e.Arguments()));
            }
            // Place the frame in the current Window
            Window::Current().Content(rootFrame);
            // Ensure the current window is active
            Window::Current().Activate();

        }
    }
    else
    {
        if (e.PrelaunchActivated() == false)
        {
            if (rootFrame.Content() == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame.Navigate(xaml_typename<metameter_desktop::ScanningPage>(), box_value(e.Arguments()));
            }
            // Ensure the current window is active
            Window::Current().Activate();
        }
    }
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
{
    // Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(IInspectable const&, NavigationFailedEventArgs const& e)
{
    throw hresult_error(E_FAIL, hstring(L"Failed to load Page ") + e.SourcePageType().Name);
}

void App::StartBleDeviceWatcher()
{
    // Register event handlers before starting the watcher.
    deviceWatcherAddedToken = deviceWatcher.Added({ get_weak(), &App::DeviceWatcher_Added });
    deviceWatcherUpdatedToken = deviceWatcher.Updated([](DeviceWatcher, DeviceInformationUpdate) {});
    deviceWatcherRemovedToken = deviceWatcher.Removed([](DeviceWatcher, DeviceInformationUpdate) {});
    deviceWatcherEnumerationCompletedToken = deviceWatcher.EnumerationCompleted([](DeviceWatcher, IInspectable const&) {});
    if (deviceWatcher.Status() == DeviceWatcherStatus::Stopping)
    {
        deviceWatcherStoppedToken = deviceWatcher.Stopped([this](DeviceWatcher, IInspectable const&) {
            deviceWatcher.Stopped(deviceWatcherStoppedToken);
            OutputDebugStringW(L"Started Device Enumeration\n");
            deviceWatcher.Start();
        });
    }
    else
    {
        // Start the watcher. Active enumeration is limited to approximately 30 seconds.
        // This limits power usage and reduces interference with other Bluetooth activities.
        // To monitor for the presence of Bluetooth LE devices for an extended period,
        // use the BluetoothLEAdvertisementWatcher runtime class. See the BluetoothAdvertisement
        // sample for an example.
        OutputDebugStringW(L"Started Device Enumeration\n");
        deviceWatcher.Start();
    }

}

void App::StopBleDeviceWatcher()
{
    if (deviceWatcher != nullptr)
    {
        // Unregister the event handlers.
        deviceWatcher.Added(deviceWatcherAddedToken);
        deviceWatcher.Updated(deviceWatcherUpdatedToken);
        deviceWatcher.Removed(deviceWatcherRemovedToken);
        deviceWatcher.EnumerationCompleted(deviceWatcherEnumerationCompletedToken);

        // Stop the watcher.
        if (deviceWatcher.Status() == DeviceWatcherStatus::Stopping)
        {
            deviceWatcher.Stopped(deviceWatcherStoppedToken);
        }
        else if (deviceWatcher.Status() != DeviceWatcherStatus::Stopped)
        {
            deviceWatcher.Stop();
        }
    }
}

fire_and_forget App::DeviceWatcher_Added(DeviceWatcher sender, DeviceInformation deviceInfo)
{
    // We must update the collection on the UI thread because the collection is databound to a UI element.
    auto lifetime = get_strong();
    co_await resume_foreground(window.Dispatcher());
    // Protect against race condition if the task runs after the app stopped the deviceWatcher.
    if (sender == deviceWatcher)
    {
        OutputDebugStringW((L"Found " + deviceInfo.Id() + deviceInfo.Name() + L"\n").c_str());
        // Get a specific Bluetooth LE object so we can check the GATT services available
        if (deviceInfo.Name() == L"Metameter")
        {
            BluetoothLEDevice bluetoothLeDevice = co_await BluetoothLEDevice::FromIdAsync(deviceInfo.Id());
            if (bluetoothLeDevice != nullptr)
            {
                GattDeviceServicesResult result = co_await bluetoothLeDevice.GetGattServicesAsync(BluetoothCacheMode::Uncached);
                OutputDebugStringW((L"GATT STATUS: " + to_hstring(static_cast<int>(result.Status())) + L" Device " + deviceInfo.Id() + deviceInfo.Name() + L"\n").c_str());
                if (result.Status() == GattCommunicationStatus::Success)
                {
                    StopBleDeviceWatcher();
                    IVectorView<GattDeviceService> services = result.Services();
                    for (auto&& service : services)
                    {
                        OutputDebugStringW((L"SERVICE " + to_hstring(service.Uuid()) + L" Device " + deviceInfo.Id() + deviceInfo.Name() + L"\n").c_str());
                        if (service.Uuid() == guid{ 0x3E83926C, 0xA0AA, 0x4261, { 0x90, 0xD4, 0xC0, 0x95, 0x85, 0xC3, 0xE7, 0x3B } })
                        {
                            State::current_device.Device(bluetoothLeDevice);
                            connectionStatusToken = State::current_device.Device().ConnectionStatusChanged({ get_weak(), &App::ConnectionStatusChanged });
                            GattCharacteristicsResult result = co_await service.GetCharacteristicsAsync(BluetoothCacheMode::Uncached);
                            if (result.Status() == GattCommunicationStatus::Success)
                            {
                                IVectorView<GattCharacteristic> characteristics{ nullptr };
                                characteristics = result.Characteristics();
                                for (auto&& characteristic : characteristics)
                                {
                                    OutputDebugStringW((L"CHARACTERISTIC " + to_hstring(characteristic.Uuid()) + L"\n").c_str());
                                    if (characteristic.Uuid() == guid{ 0x86F85C5B, 0x9BD8, 0x4151, { 0xBF, 0x64, 0x03, 0xBF, 0x16, 0xB9, 0xFE, 0x81 } })
                                    {
                                        GattCommunicationStatus status = co_await characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Notify);
                                        if (status == GattCommunicationStatus::Success)
                                        {
                                            measurementCharacteristic = characteristic;
                                            measurementToken = measurementCharacteristic.ValueChanged({ get_weak(), &App::Characteristic_MeasurementChanged });
                                            GattReadResult result = co_await measurementCharacteristic.ReadValueAsync(BluetoothCacheMode::Uncached);
                                            if (result.Status() == GattCommunicationStatus::Success)
                                                State::current_device.Measurement(*(reinterpret_cast<float*>(result.Value().data())));
                                            // co_return;
                                        }
                                    }
                                    else if (characteristic.Uuid() == guid{ 0x225A22E1, 0xE572, 0x407A, { 0x81, 0x44, 0x28, 0x3D, 0xCD, 0x49, 0x30, 0x3C } }) 
                                    {
                                        GattCommunicationStatus status = co_await characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Indicate);
                                        if (status == GattCommunicationStatus::Success)
                                        {
                                            State::current_device.ModeCharacteristic(characteristic);
                                            modeToken = State::current_device.ModeCharacteristic().ValueChanged({get_weak(), &App::Characteristic_ModeChanged});
                                            GattReadResult result = co_await State::current_device.ModeCharacteristic().ReadValueAsync(BluetoothCacheMode::Uncached);
                                            if (result.Status() == GattCommunicationStatus::Success)
                                                State::current_device.Mode(static_cast<winrt::metameter_desktop::Mode>(*(reinterpret_cast<char*>(result.Value().data()))));
                                            // co_return;
                                        }
                                    }
                                    else if (characteristic.Uuid() == guid{ 0x70CB22BF, 0xEE3C, 0x4856, { 0xB8, 0x1F, 0xFE, 0x50, 0x50, 0x8B, 0x43, 0xDD } })
                                    {
                                        GattCommunicationStatus status = co_await characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Indicate);
                                        if (status == GattCommunicationStatus::Success)
                                        {
                                            continuityCharacteristic = characteristic;
                                            continuityToken = continuityCharacteristic.ValueChanged({ get_weak(), &App::Characteristic_ContinuityChanged });
                                            GattReadResult result = co_await continuityCharacteristic.ReadValueAsync(BluetoothCacheMode::Uncached);
                                            if (result.Status() == GattCommunicationStatus::Success)
                                                State::current_device.Continuity(static_cast<winrt::metameter_desktop::Continuity>(*(reinterpret_cast<char*>(result.Value().data()))));
                                            // co_return;
                                        }
                                    }
                                }
                                Window::Current().Content().try_as<Frame>().Navigate(xaml_typename<metameter_desktop::MainPage>(), winrt::Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo::DrillInNavigationTransitionInfo());
                            }
                        }
                    }
                }
            }
        }
    }
}

fire_and_forget App::Characteristic_MeasurementChanged(GattCharacteristic const&, GattValueChangedEventArgs args)
{
    auto lifetime = get_strong();
    co_await resume_foreground(window.Dispatcher());
    float measurement = *(reinterpret_cast<float*>(args.CharacteristicValue().data()));
    std::wostringstream wostringstream;
    wostringstream << measurement << L"V\n";
    OutputDebugString(wostringstream.str().c_str());
    State::current_device.Measurement(measurement);
    co_return;
}

fire_and_forget App::Characteristic_ModeChanged(GattCharacteristic const&, GattValueChangedEventArgs args)
{
    auto lifetime = get_strong();
    co_await resume_foreground(window.Dispatcher());
    winrt::metameter_desktop::Mode mode = static_cast<winrt::metameter_desktop::Mode>(*(reinterpret_cast<char*>(args.CharacteristicValue().data())));
    std::wostringstream wostringstream;
    wostringstream << L"Mode: " << static_cast<int>(mode) << L"\n";
    OutputDebugString(wostringstream.str().c_str());

    // update measurement as well
    GattReadResult result = co_await measurementCharacteristic.ReadValueAsync();
    if (result.Status() == GattCommunicationStatus::Success)
    {
        float measurement = *(reinterpret_cast<float*>(result.Value().data()));
        State::current_device.Measurement(measurement);
    }
    State::current_device.Mode(mode);
    co_return;
}

fire_and_forget App::Characteristic_ContinuityChanged(GattCharacteristic const&, GattValueChangedEventArgs args)
{
    auto lifetime = get_strong();
    co_await resume_foreground(window.Dispatcher());
    winrt::metameter_desktop::Continuity continuity = static_cast<winrt::metameter_desktop::Continuity>(*(reinterpret_cast<char*>(args.CharacteristicValue().data())));
    std::wostringstream wostringstream;
    wostringstream << L"Continuity: " << static_cast<int>(continuity) << L"\n";
    OutputDebugString(wostringstream.str().c_str());

    // update measurement as well
    GattReadResult result = co_await measurementCharacteristic.ReadValueAsync();
    if (result.Status() == GattCommunicationStatus::Success)
    {
        float measurement = *(reinterpret_cast<float*>(result.Value().data()));
        State::current_device.Measurement(measurement);
    }
    State::current_device.Continuity(continuity);
    co_return;

}


fire_and_forget App::ConnectionStatusChanged(Windows::Devices::Bluetooth::BluetoothLEDevice sender, Windows::Foundation::IUnknown)
{
    auto lifetime = get_strong();
    co_await resume_foreground(window.Dispatcher());
    if (sender != State::current_device.Device())
        co_return;

    if (State::current_device.Device().ConnectionStatus() == BluetoothConnectionStatus::Disconnected)
    {
        OutputDebugString(L"Disconnected!\n");
        State::current_device.Device(nullptr);
        State::current_device.ModeCharacteristic(nullptr);
        StartBleDeviceWatcher();
        Window::Current().Content().try_as<Frame>().Navigate(xaml_typename<metameter_desktop::ScanningPage>(), winrt::Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo::DrillInNavigationTransitionInfo());
    }
}