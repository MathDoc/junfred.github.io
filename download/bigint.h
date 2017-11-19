#ifndef DODECAHEDRON_BIGINT_H_
#define DODECAHEDRON_BIGINT_H_

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace mstd
{
class bigint
{
      private:
	std::vector<int> number;
	bool positive;
	int base;
	unsigned int skip;
	static const int default_base = 1000000000;

      public:
	// Constructors
	bigint();
	bigint(long long);
	bigint(std::string);
	bigint(const char *);
	bigint(const bigint &b);

	// Adding
	bigint operator+(bigint const &) const;
	bigint &operator+=(bigint const &);
	bigint operator+(long long const &) const;
	bigint &operator+=(long long);

	// Subtraction
	bigint operator-(bigint const &) const;
	bigint &operator-=(bigint const &);

	// Multiplication
	bigint operator*(bigint const &);
	bigint &operator*=(bigint const &);
	bigint operator*(long long const &);
	bigint &operator*=(int const &);

	// Compare
	bool operator<(const bigint &) const;
	bool operator>(const bigint &) const;
	bool operator<=(const bigint &) const;
	bool operator>=(const bigint &) const;
	bool operator==(const bigint &) const;
	bool operator!=(const bigint &) const;

	// Allocation
	bigint operator=(const long long &);
	bigint operator=(const char *);

	// Access
	int operator[](int const &);

	// Input&Output
	friend std::istream &operator>>(std::istream &, bigint &);
	friend std::ostream &operator<<(std::ostream &, bigint const &);

	// Helpers
	void clear();
	bigint &abs();

	// Power
	bigint &pow(int const &);

	// Trivia
	int digits() const;
	int trailing_zeros() const;

      private:
	int segment_length(int) const;
	bigint pow(int const &, std::map<int, bigint> &);
	int compare(bigint const &) const; // 0 a == b, -1 a < b, 1 a > b
};

bigint abs(bigint);
std::string to_string(bigint const &);
bigint factorial(int);

// Constructor
bigint::bigint()
{
	positive = true;
	base = bigint::default_base;
	skip = 0;
}
bigint::bigint(const bigint &b)
    : number(b.number), positive(b.positive), base(b.base), skip(b.skip)
{
}
bigint::bigint(long long value)
{
	base = bigint::default_base;
	skip = 0;
	if (value < 0) {
		positive = false;
		value *= -1;
	} else {
		positive = true;
	}

	while (value) {
		number.push_back((int)(value % base));
		value /= base;
	}
}

bigint::bigint(std::string stringInteger)
{
	int size = stringInteger.length();

	base = bigint::default_base;
	skip = 0;
	positive = (stringInteger[0] != '-');

	while (true) {
		if (size <= 0)
			break;
		if (!positive && size <= 1)
			break;

		int length = 0;
		int num = 0;
		int prefix = 1;
		for (int i(size - 1); i >= 0 && i >= size - 9; --i) {
			if (stringInteger[i] < '0' || stringInteger[i] > '9')
				break;
			num += (stringInteger[i] - '0') * prefix;
			prefix *= 10;
			++length;
		}
		number.push_back(num);
		size -= length;
	}
}

bigint::bigint(const char *stringInteger)
{
	int size = 0;
	for (int i = 0; stringInteger[i]; ++i)
		++size;

	base = bigint::default_base;
	skip = 0;
	positive = (stringInteger[0] != '-');

	while (true) {
		if (size <= 0)
			break;
		if (!positive && size <= 1)
			break;

		int length = 0;
		int num = 0;
		int prefix = 1;
		for (int i(size - 1); i >= 0 && i >= size - 9; --i) {
			if (stringInteger[i] < '0' || stringInteger[i] > '9')
				break;
			num += (stringInteger[i] - '0') * prefix;
			prefix *= 10;
			++length;
		}
		number.push_back(num);
		size -= length;
	}
}

// Adding
bigint bigint::operator+(bigint const &b) const
{
	bigint c = *this;
	c += b;

	return c;
}

bigint &bigint::operator+=(bigint const &b)
{
	if (!b.positive) {
		return *this -= b;
	}
	std::vector<int>::iterator it1 = number.begin();
	std::vector<int>::const_iterator it2 = b.number.begin();
	int sum = 0;
	while (it1 != number.end() || it2 != b.number.end()) {
		if (it1 != number.end()) {
			sum += *it1;
		} else {
			number.push_back(0);
			it1 = number.end() - 1;
		}
		if (it2 != b.number.end()) {
			sum += *it2;
			++it2;
		}
		*it1 = sum % base;
		++it1;
		sum /= base;
	}
	if (sum)
		number.push_back(1);

	return *this;
}

bigint bigint::operator+(long long const &b) const
{
	bigint c = *this;
	c += b;

	return c;
}

bigint &bigint::operator+=(long long b)
{
	std::vector<int>::iterator it = number.begin();
	if (skip > number.size()) {
		number.insert(number.end(), skip - number.size(), 0);
	}
	it += skip;
	bool initial_flag = true;
	while (b || initial_flag) {
		initial_flag = false;
		if (it != number.end()) {
			*it += b % base;
			b /= base;
			b += *it / base;
			*it %= base;
			++it;
		} else {
			number.push_back(0);
			it = number.end() - 1;
		}
	}

	return *this;
}

// Subtraction
bigint bigint::operator-(bigint const &b) const
{
	bigint c = *this;
	c -= b;

	return c;
}

bigint &bigint::operator-=(bigint const &b)
{
	std::vector<int>::iterator it1 = number.begin();
	std::vector<int>::const_iterator it2 = b.number.begin();
	int dif = 0;
	while (it1 != number.end() || it2 != b.number.end()) {
		if (it1 != number.end()) {
			dif += *it1;
			++it1;
		}
		if (it2 != b.number.end()) {
			dif -= *it2;
			++it2;
		}
		if (dif < 0) {
			*(it1 - 1) = dif + base;
			dif = -1;
		} else {
			*(it1 - 1) = dif % base;
			dif /= base;
		}
	}
	if (dif < 0)
		positive = false;

	if (number.size() > 1) {
		do {
			it1 = number.end() - 1;
			if (*it1 == 0)
				number.pop_back();
			else
				break;
		} while (number.size() > 1);
	}

	return *this;
}

// Multiplication
bigint bigint::operator*(bigint const &b)
{
	if (b.number.size() == 1)
		return *this *= b.number[0];
	std::vector<int>::iterator it1;
	std::vector<int>::const_iterator it2;
	bigint c;
	for (it1 = number.begin(); it1 != number.end(); ++it1) {
		for (it2 = b.number.begin(); it2 != b.number.end(); ++it2) {
			c.skip = (unsigned int)(it1 - number.begin()) +
				 (it2 - b.number.begin()); // TODO
			c += (long long)(*it1) * (*it2);
		}
	}
	c.skip = 0;

	return c;
}

bigint &bigint::operator*=(bigint const &b)
{
	*this = *this * b;

	return *this;
}

bigint bigint::operator*(long long const &b)
{
	bigint c = *this;
	c *= b;

	return c;
}

bigint &bigint::operator*=(int const &b)
{
	std::vector<int>::iterator it = number.begin();
	long long sum = 0;
	while (it != number.end()) {
		sum += (long long)(*it) * b;
		*it = (int)(sum % base);
		sum /= base;
		++it;
	}
	if (sum)
		number.push_back((int)sum);

	return *this;
}

// Power
bigint bigint::pow(int const &power, std::map<int, bigint> &lookup)
{
	if (power == 1)
		return *this;
	if (lookup.count(power))
		return lookup[power];

	int closestPower = 1;
	while (closestPower < power)
		closestPower <<= 1;
	closestPower >>= 1;

	if (power == closestPower)
		lookup[power] = pow(power / 2, lookup) * pow(power / 2, lookup);
	else
		lookup[power] = pow(closestPower, lookup) *
				pow(power - closestPower, lookup);

	return lookup[power];
}

bigint &bigint::pow(int const &power)
{
	std::map<int, bigint> lookup;
	if (power % 2 == 0 && !positive) {
		positive = true;
	}
	*this = pow(power, lookup);

	return *this;
}

// Compare
int bigint::compare(
    const bigint &a) const // 0 this == a || -1 this < a || 1 this > a
{
	if (positive && !a.positive)
		return 1;
	if (!positive && a.positive)
		return -1;

	int check = 1;
	if (!positive && !a.positive)
		check = -1;

	if (number.size() < a.number.size())
		return -1 * check;
	if (number.size() > a.number.size())
		return check;
	for (size_t i(number.size()); i > 0; --i) {
		if (number[i - 1] < a.number[i - 1])
			return -1 * check;
		if (number[i - 1] > a.number[i - 1])
			return check;
	}

	return 0; // ==
}

bool bigint::operator<(bigint const &b) const { return compare(b) == -1; }

bool bigint::operator<=(const bigint &b) const
{
	int compared = compare(b);

	return compared == 0 || compared == -1;
}

bool bigint::operator>(const bigint &b) const { return compare(b) == 1; }

bool bigint::operator>=(const bigint &b) const
{
	int compared = compare(b);

	return compared == 0 || compared == 1;
}

bool bigint::operator==(bigint const &b) const { return compare(b) == 0; }

bool bigint::operator!=(bigint const &b) const { return !(*this == b); }

// Allocation
bigint bigint::operator=(const long long &a)
{
	number.clear();
	long long t = a;
	do {
		number.push_back((int)(t % base));
		t /= base;
	} while (t != 0);

	return *this;
}

bigint bigint::operator=(const char *stringInteger)
{
	return bigint(stringInteger);
}

// Access
int bigint::operator[](int const &b) { return to_string(*this)[b] - '0'; }

// Trivia
int bigint::digits() const
{
	int segments = number.size();

	if (segments == 0)
		return 0;

	int digits = 9 * (segments - 1);
	digits += segment_length(number.back());

	return digits;
}

int bigint::trailing_zeros() const
{
	if (number.empty() || (number.size() == 1 && number[0] == 0))
		return 1;

	int zeros = 0;
	std::vector<int>::const_iterator it = number.begin();
	if (number.size() > 1) {
		for (; it != number.end() - 1 && *it == 0; ++it) {
			zeros += 9;
		}
	}
	int a = *it;
	while (a % 10 == 0 && a) {
		++zeros;
		a /= 10;
	}

	return zeros;
}

// Helpers
void bigint::clear()
{
	number.clear();
	positive = true;
	skip = 0;
}

bigint &bigint::abs()
{
	positive = true;

	return *this;
}

// Input&Output
std::ostream &operator<<(std::ostream &out, bigint const &a)
{
	if (!a.number.size())
		return out << 0;
	int i = a.number.size() - 1;
	for (; i >= 0 && a.number[i] == 0; --i)
		;

	if (i == -1)
		return out << 0;
	if (!a.positive)
		out << '-';

	std::vector<int>::const_reverse_iterator it =
	    a.number.rbegin() + (a.number.size() - i - 1);

	out << *it++;
	for (; it != a.number.rend(); ++it) {
		for (int i(0), len = a.segment_length(*it); i < 9 - len; ++i)
			out << '0';
		if (*it)
			out << *it;
	}

	return out;
}

std::istream &operator>>(std::istream &in, bigint &a)
{
	std::string str;
	in >> str;

	a = str;

	return in;
}

int bigint::segment_length(int segment) const
{
	int length = 0;
	while (segment) {
		segment /= 10;
		++length;
	}

	return length;
}

bigint abs(bigint value) { return value.abs(); }

std::string to_string(bigint const &value)
{
	std::ostringstream stream;
	stream << value;

	return stream.str();
}

bigint factorial(int n)
{
	bigint result = 1;
	if (n % 2) {
		result = n;
		--n;
	}
	int last = 0;
	for (; n >= 2; n -= 2) {
		result *= n + last;
		last += n;
	}

	return result;
}
}
#endif
