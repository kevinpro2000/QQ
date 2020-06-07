#pragma once
#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
using namespace std;

#define HEX_CHAR "0123456789ABCDEF"

class QInt
{
private:
	long long abc[2];
public:
	QInt();
	~QInt();

	static QInt One();
	static QInt Zero();

	void scanQInt(string src, int base);
	void printQInt(int base);

	bool getBit(int pos) const;
	void setBit(int pos, bool value);

	QInt operator+ (const QInt& B) const;
	QInt operator- (const QInt& B) const;
	QInt operator* (const QInt& B) const;
	QInt operator/ (const QInt& B) const;

	bool operator> (const QInt& B) const;
	bool operator< (const QInt& B) const;
	bool operator== (const QInt& B) const;
	bool operator!= (const QInt& B) const;
	bool operator>= (const QInt& B) const;
	bool operator<= (const QInt& B) const;
	QInt& operator= (const QInt& B);

	QInt operator& (const QInt& B) const;
	QInt operator| (const QInt& B) const;
	QInt operator^ (const QInt& B) const;
	QInt operator~ () const;

	QInt operator>> (int nums) const;
	QInt operator<< (int nums) const;
	QInt rol(int nums) const;
	QInt ror(int nums) const;


	void divide(const QInt& divisor, QInt& div, QInt& mod) const;
	QInt shiftRight(int nums) const;
	friend QInt plusQInt(const QInt& A, const QInt& B);

	friend QInt decToBin(string src);
	friend QInt hexToBin(string src);
	friend string binToDec(const QInt& src);
	friend string binToHex(const QInt& src);

	bool isNegative() const;
	QInt toInverse() const;

	string toBinStr() const;
};

QInt decToBin(string src);
QInt hexToBin(string src);
string binToDec(const QInt& src);
string binToHex(const QInt& src);
QInt plusQInt(const QInt& A, const QInt& B);
string Upstring(string a);
