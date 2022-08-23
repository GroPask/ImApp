#ifndef IMAPP_HPP
#define IMAPP_HPP

#include <imgui.h>

#include <type_traits>

#define IMAPP_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__) // Allows us to forward without include anything

namespace ImApp
{
	// Utils
	template <class E>
	struct FlagsContainer
	{
		using IntegerType = typename std::underlying_type<E>::type;

		static constexpr FlagsContainer None() noexcept { return FlagsContainer{ IntegerType{ 0 } }; }

		FlagsContainer() = default;
		FlagsContainer(const FlagsContainer&) = default;
		FlagsContainer& operator=(const FlagsContainer&) = default;
		~FlagsContainer() = default;

		explicit constexpr FlagsContainer(IntegerType _value) noexcept : m_value(_value) {}
		constexpr FlagsContainer(E _flag) noexcept : m_value(static_cast<IntegerType>(_flag)) {}

		friend constexpr bool operator==(FlagsContainer _flagsLeft, FlagsContainer _flagsRight) noexcept { return (_flagsLeft.m_value == _flagsRight.m_value); }
		friend constexpr bool operator!=(FlagsContainer _flagsLeft, FlagsContainer _flagsRight) noexcept { return (_flagsLeft.m_value != _flagsRight.m_value); }
		friend constexpr FlagsContainer operator|(FlagsContainer _flagsLeft, FlagsContainer _flagsRight) noexcept { return FlagsContainer{ _flagsLeft.m_value | _flagsRight.m_value }; }
		friend constexpr FlagsContainer operator|(FlagsContainer _flagsLeft, E _flagRight) noexcept { return (_flagsLeft | FlagsContainer{ _flagRight }); }
		friend constexpr FlagsContainer operator|(E _flagLeft, FlagsContainer _flagsRight) noexcept { return (FlagsContainer{ _flagLeft } | _flagsRight); }

		constexpr bool Has(E _flagToCheck) const noexcept { return HasOneOf(_flagToCheck); }
		constexpr bool HasOneOf(FlagsContainer _flagsToCheck) const noexcept { return (FlagsContainer{ m_value & _flagsToCheck.m_value } != None()); }

	private:
		IntegerType m_value; // Not initialized with default constructor (as simple integer/enum), use "= MyFlags::None()" if you need a zero initialized flags
	};

	#define IMAPP_DECLARE_FLAGS(_flagsName, _flagEnum) \
		using _flagsName = FlagsContainer<_flagEnum>; \
		inline constexpr _flagsName operator|(const _flagEnum& _flagLeft, const _flagEnum& _flagRight) noexcept { return (_flagsName{ _flagLeft } | _flagsName{ _flagRight}); }

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
