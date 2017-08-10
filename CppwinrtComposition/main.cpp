#include "pch.h"

using namespace winrt;

using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Controls::Primitives;
using namespace winrt::Windows::UI::Xaml::Data;
using namespace winrt::Windows::UI::Xaml::Hosting;
using namespace winrt::Windows::UI::Xaml::Input;
using namespace winrt::Windows::UI::Xaml::Interop;
using namespace winrt::Windows::UI::Xaml::Markup;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;

struct App : ApplicationT<App>
{
	fire_and_forget OnLaunched(LaunchActivatedEventArgs const&)
	{
		auto const packageFolder = Package::Current().InstalledLocation();
		auto const file = co_await packageFolder.GetFileAsync(L"MainPage.Xaml");
		auto const xaml = co_await FileIO::ReadTextAsync(file);
		auto const uiElement = XamlReader::Load(xaml).as<Page>();

		auto btn = uiElement.FindName(L"Btn").as<ButtonBase>();
		btn.Tapped([btn](auto && ...) 
		{ 
			auto btnVisual = ElementCompositionPreview::GetElementVisual(btn);
			btnVisual.CenterPoint({(float)btn.ActualWidth() / 2,(float)btn.ActualHeight() / 2, 0}); // for rotate
			auto compositor = btnVisual.Compositor();
			auto animation = compositor.CreateScalarKeyFrameAnimation();
			animation.Duration(TimeSpan(std::chrono::seconds(1)));
			animation.InsertKeyFrame(0, 0);
			animation.InsertKeyFrame(1, 360);

			btnVisual.StartAnimation(L"RotationAngleInDegrees", animation);
		});

		auto window = Window::Current();
		window.Content(uiElement);
		window.Activate();
	}

private:
	TextBlock tb = nullptr;
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	Application::Start([](auto &&) { make<App>(); });
}
