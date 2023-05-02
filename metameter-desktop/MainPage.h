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
        void UpdateMeasurement();
        void UpdateMode();
        void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
        void OnPropertyChanged(IInspectable sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs e);
        void OnDeviceFound();
        void OnModeUpdate();
        fire_and_forget ModeButtonClicked(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::metameter_desktop::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
