#ifndef EXPRESS_OVERLOAD_HPP
#define EXPRESS_OVERLOAD_HPP 1

#include <functional>

namespace expr{
	template<typename ... Fns>
	struct Overloaded: Fns...{ using Fns::operator()...; };

	template<typename ... Fns>
	Overloaded(Fns...) -> Overloaded<Fns...>;

	template<typename ... Fns>
	auto overload(Fns &&... fns){
		return Overloaded{ std::forward<Fns>(fns)... };
	}
}

#endif // !EXPRESS_OVERLOAD_HPP
