#ifndef IMAPP_UTILS_HPP
#define IMAPP_UTILS_HPP

#include <type_traits>

#define IMAPP_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__) // Allows us to forward without include anything

#define IMAPP_DECLARE_FLAGS(_flagsName, _flagEnum) \
	using _flagsName = FlagsContainer<_flagEnum>; \
	inline constexpr _flagsName operator|(const _flagEnum& _flagLeft, const _flagEnum& _flagRight) noexcept { return (_flagsName{ _flagLeft } | _flagsName{ _flagRight}); }

namespace ImApp
{
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
}

#endif
