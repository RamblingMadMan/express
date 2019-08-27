#include <stdexcept>
#include <algorithm>
#include <variant>

#include "fmt/core.h"
#include "fmt/ostream.h"

#include "express/Overload.hpp"
#include "express/Expr.hpp"

using namespace expr;

Expr::~Expr(){}

ExprAdaptor::~ExprAdaptor(){}

ExprPtr expr::intLit(long i){
	return std::make_unique<expressions::Integer>(AInt(i));
}

ExprPtr expr::intLit(unsigned long ui){
	return std::make_unique<expressions::Integer>(AInt(ui));
}

ExprPtr expr::intLit(const std::string &str){
	return std::make_unique<expressions::Integer>(AInt(str));
}

ExprPtr expr::utf8Lit(std::string str){
	return std::make_unique<expressions::Utf8String>(std::move(str));
}

template<typename Fn>
std::function<Var(Var, Var)> makeBinopVarFn(Fn fn){
	return
		[f{std::move(fn)}](Var lhs, Var rhs){
			return std::visit(
				[&f, &rhs](auto &&lhsVal){
					return std::visit(
						[&f, &lhsVal](auto &&rhsVal){
							return f(lhsVal, rhsVal);
						},
						rhs
					);
				},
				lhs
			);
		};
}

ExprPtr expr::add(ExprPtr lhs, ExprPtr rhs){
	static Operator addOp{
		"+",
		makeBinopVarFn(
			overload(
				[](const std::string &lhs, const std::string &rhs) -> Var{ return lhs + rhs; },
				[](const AInt &lhs, const AInt &rhs) -> Var{ return lhs + rhs; },
				[](const auto&, const auto&) -> Var{
					throw std::runtime_error("Unexpected operands in addition");
				}
			)
		)
	};

	return std::make_unique<expressions::Binop>(std::move(lhs), std::move(rhs), addOp);
}

ExprPtr expr::sub(ExprPtr lhs, ExprPtr rhs){
	static Operator subOp{
		"-",
		makeBinopVarFn(
			overload(
				[](const AInt &lhs, const AInt &rhs) -> Var{ return lhs - rhs; },
				[](const auto&, const auto&) -> Var{
					throw std::runtime_error("Unexpected operands in subtraction");
				}
			)
		)
	};

	return std::make_unique<expressions::Binop>(std::move(lhs), std::move(rhs), subOp);
}

ExprPtr expr::mul(ExprPtr lhs, ExprPtr rhs){
	static Operator mulOp{
		"*",
		makeBinopVarFn(
			overload(
				[](const AInt &lhs, const AInt &rhs) -> Var{ return lhs * rhs; },
				[](const auto &lhs, const auto &rhs) -> Var{
					throw std::runtime_error(
						fmt::format(
							"Unexpected operands in multiplication, < {} > and < {} >",
							toString(lhs), toString(rhs)
						)
					);
				}
			)
		)
	};

	return std::make_unique<expressions::Binop>(std::move(lhs), std::move(rhs), mulOp);
}

ExprPtr expr::div(ExprPtr lhs, ExprPtr rhs){
	static Operator divOp{
		"/",
		makeBinopVarFn(
			overload(
				[](const auto&, const auto&) -> Var{
					throw std::runtime_error("Division unimplemented");
				}
			)
		)
	};

	return std::make_unique<expressions::Binop>(std::move(lhs), std::move(rhs), divOp);
}

ExprPtr expr::var(variables::Variable v){
	return std::make_unique<expressions::VarRef>(v);
}

std::string expressions::VarRef::toString() const{
	std::string res;

	auto varC = 'a' + var.id;
	if(varC > 'z'){
		res = "@" + std::to_string(var.id);
	}
	else{
		res.insert(end(res), char(varC));
	}

	return res;
}

std::string expressions::Binop::toString() const{
	return lhs->toString() + " " + op.symbol + " " + rhs->toString();
}

std::string replaceAll(std::string s, const std::string &from, const std::string &to){
	if(from.empty()) return s;

	std::size_t startPos = 0;
	while((startPos = s.find(from, startPos)) != std::string::npos){
		s.replace(startPos, from.length(), to);
		startPos += to.length();
	}

	return s;
}

std::string expressions::Utf8String::toString() const{
	std::string s = value;
	s = replaceAll(std::move(s), "\t", "\\t");
	s = replaceAll(std::move(s), "\n", "\\n");
	s = replaceAll(std::move(s), "\"", "\\\"");
	return "\"" + s + "\"";
}

std::string expressions::Integer::toString() const{
	return value.toString();
}

using Var = std::variant<AInt, std::string>;

template<typename VarOp>
ExprPtr literalOp(const expressions::Literal *lhs, const expressions::Literal *rhs, VarOp &&op){
	// simple binop if valid

	auto makeVar = [](const expressions::Literal *lit) -> std::optional<Var>{
		if(auto num = dynamic_cast<const expressions::NumericLiteral*>(lit)){
			if(auto int_ = dynamic_cast<const expressions::Integer*>(num))
				return std::make_optional<Var>(int_->value);
		}
		else if(auto str = dynamic_cast<const expressions::StringLiteral*>(lit)){
			if(auto utf8 = dynamic_cast<const expressions::Utf8String*>(str))
				return std::make_optional<Var>(utf8->value);
		}

		return std::nullopt;
	};

	auto lhsVar = makeVar(lhs);
	auto rhsVar = makeVar(rhs);

	if(!lhsVar && !rhsVar)
		throw std::runtime_error(fmt::format("Invalid literal operands < {} > and < {} >", lhs->toString(), rhs->toString()));
	else if(!lhsVar)
		throw std::runtime_error(fmt::format("Invalid literal operand: {}", lhs->toString()));
	else if(!rhsVar)
		throw std::runtime_error(fmt::format("Invalid literal operand: {}", rhs->toString()));

	auto res = std::forward<VarOp>(op)(std::move(*lhsVar), std::move(*rhsVar));

	return
		std::visit(
			overload(
				[](const std::string &s) -> ExprPtr{ return std::make_unique<expressions::Utf8String>(s); },
				[](const AInt &i) -> ExprPtr{ return std::make_unique<expressions::Integer>(i); }
			),
			res
		);
}

ExprPtr expr::reduce(ExprPtr ptr){
	auto p = ptr.get();

	if(auto binop = dynamic_cast<expressions::Binop*>(p)){
		binop->lhs = reduce(std::move(binop->lhs));
		binop->rhs = reduce(std::move(binop->rhs));

		auto lhs = dynamic_cast<const expressions::Literal*>(binop->lhs.get());
		auto rhs = dynamic_cast<const expressions::Literal*>(binop->rhs.get());

		if(!lhs || !rhs)
			return ptr;

		return std::visit(
			overload(
				[lhs, rhs](std::function<Var(Var, Var)> &fn) -> ExprPtr{
					return literalOp(lhs, rhs, fn);
				},
				[](auto&&) -> ExprPtr{ throw std::runtime_error("INTERNAL ERROR: unexpected unary op function in binary operator"); }
			),
			binop->op.op
		);
	}

	return ptr;
}

ExprPtr expr::simplify(ExprPtr e){
	// first reduction
	e = reduce(std::move(e));

	auto p = e.get();

	if(auto binop = dynamic_cast<const expressions::Binop*>(p)){
		// try re-arrange then reduce
		return e;
	}
	else{
		return e;
	}
}
