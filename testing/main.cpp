#include "fmt/core.h"
#include "fmt/ostream.h"

#include "express/Expr.hpp"

void print(const expr::Showable &s){
	fmt::print("{}\n", s);
}

int main(int argc, char *argv[]){
	using namespace expr::literals;
	using namespace expr::variables;

	auto e0 = expr::reduce(24_e * 18_e);
	auto e1 = 2 * x;

	print(e0);
	print(e1);
}
