#pragma once

#include <compare>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>

template<double eps = 1e-4>
class WrappedDouble {
public:
	WrappedDouble() = default;
	WrappedDouble(double value): m_value(value) {}
	WrappedDouble(size_t value): m_value(value) {}
	WrappedDouble(int    value): m_value(value) {}
	
	using Self = WrappedDouble<eps>;
	
	Self operator- () const {return -m_value;}
	Self operator+ (const Self& other) const {return m_value + other.m_value;}
	Self operator- (const Self& other) const {return m_value - other.m_value;}
	Self operator* (const Self& other) const {return m_value * other.m_value;}
	Self operator/ (const Self& other) const {return m_value / other.m_value;}
	Self&operator+=(const Self& other) {m_value += other.m_value; return *this;}
	Self&operator-=(const Self& other) {m_value -= other.m_value; return *this;}
	Self&operator/=(const Self& other) {m_value /= other.m_value; return *this;}
	
	bool operator==(const Self& other) const {
		// not forget inf-inf=>nan
		return !(std::abs(m_value - other.m_value) > eps);
	}
	auto operator<=>(const Self& other) const {
		if (m_value - other.m_value > +eps) return 1. <=> 0.;
		if (m_value - other.m_value < -eps) return 0. <=> 1.;
		return 0. <=> 0.;
	}
	
	friend std::ostream& operator<<(std::ostream& out, const Self& x)
		{ return out << x.m_value; }
	void prettyPrint(std::string& out) const
		{ out = std::to_string(m_value); }
	
	double m_value;
};

template<double eps>
bool ::std::isnan(const WrappedDouble<eps>& x)
	{ return ::std::isnan(x.m_value); }

template<double eps>
class std::numeric_limits<WrappedDouble<eps>> {
public:
	static WrappedDouble<eps> infinity()
		{ return ::std::numeric_limits<double>::infinity(); }
};
