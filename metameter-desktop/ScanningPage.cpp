#include "pch.h"
#include "ScanningPage.h"
#if __has_include("ScanningPage.g.cpp")
#include "ScanningPage.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::metameter_desktop::implementation
{
    int32_t ScanningPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void ScanningPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
