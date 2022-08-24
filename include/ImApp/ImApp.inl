#include "ImApp/ImApp.hpp"

namespace ImApp
{
	namespace details
	{
		template <class>
		struct UpdateLauncher
		{
			template <class F>
			static void Do(bool& open, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()))
			{
				Update(&open, IMAPP_FWD(updateFunc));
			}
		};

		template <>
		struct UpdateLauncher<bool>
		{
			template <class F>
			static void Do(bool& open, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()))
			{
				Update(&open, [&]()
				{
					if (!static_cast<F&&>(updateFunc)()) // No IMAPP_FWD because of capture don't work well with decltype
						open = false;
				});
			}
		};

		bool PollEvents(bool* open) noexcept;
		void BeginFrame() noexcept;
		void EndFrame() noexcept;
	}
}

template <class F>
int ImApp::Run(const char* mainWindowTitle, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()))
{
	return Run(mainWindowTitle, AppFlags::None(), IMAPP_FWD(updateFunc));
}

template <class F>
int ImApp::Run(const char* mainWindowTitle, AppFlags appFlags, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()))
{
	if (Init(mainWindowTitle, appFlags))
	{
		for (bool open = true; open; )
		{
			using UpdateFuncReturnType = decltype(IMAPP_FWD(updateFunc)());
			details::UpdateLauncher<UpdateFuncReturnType>::Do(open, IMAPP_FWD(updateFunc));
		}
	}

	return Terminate();
}

template <class F>
void ImApp::Update(F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()))
{
	Update(nullptr, IMAPP_FWD(updateFunc));
}

template <class F>
void ImApp::Update(bool* open, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()))
{
	if (!details::PollEvents(open))
		return;

	details::BeginFrame();
	IMAPP_FWD(updateFunc)();
	details::EndFrame();
}
