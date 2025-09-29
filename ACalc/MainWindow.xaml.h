#pragma once

#include "MainWindow.g.h"

#include "Subjects.h"

namespace winrt::ACalc::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        winrt::Windows::Foundation::IAsyncAction AddNewSubButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void Subjects_ListView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

    private:
        winrt::Windows::Foundation::Collections::IObservableVector<ACalc::Subjects> m_subjects;
    };
}

namespace winrt::ACalc::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
