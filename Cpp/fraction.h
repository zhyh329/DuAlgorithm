#pragma once
#include "common.h"
#include "MathMods.h"

class Fraction
{
public:
	int numerator, denominator;
	
	Fraction(int m, int d) :numerator(m), denominator(d) {
		this->abbreviate();
	};

	void abbreviate()
	{
		int t = Mods::gcd(numerator, denominator);
		numerator /= t;
		denominator /= t;
	}

	void print(Fraction x)
	{
		int l = static_cast<int>(max(floor(log10(x.numerator)), floor(log10(x.denominator)))) + 2;
		cout << x.numerator << endl;
		for (int i = 0; i < l; ++i)
			cout << '-';
		cout << endl << x.denominator << endl;
	}

	// 166. Fraction to Recurring Decimal
	string toDecimal() {
		int s1 = numerator >= 0 ? 1 : -1;
		int s2 = denominator >= 0 ? 1 : -1;
		long long num = abs((long long)numerator);
		long long den = abs((long long)denominator);
		long long out = num / den;
		long long rem = num % den;
		unordered_map<long long, int> m;
		string res = to_string(out);
		if (s1 * s2 == -1 && (out > 0 || rem > 0)) res = "-" + res;
		if (rem == 0) return res;
		res += ".";
		string s = "";
		int pos = 0;
		while (rem != 0) {
			if (m.find(rem) != m.end()) {
				s.insert(m[rem], "(");
				s += ")";
				return res + s;
			}
			m[rem] = pos;
			s += to_string((rem * 10) / den);
			rem = (rem * 10) % den;
			++pos;
		}
		return res + s;
	}

	double getDouble(Fraction x)
	{
		return static_cast<double>(x.numerator / x.denominator);
	}
};

Fraction operator +(Fraction const &x, Fraction const &y)
{
	return Fraction(x.numerator * y.denominator + y.numerator * x.denominator, x.denominator * y.denominator);
}

Fraction operator *(Fraction const &x, Fraction const &y)
{
	return Fraction(x.numerator * y.numerator, x.denominator * y.denominator);
}

namespace Fractions {
	// 166. Fraction to Recurring Decimal
	string fractionToDecimal(int numerator, int denominator) {
		int s1 = numerator >= 0 ? 1 : -1;
		int s2 = denominator >= 0 ? 1 : -1;
		long long num = abs((long long)numerator);
		long long den = abs((long long)denominator);
		long long out = num / den;
		long long rem = num % den;
		unordered_map<long long, int> m;
		string res = to_string(out);
		if (s1 * s2 == -1 && (out > 0 || rem > 0)) res = "-" + res;
		if (rem == 0) return res;
		res += ".";
		string s = "";
		int pos = 0;
		while (rem != 0) {
			if (m.find(rem) != m.end()) {
				s.insert(m[rem], "(");
				s += ")";
				return res + s;
			}
			m[rem] = pos;
			s += to_string((rem * 10) / den);
			rem = (rem * 10) % den;
			++pos;
		}
		return res + s;
	}

}