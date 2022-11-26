#ifndef IMAPP_HPP
#define IMAPP_HPP

#include <imgui.h>

#include "ImApp/Utils.hpp"

namespace ImApp
{
	// Enums
	enum class Cond
	{
		Always,       // No condition (always set the variable)
		FirstUseEver, // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
	};

	enum class AppFlag
	{
		MainWindow_NoResize   = (1 << 0),
		MainWindow_NoSaveSize = (1 << 1),
	};
	IMAPP_DECLARE_FLAGS(AppFlags, AppFlag)

	enum class MainWindowContentFlag
	{
		MenuBar = (1 << 0)
	};
	IMAPP_DECLARE_FLAGS(MainWindowContentFlags, MainWindowContentFlag)

	// High level api
	template <class F>
	int Run(const char* mainWindowTitle, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)())); // UpdateFunc can return a bool to indicate to close

	template <class F>
	int Run(const char* mainWindowTitle, AppFlags appFlags, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)())); // UpdateFunc can return a bool to indicate to close

	// Low level api
	bool Init(const char* mainWindowTitle, AppFlags appFlags = AppFlags::None()) noexcept;

	template <class F>
	void Update(F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()));

	template <class F>
	void Update(bool* open, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()));

	int Terminate() noexcept;

	// Configuration api
	void SetNextMainWindowSize(int witdh, int height, Cond cond = Cond::Always) noexcept;

	// Main window content api
	bool BeginMainWindowContent(MainWindowContentFlags mainWindowContentFlags = MainWindowContentFlags::None()) noexcept;
	void EndMainWindowContent() noexcept;
}

#include <ImApp/ImApp.inl>

#endif
