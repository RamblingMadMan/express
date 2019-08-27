#include "gmpxx.h"

#include "express/AInt.hpp"

using namespace expr;

struct AInt::Impl{
	mpz_class value;
};

AInt::AInt() noexcept: m_impl(std::make_unique<Impl>()){}

AInt::AInt(const std::string &str): AInt(){
	m_impl->value.set_str(str, 10);
}

AInt::AInt(long i) noexcept: AInt(){
	m_impl->value = i;
}

AInt::AInt(unsigned long ui) noexcept: AInt(){
	m_impl->value = ui;
}

AInt::AInt(const AInt &other) noexcept: AInt(){
	m_impl->value = other.m_impl->value;
}

AInt::AInt(AInt &&other) noexcept: AInt(){
	std::swap(m_impl, other.m_impl);
}

AInt &AInt::operator =(const AInt &other) noexcept{
	m_impl->value = other.m_impl->value;
	return *this;
}

AInt &AInt::operator =(AInt &&other) noexcept{
	m_impl = std::move(other.m_impl);
	return *this;
}

AInt &AInt::operator =(long int i) noexcept{
	m_impl->value = i;
	return *this;
}

AInt &AInt::operator =(unsigned long int ui) noexcept{
	m_impl->value = ui;
	return *this;
}

AInt::~AInt(){}

std::string AInt::toString() const{
	return m_impl->value.get_str();
}

AInt AInt::operator +(const AInt &rhs) const noexcept{
	AInt ret;
	ret.m_impl->value = m_impl->value + rhs.m_impl->value;
	return ret;
}

AInt AInt::operator -(const AInt &rhs) const noexcept{
	AInt ret;
	ret.m_impl->value = m_impl->value - rhs.m_impl->value;
	return ret;
}

AInt AInt::operator *(const AInt &rhs) const noexcept{
	AInt ret;
	ret.m_impl->value = m_impl->value * rhs.m_impl->value;
	return ret;
}
