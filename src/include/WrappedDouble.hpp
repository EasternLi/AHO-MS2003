#pragma once

#include <compare>
#include <cmath>
#include <iostream>
#include <string>

// NOTE: not consider NAN
class WrappedDouble {
public:
	WrappedDouble() = default;
	WrappedDouble(double value): m_value(value) {}
	WrappedDouble(size_t value): m_value(value) {}
	WrappedDouble(int    value): m_value(value) {}
	
	using Self = WrappedDouble;
	
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
	bool operator< (const Self& other) const
		{return m_value + eps < other.m_value;}
	bool operator<=(const Self& other) const
		{return m_value - eps <=other.m_value;}
	bool operator> (const Self& other) const
		{return m_value - eps > other.m_value;}
	bool operator>=(const Self& other) const
		{return m_value + eps >=other.m_value;}
	
	friend std::ostream& operator<<(std::ostream& out, const Self& x)
		{ return out << x.m_value; }
	void prettyPrint(std::string& out) const
		{ out = std::to_string(m_value); }
	
	double m_value;
private:
	constexpr static double eps = 1e-4;
};
