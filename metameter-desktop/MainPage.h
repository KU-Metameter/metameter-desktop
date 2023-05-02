#pragma once

#include "MainPage.g.h"

namespace winrt::metameter_desktop::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        fire_and_forget UpdateMeasurement();
        void UpdateMode();
        void UpdateContinuity();
        void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
        void OnPropertyChanged(IInspectable sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs e);
        void OnDeviceFound();
        void OnModeUpdate();
        void OnContinuityUpdate();
        fire_and_forget ModeButtonClicked(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs const& e);
        fire_and_forget CSVButtonClicked(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs const& e);

    private:
        Windows::Storage::StorageFile m_tempFile{ nullptr };
        std::chrono::time_point<std::chrono::system_clock> m_startTime;
        bool m_recording{ false };
    };
}

namespace winrt::metameter_desktop::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
