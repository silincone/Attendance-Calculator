#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::ACalc::implementation
{
	MainWindow::MainWindow()
	{
		m_subjects = winrt::single_threaded_observable_vector<winrt::ACalc::Subjects>();
	}

	winrt::Windows::Foundation::IAsyncAction MainWindow::MainGrid_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{
		winrt::apartment_context ui_thread;
		co_await ui_thread;

		Display_ListView().Focus(winrt::Microsoft::UI::Xaml::FocusState::Programmatic);
	}

	winrt::Windows::Foundation::IAsyncAction MainWindow::AddNewSubButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		winrt::Microsoft::UI::Xaml::Controls::ContentDialog addNewSubjectDialog;

		addNewSubjectDialog.Title(box_value(L"Add New Subject"));

		Controls::Grid content;

		Controls::ColumnDefinition col1, col2;
		col1.Width({ 0, GridUnitType::Auto });
		col2.Width({ 1, GridUnitType::Star });
		content.ColumnDefinitions().Append(col1);
		content.ColumnDefinitions().Append(col2);

		Controls::RowDefinition row1, row2, row3;
		row1.Height({ 0, GridUnitType::Auto });
		row2.Height({ 0, GridUnitType::Auto });
		row3.Height({ 0, GridUnitType::Auto });
		content.RowDefinitions().Append(row1);
		content.RowDefinitions().Append(row2);
		content.RowDefinitions().Append(row3);

		Controls::TextBlock subjectName_TextBlock;
		subjectName_TextBlock.Text(L"Subject Name:");
		subjectName_TextBlock.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(subjectName_TextBlock, 0);
		content.SetRow(subjectName_TextBlock, 0);

		Controls::TextBox subjectName_TextBox;
		subjectName_TextBox.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(subjectName_TextBox, 1);
		content.SetRow(subjectName_TextBox, 0);

		Controls::TextBlock classesConducted_TextBlock;
		classesConducted_TextBlock.Text(L"Classes Conducted:");
		classesConducted_TextBlock.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesConducted_TextBlock, 0);
		content.SetRow(classesConducted_TextBlock, 1);

		Controls::NumberBox classesConducted_NumberBox;
		classesConducted_NumberBox.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesConducted_NumberBox, 1);
		content.SetRow(classesConducted_NumberBox, 1);

		Controls::TextBlock classesAttended_TextBlock;
		classesAttended_TextBlock.Text(L"Classes Attended:");
		classesAttended_TextBlock.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesAttended_TextBlock, 0);
		content.SetRow(classesAttended_TextBlock, 2);

		Controls::NumberBox classesAttended_NumberBox;
		classesAttended_NumberBox.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesAttended_NumberBox, 1);
		content.SetRow(classesAttended_NumberBox, 2);

		content.Children().Append(subjectName_TextBlock);
		content.Children().Append(subjectName_TextBox);
		content.Children().Append(classesConducted_TextBlock);
		content.Children().Append(classesConducted_NumberBox);
		content.Children().Append(classesAttended_TextBlock);
		content.Children().Append(classesAttended_NumberBox);

		addNewSubjectDialog.Content(content);
		addNewSubjectDialog.PrimaryButtonText(L"Add");
		addNewSubjectDialog.CloseButtonText(L"Cancel");
		addNewSubjectDialog.XamlRoot(MainGrid().XamlRoot());

		addNewSubjectDialog.PrimaryButtonClick([&](Controls::ContentDialog const&, Controls::ContentDialogButtonClickEventArgs const& args)
			{
				args.Cancel(true);

				subjectName_TextBox.Description({});
				classesConducted_NumberBox.Description({});
				classesAttended_NumberBox.Description({});

				winrt::hstring subjectName = subjectName_TextBox.Text();
				double classesConducted = classesConducted_NumberBox.Value();
				double classesAttended = classesAttended_NumberBox.Value();

				bool isValid = true;

				std::uint16_t conducted{ static_cast<std::uint16_t>(classesConducted) }, attended{ static_cast<std::uint16_t>(classesAttended) };

				if (subjectName.empty())
				{
					subjectName_TextBox.Description(box_value(L"Subject name cannot be empty."));
					isValid = false;
				}

				for (auto subject : m_subjects)
				{
					if (subject.SubjectName() == subjectName)
					{
						subjectName_TextBox.Description(box_value(L"Subject (" + to_hstring(subjectName) + L") already present!"));
						isValid = false;
					}
				}

				if (std::isnan(classesConducted) || classesConducted < 0 || classesConducted > std::numeric_limits<std::uint16_t>::max() || classesConducted != static_cast<int>(classesConducted))
				{
					classesConducted_NumberBox.Description(box_value(L"Please enter a valid number."));
					isValid = false;
				}

				if (std::isnan(classesAttended) || classesAttended < 0 || classesAttended > std::numeric_limits<std::uint16_t>::max() || classesAttended != static_cast<int>(classesAttended))
				{
					classesAttended_NumberBox.Description(box_value(L"Please enter a valid number."));
					isValid = false;
				}

				if (isValid && (attended > conducted))
				{
					classesAttended_NumberBox.Description(box_value(L"Classes attended cannot be more \nthan classes conducted."));
					isValid = false;
				}

				if (isValid)
				{
					args.Cancel(false);
				}

			});

		Controls::ContentDialogResult result = co_await addNewSubjectDialog.ShowAsync();

		if (result == Controls::ContentDialogResult::Primary)
		{
			winrt::hstring subjectName = subjectName_TextBox.Text();
			std::uint16_t classesAttended = static_cast<std::uint16_t>(classesAttended_NumberBox.Value());
			std::uint16_t classesConducted = static_cast<std::uint16_t>(classesConducted_NumberBox.Value());

			winrt::ACalc::Subjects m_sub = winrt::make<winrt::ACalc::implementation::Subjects>(subjectName, classesAttended, classesConducted);
			m_subjects.Append(m_sub);
		}
	}

	void MainWindow::Subjects_ListView_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		Subjects_ListView().ItemsSource(m_subjects);
	}
	
	void MainWindow::Display_ListView_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		Display_ListView().ItemsSource(m_subjects);
	}
	
	void MainWindow::DeleteMenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto item = sender.as<FrameworkElement>().DataContext();
		auto subject = item.as<ACalc::Subjects>();

		for (std::uint32_t i = 0; i < m_subjects.Size(); ++i)
		{
			if (m_subjects.GetAt(i) == subject)
			{
				m_subjects.RemoveAt(i);
				return;
			}
		}
	}
	
	winrt::Windows::Foundation::IAsyncAction MainWindow::EditMenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto item = sender.as<FrameworkElement>().DataContext();
		auto subject = item.as<ACalc::Subjects>();

		winrt::Microsoft::UI::Xaml::Controls::ContentDialog addNewSubjectDialog;

		addNewSubjectDialog.Title(box_value(L"Add New Subject"));

		Controls::Grid content;

		Controls::ColumnDefinition col1, col2;
		col1.Width({ 0, GridUnitType::Auto });
		col2.Width({ 1, GridUnitType::Star });
		content.ColumnDefinitions().Append(col1);
		content.ColumnDefinitions().Append(col2);

		Controls::RowDefinition row1, row2, row3;
		row1.Height({ 0, GridUnitType::Auto });
		row2.Height({ 0, GridUnitType::Auto });
		row3.Height({ 0, GridUnitType::Auto });
		content.RowDefinitions().Append(row1);
		content.RowDefinitions().Append(row2);
		content.RowDefinitions().Append(row3);

		Controls::TextBlock subjectName_TextBlock;
		subjectName_TextBlock.Text(L"Subject Name:");
		subjectName_TextBlock.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(subjectName_TextBlock, 0);
		content.SetRow(subjectName_TextBlock, 0);

		Controls::TextBox subjectName_TextBox;
		subjectName_TextBox.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(subjectName_TextBox, 1);
		content.SetRow(subjectName_TextBox, 0);

		Controls::TextBlock classesConducted_TextBlock;
		classesConducted_TextBlock.Text(L"Classes Conducted:");
		classesConducted_TextBlock.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesConducted_TextBlock, 0);
		content.SetRow(classesConducted_TextBlock, 1);

		Controls::NumberBox classesConducted_NumberBox;
		classesConducted_NumberBox.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesConducted_NumberBox, 1);
		content.SetRow(classesConducted_NumberBox, 1);

		Controls::TextBlock classesAttended_TextBlock;
		classesAttended_TextBlock.Text(L"Classes Attended:");
		classesAttended_TextBlock.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesAttended_TextBlock, 0);
		content.SetRow(classesAttended_TextBlock, 2);

		Controls::NumberBox classesAttended_NumberBox;
		classesAttended_NumberBox.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		content.SetColumn(classesAttended_NumberBox, 1);
		content.SetRow(classesAttended_NumberBox, 2);

		content.Children().Append(subjectName_TextBlock);
		content.Children().Append(subjectName_TextBox);
		content.Children().Append(classesConducted_TextBlock);
		content.Children().Append(classesConducted_NumberBox);
		content.Children().Append(classesAttended_TextBlock);
		content.Children().Append(classesAttended_NumberBox);

		addNewSubjectDialog.Content(content);
		addNewSubjectDialog.PrimaryButtonText(L"Add");
		addNewSubjectDialog.CloseButtonText(L"Cancel");
		addNewSubjectDialog.XamlRoot(MainGrid().XamlRoot());

		addNewSubjectDialog.PrimaryButtonClick([&](Controls::ContentDialog const&, Controls::ContentDialogButtonClickEventArgs const& args)
			{
				args.Cancel(true);

				subjectName_TextBox.Description({});
				classesConducted_NumberBox.Description({});
				classesAttended_NumberBox.Description({});

				winrt::hstring subjectName = subjectName_TextBox.Text();
				double classesConducted = classesConducted_NumberBox.Value();
				double classesAttended = classesAttended_NumberBox.Value();

				bool isValid = true;

				std::uint16_t conducted{ static_cast<std::uint16_t>(classesConducted) }, attended{ static_cast<std::uint16_t>(classesAttended) };

				if (subjectName.empty())
				{
					subjectName_TextBox.Description(box_value(L"Subject name cannot be empty."));
					isValid = false;
				}

				for (auto subject : m_subjects)
				{
					if (subject.SubjectName() == subjectName)
					{
						subjectName_TextBox.Description(box_value(L"Subject (" + to_hstring(subjectName) + L") already present!"));
						isValid = false;
					}
				}

				if (std::isnan(classesConducted) || classesConducted < 0 || classesConducted > std::numeric_limits<std::uint16_t>::max() || classesConducted != static_cast<int>(classesConducted))
				{
					classesConducted_NumberBox.Description(box_value(L"Please enter a valid number."));
					isValid = false;
				}

				if (std::isnan(classesAttended) || classesAttended < 0 || classesAttended > std::numeric_limits<std::uint16_t>::max() || classesAttended != static_cast<int>(classesAttended))
				{
					classesAttended_NumberBox.Description(box_value(L"Please enter a valid number."));
					isValid = false;
				}

				if (isValid && (attended > conducted))
				{
					classesAttended_NumberBox.Description(box_value(L"Classes attended cannot be more \nthan classes conducted."));
					isValid = false;
				}

				if (isValid)
				{
					args.Cancel(false);
				}
			});

		Controls::ContentDialogResult result = co_await addNewSubjectDialog.ShowAsync();

		if (result == Controls::ContentDialogResult::Primary)
		{
			winrt::hstring subjectName = subjectName_TextBox.Text();
			std::uint16_t classesAttended = static_cast<std::uint16_t>(classesAttended_NumberBox.Value());
			std::uint16_t classesConducted = static_cast<std::uint16_t>(classesConducted_NumberBox.Value());

			subject.ClassesAttended(classesAttended);
			subject.ClassesConducted(classesConducted);
		}
	}
	
	winrt::Windows::Foundation::IAsyncAction MainWindow::EditDesiredPercentage_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{
		Controls::ContentDialog editDesiredPercentDialog;

		editDesiredPercentDialog.Title(box_value(L"Enter Desired Percentage"));

		Controls::Grid content;

		Controls::ColumnDefinition col1, col2;
		col1.Width({ 0, GridUnitType::Auto });
		col2.Width({ 1, GridUnitType::Star });
		content.ColumnDefinitions().Append(col1);
		content.ColumnDefinitions().Append(col2);

		Controls::RowDefinition row1, row2, row3;
		row1.Height({ 0, GridUnitType::Auto });
		content.RowDefinitions().Append(row1);
		
		Controls::TextBlock desiredPercent_TextBlock;
		desiredPercent_TextBlock.Text(L"Desired Percentage: ");
		desiredPercent_TextBlock.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		Controls::Grid::SetColumn(desiredPercent_TextBlock, 0);
		Controls::Grid::SetRow(desiredPercent_TextBlock, 0);

		Controls::NumberBox desiredPercent_NumberBox;
		desiredPercent_NumberBox.Value(std::stod(to_string(DesiredPercent_TextBlock().Text())));
		desiredPercent_NumberBox.Margin(winrt::Microsoft::UI::Xaml::ThicknessHelper::FromLengths(10.0, 10.0, 10.0, 10.0));
		Controls::Grid::SetColumn(desiredPercent_NumberBox, 1);
		Controls::Grid::SetRow(desiredPercent_NumberBox, 0);

		content.Children().Append(desiredPercent_TextBlock);
		content.Children().Append(desiredPercent_NumberBox);

		editDesiredPercentDialog.Content(content);
		editDesiredPercentDialog.PrimaryButtonText(L"Save Changes");
		editDesiredPercentDialog.SecondaryButtonText(L"Cancel");
		editDesiredPercentDialog.XamlRoot(MainGrid().XamlRoot());

		editDesiredPercentDialog.PrimaryButtonClick([&](Controls::ContentDialog const&, Controls::ContentDialogButtonClickEventArgs const& args)
			{
				args.Cancel(true);

				desiredPercent_NumberBox.Description({});

				double desiredPercent = desiredPercent_NumberBox.Value();

				bool isValid = true;

				if (desiredPercent < static_cast<double>(25.0) || desiredPercent > static_cast<double>(100.0))
				{
					desiredPercent_NumberBox.Description(box_value(L"Please enter a percentage in the \nrange, (25, 100)"));
					isValid = false;
				}

				if (isValid)
				{
					args.Cancel(false);
				}
			}
		);

		Controls::ContentDialogResult result = co_await editDesiredPercentDialog.ShowAsync();

		if (result == Controls::ContentDialogResult::Primary)
		{
			DesiredPercent_TextBlock().Text(to_hstring(desiredPercent_NumberBox.Value()));
		}
	}
}