#ifndef IMAPP_HPP
#define IMAPP_HPP

#include <imgui.h>

#define IMAPP_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__) // Allows us to forward without include anything

namespace ImApp
{
	// High level api
	template <class F> int Run(const char* mainWindowTitle, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)())); // UpdateFunc can return a bool to indicate to close

	// Low level api
	bool Init(const char* mainWindowTitle) noexcept;

	template <class F> void Update(F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()));
	template <class F> void Update(bool* open, F&& updateFunc) noexcept(noexcept(IMAPP_FWD(updateFunc)()));

	int Terminate() noexcept;
}

#include <ImApp/ImApp.inl>

#endif
