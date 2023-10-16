#pragma once

#include "utility.hpp"

namespace brisk
{
	template <class Function, class... Args>
	decltype(auto) invoke(Function&& f, Args&&... args) noexcept
	{
		return forward<Function>(f)(forward<Args>(args)...);
	}

	template <class Type>
	struct less
	{
		constexpr bool operator()(const Type& lhs, const T& rhs) const
		{
			return lhs < rhs;
		}
	};

	template <class Type>
	struct greater
	{
		constexpr bool operator()(const Type& lhs, const Type& rhs) const
		{
			return lhs > rhs;
		}
	};

	template <class Type>
	struct less_equal
	{
		constexpr bool operator()(const Type& lhs, const Type& rhs) const
		{
			return lhs <= rhs;
		}
	};

	template <class Type>
	struct greater_equal
	{
		constexpr bool operator()(const Type& lhs, const Type& rhs) const
		{
			return lhs >= rhs;
		}
	};

	template <class Type>
	struct equal
	{
		constexpr bool operator()(const Type& lhs, const Type& rhs) const
		{
			return lhs == rhs;
		}
	};

	template <class Type>
	struct not_equal_to
	{
		constexpr bool operator()(const Type& lhs, const Type& rhs) const
		{
			return lhs != rhs;
		}
	};
}