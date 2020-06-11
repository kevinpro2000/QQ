#pragma once
#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
using namespace std;

#define HEX_CHAR "0123456789ABCDEF"
#define QINT_BIT_SIZE 128
#define LLONG_BIT_SIZE 64
#define DATA_ARR_SIZE 2

class QInt
{
private:
	long long dataArr[2];
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


	bool operator== (const QInt& B) const;
	bool operator!= (const QInt& B) const;
	QInt& operator= (const QInt& B);

	QInt operator& (const QInt& B) const;
	QInt operator| (const QInt& B) const;
	QInt operator^ (const QInt& B) const;
	QInt operator~ () const;

	QInt operator>> (int nums) const;
	QInt operator<< (int nums) const;
	QInt rol() const;
	QInt ror() const;


	void divide(const QInt& divisor, QInt& div, QInt& mod) const;
	friend QInt plusQInt(const QInt& A, const QInt& B);

	friend QInt decToBin(string src);
	friend QInt hexToBin(string src);
	friend string binToDec(const QInt& src);
	friend string binToHex(const QInt& src);

	bool isNegative() const;
	QInt toInverse() const;

	string toBinStr() const;
};
