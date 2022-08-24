#ifndef IMAPP_HPP
#define IMAPP_HPP

#include <imgui.h>

#include "ImApp/Utils.hpp"

namespace ImApp
{
	// Flags
	enum class AppFlag
	{
		MainWindow_NoResize = (1 << 0),
	};
	IMAPP_DECLARE_FLAGS(AppFlags, AppFlag)

	// High level api
	template <class F>
	int Run(const char* mainWindowTitle, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)())); // UpdateFunc can return a bool to indicate to close

	// Low level api
	bool Init(const char* mainWindowTitle, AppFlags appFlags = AppFlags::None()) noexcept;

	template <class F>
	void Update(F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()));

	template <class F>
	void Update(bool* open, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()));

	int Terminate() noexcept;

	// Main window content api
	bool BeginMainWindowContent() noexcept;
	void EndMainWindowContent() noexcept;
}

#include <ImApp/ImApp.inl>

#endif
