#ifndef EXPRESS_EXPR_HPP
#define EXPRESS_EXPR_HPP 1

#include <memory>
#include <variant>
#include <functional>

#include "Showable.hpp"
#include "Variable.hpp"
#include "AInt.hpp"

namespace expr{
	//! Base expression type
	struct Expr: Showable{
		virtual ~Expr();
	};

	// An integer or utf8-string
	using Var = std::variant<AInt, std::string>;
	using ExprPtr = std::unique_ptr<Expr>;

	//! reduce expressions to their canonical form
	ExprPtr reduce(ExprPtr p);

	//! Simplify expressions to their tersest representation
	ExprPtr simplify(ExprPtr p);

	struct Operator{
		using OpVar = std::variant<std::function<Var(Var, Var)>, std::function<Var(Var)>>;

		std::string symbol;
		OpVar op;
	};

	ExprPtr add(ExprPtr lhs, ExprPtr rhs);
	ExprPtr sub(ExprPtr lhs, ExprPtr rhs);
	ExprPtr mul(ExprPtr lhs, ExprPtr rhs);
	ExprPtr div(ExprPtr lhs, ExprPtr rhs);

	ExprPtr intLit(long i);
	ExprPtr intLit(unsigned long ui);
	ExprPtr intLit(const std::string &str);
	ExprPtr utf8Lit(std::string str);

	ExprPtr var(variables::Variable v);

	inline ExprPtr lit(long i){ return intLit(i); }
	inline ExprPtr lit(unsigned long ui){ return intLit(ui); }
	inline ExprPtr lit(std::string str){ return utf8Lit(std::move(str)); }

	namespace expressions{
		struct VarRef: Expr{
			VarRef(variables::Variable var_)
				: var(var_){}

			variables::Variable var;

			std::string toString() const override;
		};

		struct Binop: Expr{
			Binop(ExprPtr lhs_, ExprPtr rhs_, Operator op_)
				: lhs(std::move(lhs_)), rhs(std::move(rhs_)), op(std::move(op_)){}

			ExprPtr lhs, rhs;
			Operator op;

			std::string toString() const override;
		};

		struct Literal: Expr{};
		struct StringLiteral: Literal{};
		struct NumericLiteral: Literal{};

		struct Utf8String: StringLiteral{
			explicit Utf8String(std::string value_): value(std::move(value_)){}

			std::string toString() const override;

			std::string value;
		};

		struct Integer: NumericLiteral{
			explicit Integer(AInt value_): value(std::move(value_)){}

			std::string toString() const override;

			AInt value;
		};
	}

	struct ExprAdaptor: Showable{
		ExprAdaptor(ExprPtr expr_)
			: expr(std::move(expr_)){}

		ExprAdaptor(ExprAdaptor&&) = default;

		ExprAdaptor &operator =(ExprAdaptor &&other) = default;

		virtual ~ExprAdaptor();

		ExprPtr expr;

		std::string toString() const override{ return expr->toString(); }
	};

	inline ExprAdaptor reduce(ExprAdaptor e){
		return { reduce(std::move(e.expr)) };
	}

	inline ExprAdaptor simplify(ExprAdaptor e){
		return { simplify(std::move(e.expr)) };
	}

	namespace literals{
		inline ExprAdaptor operator "" _e(unsigned long long int i){
			return { intLit(static_cast<unsigned long>(i)) };
		}

		inline ExprAdaptor operator "" _e(const char *s, std::size_t n){
			return { utf8Lit(std::string(s, n)) };
		}
	}

	namespace ops{
		inline ExprAdaptor operator +(ExprAdaptor lhs, ExprAdaptor rhs){
			return { add(std::move(lhs.expr), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator -(ExprAdaptor lhs, ExprAdaptor rhs){
			return { sub(std::move(lhs.expr), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator *(ExprAdaptor lhs, ExprAdaptor rhs){
			return { mul(std::move(lhs.expr), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator /(ExprAdaptor lhs, ExprAdaptor rhs){
			return { div(std::move(lhs.expr), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator +(variables::Variable lhs, ExprAdaptor rhs){
			return { add(var(lhs), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator -(variables::Variable lhs, ExprAdaptor rhs){
			return { sub(var(lhs), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator *(variables::Variable lhs, ExprAdaptor rhs){
			return { mul(var(lhs), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator /(variables::Variable lhs, ExprAdaptor rhs){
			return { div(var(lhs), std::move(rhs.expr)) };
		}

		inline ExprAdaptor operator +(ExprAdaptor lhs, variables::Variable rhs){
			return { add(std::move(lhs.expr), var(rhs)) };
		}

		inline ExprAdaptor operator -(ExprAdaptor lhs, variables::Variable rhs){
			return { sub(std::move(lhs.expr), var(rhs)) };
		}

		inline ExprAdaptor operator *(ExprAdaptor lhs, variables::Variable rhs){
			return { mul(std::move(lhs.expr), var(rhs)) };
		}

		inline ExprAdaptor operator /(ExprAdaptor lhs, variables::Variable rhs){
			return { div(std::move(lhs.expr), var(rhs)) };
		}

		inline ExprAdaptor operator +(variables::Variable lhs, variables::Variable rhs){
			return { add(var(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator -(variables::Variable lhs, variables::Variable rhs){
			return { sub(var(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator *(variables::Variable lhs, variables::Variable rhs){
			return { mul(var(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator /(variables::Variable lhs, variables::Variable rhs){
			return { div(var(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator +(long lhs, variables::Variable rhs){
			return { add(intLit(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator -(long lhs, variables::Variable rhs){
			return { sub(intLit(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator *(long lhs, variables::Variable rhs){
			return { mul(intLit(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator /(long lhs, variables::Variable rhs){
			return { div(intLit(lhs), var(rhs)) };
		}

		inline ExprAdaptor operator +(variables::Variable lhs, long rhs){
			return { add(var(lhs), intLit(rhs)) };
		}

		inline ExprAdaptor operator -(variables::Variable lhs, long rhs){
			return { sub(var(lhs), intLit(rhs)) };
		}

		inline ExprAdaptor operator *(variables::Variable lhs, long rhs){
			return { mul(var(lhs), intLit(rhs)) };
		}

		inline ExprAdaptor operator /(variables::Variable lhs, long rhs){
			return { div(var(lhs), intLit(rhs)) };
		}
	}
}

using namespace expr::ops;

#endif // !EXPRESS_EXPR_HPP
