#pragma once

#include "ScanningPage.g.h"

namespace winrt::metameter_desktop::implementation
{
    struct ScanningPage : ScanningPageT<ScanningPage>
    {
        ScanningPage() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::metameter_desktop::factory_implementation
{
    struct ScanningPage : ScanningPageT<ScanningPage, implementation::ScanningPage>
    {
    };
}
