#ifndef EXPRESS_SHOWABLE_HPP
#define EXPRESS_SHOWABLE_HPP 1

#include <ostream>
#include <string>

namespace expr{
	struct Showable{ virtual std::string toString() const = 0; };

	inline std::string toString(const Showable &s){ return s.toString(); }
	inline std::string toString(std::string s){ return s; }
}

inline std::ostream &operator <<(std::ostream &str, const expr::Showable &s){
	str << s.toString();
	return str;
}

#endif // !EXPRESS_SHOWABLE_HPP
