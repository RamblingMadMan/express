#ifndef EXPRESS_AINT_HPP
#define EXPRESS_AINT_HPP 1

#include <memory>

#include "Showable.hpp"

namespace expr{
	//! Arbitrary precision integer
	class AInt: public Showable{
		public:
			explicit AInt(const std::string &str);
			explicit AInt(long i) noexcept;
			explicit AInt(unsigned long ui) noexcept;

			explicit AInt(int i) noexcept: AInt(static_cast<long>(i)){}
			explicit AInt(unsigned int ui) noexcept: AInt(static_cast<unsigned long>(ui)){}

			AInt(const AInt &other) noexcept;
			AInt(AInt &&other) noexcept;

			AInt &operator =(const AInt &other) noexcept;
			AInt &operator =(AInt &&other) noexcept;
			AInt &operator =(long int i) noexcept;
			AInt &operator =(unsigned long int ui) noexcept;

			virtual ~AInt();

			std::string toString() const override;

			AInt operator +(const AInt &rhs) const noexcept;
			AInt operator -(const AInt &rhs) const noexcept;
			AInt operator *(const AInt &rhs) const noexcept;
			//AInt operator /(const AInt &rhs);

		private:
			AInt() noexcept;

			struct Impl;
			std::unique_ptr<Impl> m_impl;
	};
}

#endif // !EXPRESS_AINT_HPP
