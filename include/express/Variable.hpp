#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <cstddef>

namespace expr{
	namespace variables{
		struct Variable{
			explicit constexpr Variable(std::size_t id_)
				: id(id_){}

			constexpr bool operator ==(const Variable &other) const noexcept{
				return id == other.id;
			}

			std::size_t id;
		};

		constexpr std::size_t idStart = __COUNTER__ + 1;

	#define DEF_VAR_IMPL(name, id) constexpr inline Variable name(id - idStart)
	#define DEF_VAR(name) DEF_VAR_IMPL(name, __COUNTER__)

		DEF_VAR(a);
		DEF_VAR(b);
		DEF_VAR(c);
		DEF_VAR(d);
		DEF_VAR(e);
		DEF_VAR(f);
		DEF_VAR(g);
		DEF_VAR(h);
		DEF_VAR(i);
		DEF_VAR(j);
		DEF_VAR(k);
		DEF_VAR(l);
		DEF_VAR(m);
		DEF_VAR(n);
		DEF_VAR(o);
		DEF_VAR(p);
		DEF_VAR(q);
		DEF_VAR(r);
		DEF_VAR(s);
		DEF_VAR(t);
		DEF_VAR(u);
		DEF_VAR(v);
		DEF_VAR(w);
		DEF_VAR(x);
		DEF_VAR(y);
		DEF_VAR(z);

	#undef DEF_VAR
	#undef DEF_VAR_IMPL
	}
}

#endif // VARIABLE_HPP
