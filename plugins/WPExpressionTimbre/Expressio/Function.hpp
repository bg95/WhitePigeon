#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <utility>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include "FunctionTraits.hpp"

namespace Expressio {
namespace Detail {

template <std::size_t argsCount>
struct ArgumentsUnpacker
{
	template<class Type, class Function, class InputIterator, class... UnpackedArguments>
	static Type unpack(Function&& function, InputIterator argsBegin, InputIterator argsEnd,
	                   UnpackedArguments&&... unpackedArgs)
	{
		if (argsBegin == argsEnd)
			throw std::invalid_argument("Syntax error: Not enough arguments.");

		return ArgumentsUnpacker<argsCount - 1>::template unpack<Type>
		        (std::forward<Function>(function), std::next(argsBegin), argsEnd,
		         std::forward<UnpackedArguments>(unpackedArgs)..., *argsBegin);
	}
};

template<>
struct ArgumentsUnpacker<0>
{
	template <class Type, class Function, class InputIterator, class... UnpackedArguments>
	static Type unpack(Function&& function, InputIterator argsBegin, InputIterator argsEnd,
	                   UnpackedArguments&&... unpackedArgs)
	{
		if (argsBegin != argsEnd)
			throw std::invalid_argument("Syntax error: Too many arguments.");

		return function(std::forward<UnpackedArguments>(unpackedArgs)...);
	}
};

template <class Type, class InputIterator, class Function>
std::function<Type(InputIterator, InputIterator)> createIteratedFunction(Function function)
{
	return [function](InputIterator argsBegin, InputIterator argsEnd)
	{
		return ArgumentsUnpacker<FunctionTraits<Function>::arity>::template unpack<Type>(function, argsBegin, argsEnd);
	};
}

template<class ValueType, class ArgumentIterator>
class Function
{
	std::function<ValueType(ArgumentIterator, ArgumentIterator)> function;
	std::size_t argsNum;

public:
	template<class Func>
	Function(const Func& f) :
		function(createIteratedFunction<ValueType, ArgumentIterator>(f)),
		argsNum(FunctionTraits<Func>::arity) { }

	ValueType operator()(ArgumentIterator begin, ArgumentIterator end) const
	{
		return function(begin, end);
	}

	std::size_t getArgsNum() const { return argsNum; }
};

}} // namespace Expressio::Detail

#endif // FUNCTION_HPP
