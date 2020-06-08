#include "QInt.h"

QInt::QInt()
{
	fill(abc, abc + 2, 0);
}

QInt::~QInt()
{
}

QInt QInt::One()
{
	static QInt staticOne;
	static bool oneInit = false;
	if (!oneInit)
	{
		fill(staticOne.abc, staticOne.abc + 2, 0);
		oneInit = true;
	}
	staticOne.abc[2 - 1] = 1;
	return staticOne;
}

QInt QInt::Zero()
{
	static QInt staticZero;
	static bool zeroInit = false;
	if (!zeroInit)
	{
		fill(staticZero.abc, staticZero.abc + 2, 0);
		zeroInit = true;
	}
	return staticZero;
}

bool QInt::getBit(int pos) const
{
	int bytePos = 2 - 1 - pos / 64;
	int bitPos = pos % 64;
	return ((this->abc[bytePos] >> bitPos) & 1);
}

void QInt::setBit(int pos, bool value)
{
	int bytePos = 2 - 1 - pos / 64;
	int bitPos = pos % 64;
	if (value)
		this->abc[bytePos] = ((long long)1 << bitPos) | this->abc[bytePos];
	else
		this->abc[bytePos] = (~((long long)1 << bitPos)) & this->abc[bytePos];
}

QInt QInt::operator+(const QInt& B) const
{
	QInt res = plusQInt(*this, B);
	return res;
}

QInt QInt::operator-(const QInt& B) const
{
	QInt res = plusQInt(*this, B.toInverse());
	return res;
}

//Thuat toan Booth
QInt QInt::operator*(const QInt& B) const
{
	if (*this == QInt::Zero() || B == QInt::Zero())
		return QInt();

	QInt A;
	QInt Q = B;
	bool P = false;
	for (int i = 0; i < 2 * 64; i++)
	{
		bool lsbQ = Q.getBit(0); // lay LSB cua Q

		if (lsbQ == false && P == true) // 01
			A = plusQInt(A, *this);//A = A + *this;
		if (lsbQ == true && P == false) // 10
		{
			QInt temp = *this;
			A = plusQInt(A, temp.toInverse());//A = A - *this;
		}
		// Shift
		P = lsbQ;
		Q = Q >> 1;

		Q.setBit(2 * 64 - 1, A.getBit(0));
		A = A >> 1;
	}
	
	return Q;
}

QInt QInt::operator/(const QInt& B) const
{
	if (B == QInt::One() || *this == QInt::Zero())
		return *this;

	QInt div;
	QInt mod;
	this->divide(B, div, mod);

	return div;
}

bool QInt::operator>(const QInt& B) const
{
	bool isANegative = this->isNegative();
	bool isBNegative = B.isNegative();

	if (isANegative + isBNegative == 1)//A va B trai dau
	{
		if (isANegative)
			return false;
		else
			return true;
	}

	for (int i = 0; i < 2; i++)
	{
		//A>B
		if (this->abc[i] > B.abc[i])
			return true;

		//A<B
		if (this->abc[i] < B.abc[i])
			return false;
	}
	return false;
}

bool QInt::operator<(const QInt& B) const
{
	bool isANegative = this->isNegative();
	bool isBNegative = B.isNegative();

	if (isANegative + isBNegative == 1) //A va B trai dau nhau
	{
		if (isANegative)
			return true;
		else return false;
	}

	for (int i = 0; i < 2; i++)
	{
		//A > B
		if (this->abc[i] > B.abc[i])
			return false;
		//A < B
		if (this->abc[i] < B.abc[i])
			return true;
	}
	return false;
}

bool QInt::operator==(const QInt& B) const
{
	for (int i = 0; i < 2; i++)
	{
		if (this->abc[i] != B.abc[i])
			return false;
	}
	return true;
}

bool QInt::operator!=(const QInt& B) const
{
	return !(*this == B);
}

bool QInt::operator>=(const QInt& B) const
{
	return !(*this < B);
}

bool QInt::operator<=(const QInt& B) const
{
	return !(*this > B);
}

QInt& QInt::operator=(const QInt& B)
{
	for (int i = 0; i < 2; i++)
	{
		this->abc[i] = B.abc[i];
	}
	return *this;
}

QInt QInt::operator&(const QInt& B) const
{
	QInt res;
	for (int i = 0; i < 2; i++)
	{
		res.abc[i] = this->abc[i] & B.abc[i];
	}
	return res;
}

QInt QInt::operator|(const QInt& B) const
{
	QInt res;
	for (int i = 0; i < 2; i++)
	{
		res.abc[i] = this->abc[i] | B.abc[i];
	}
	return res;
}

QInt QInt::operator^(const QInt& B) const
{
	QInt res;
	for (int i = 0; i < 2; i++)
	{
		res.abc[i] = this->abc[i] ^ B.abc[i];
	}
	return res;
}

QInt QInt::operator~() const
{
	QInt res;
	for (int i = 0; i < 2; i++)
	{
		res.abc[i] = ~this->abc[i];
	}
	return res;
}

QInt QInt::operator>>(int nums) const
{
	QInt res = *this;
	bool msb = this->getBit(2 * 64 - 1);
	while (nums > 0)
	{
		res.abc[1] = res.abc[1] >> 1;
		res.setBit(63, 0);
		if (res.abc[0] & 1)		
			res.abc[1] = (((long long)1 << 63) | res.abc[1]);
		res.abc[0] = res.abc[0] >> 1;
		res.setBit(2 * 64 - 1, msb);
		nums--;
	}
	return res;
}

QInt QInt::operator<<(int nums) const
{
	QInt res = *this;
	while (nums > 0)
	{
		for (int i = 0; i < 2 - 1; i++)
		{
			res.abc[i] = res.abc[i] << 1;
			if ((res.abc[i + 1] >> 63) & 1)
			{
				res.abc[i] = (1 | res.abc[i]);
			}
		}
		res.abc[2 - 1] = res.abc[2 - 1] << 1;
		nums--;
	}
	return res;
}

string divByTwo(string src)
{
	string res = "";

	int additive = 0;
	for (int i = 0; i < src.length(); i++)
	{
		int digit = src[i] - '0';
		res += ((digit / 2 + additive) + '0');
		additive = ((digit % 2 == 0) ? 0 : 5);
	}

	if (res[0] == '0' && res.length() > 1)
		res.erase(0, 1);
	return res;
}

string mulByTwo(string src, int additive)
{
	string res = "";
	int carry = additive;

	for (int i = src.length() - 1; i >= 0; i--)
	{
		int temp = src[i] - '0';
		temp = temp * 2 + carry;
		res += (temp % 10 + '0');
		carry = temp / 10;
	}
	if (carry > 0)
		res += (carry + '0');

	reverse(res.begin(), res.end());

	return res;
}

QInt QInt::rol(int nums) const
{
	QInt res = *this;
	while (nums > 0)
	{
		bool additive = ((res.abc[0] >> 63) & 1);
		for (int i = 0; i < 2 - 1; i++)
		{
			res.abc[i] = res.abc[i] << 1;
			if ((res.abc[i + 1] >> 63) & 1)
			{
				res.abc[i] = (1 | res.abc[i]);
			}
		}
		res.abc[2 - 1] = res.abc[2 - 1] << 1;
		if (additive)
			res.abc[2 - 1] = (1 | res.abc[2 - 1]);
		nums--;
	}
	return res;
}

QInt QInt::ror(int nums) const
{
	QInt res = *this;
	while (nums > 0)
	{
		bool additive = (res.abc[2 - 1] & 1);
		for (int i = 2 - 1; i >= 1; i--)
		{
			res.abc[i] = res.abc[i] >> 1;
			if (res.abc[i - 1] & 1)
			{
				res.abc[i] = (((long long)1 << 63) | res.abc[i]);
			}
		}
		res.abc[0] = res.abc[0] >> 1;
		if (additive)
			res.abc[0] = (((long long)1 << 63) | res.abc[0]);
		nums--;
	}
	return res;
}

//Ham ho tro / 2 so QInt
void QInt::divide(const QInt& divisor, QInt& div, QInt& mod) const
{
	mod = QInt::Zero();
	div = *this;
	QInt M = divisor;
	// Luu lai dau cua phep chia.
	bool isDNegative = div.isNegative();
	bool isMNegative = M.isNegative();
	if (div.getBit(2 * 64 - 1) == true) // Bao dam div va M luon >= 0
	{
		QInt A;
		//div = A-div;
		div = plusQInt(A, div.toInverse());
	}
	if (M.getBit(2 * 64 - 1) == true)
	{
		QInt A;
		//M = A-M;
		M = plusQInt(A, M.toInverse());
	}
	for (int i = 0; i < 2 * 64; i++) // Thuc hien chia theo thuat toan trong tai lieu.
	{
		mod = mod << 1;
		mod.setBit(0, div.getBit(2 * 64 - 1)); // Copy MSB cua Q vao LSB cua A.
		div = div << 1;
		//QInt tmp = mod - M;
		QInt tmp = plusQInt(mod, M.toInverse());
		if (tmp.getBit(2 * 64 - 1) == false) // tmp >= 0
		{
			mod = tmp;
			div.setBit(0, 1);
		}
	}

	if (isDNegative)		//So du va so bi chia cung dau
		mod = mod.toInverse();
	if (isDNegative + isMNegative == 1) //Neu so bi chia va so chia trai dau thi doi dau thuong
		div = div.toInverse();
}

QInt decToBin(string src)
{
	QInt res;
	bool isNegative = false;
	if (src[0] == '-')
	{
		isNegative = true;
		src.erase(0, 1);
	}

	string binStr = "";
	do
	{
		int lastDigit = src[src.length() - 1] - '0';
		binStr += ((lastDigit % 2 == 0) ? '0' : '1');
		src = divByTwo(src);
	} while (src != "0");

	reverse(binStr.begin(), binStr.end());
	res.scanQInt(binStr, 2);
	if (isNegative)
		res = res.toInverse();
	return res;
}

QInt hexToBin(string src)
{
	QInt res;
	string binStr = "";
	for (int i = 0; i < src.length(); i++)
	{
		int pos = ((string)HEX_CHAR).find(src[i], 0);
		binStr += bitset<4>(pos).to_string();
	}
	res.scanQInt(binStr, 2);
	return res;
}

string binToDec(const QInt& src)
{
	string res = "";
	if (src == QInt::Zero())
	{
		res = "0";
		return res;
	}

	QInt posSrc = src;
	if (src.isNegative())
	{
		posSrc = posSrc.toInverse();
	}

	string binStr = posSrc.toBinStr();
	int pos = binStr.find('1', 0) + 1;
	res = "1";
	while (pos < binStr.length())
	{
		int additive = binStr[pos] - '0';
		res = mulByTwo(res, additive);
		pos++;
	}

	if (src.isNegative())
		res = '-' + res;

	return res;
}

string binToHex(const QInt& src)
{
	string res = "";
	stringstream ss;
	for (int i = 0; i < 2; i++)
	{
		ss << hex << src.abc[i];
		res += ss.str();
		ss.str("");
	}
	while (res[0] == '0' && res.length() > 1)
		res.erase(0, 1);
	res = Upstring(res);
	return res;
}

QInt plusQInt(const QInt& A, const QInt& B)
{
	QInt res;
	bool carry = 0;
	for (int i = 0; i <= 2 * 64 - 1; i++)
	{
		int temp = A.getBit(i) + B.getBit(i) + carry;
		switch (temp)
		{
		case 0://carry=0, bitA=0, bitB=0
			carry = 0;
			res.setBit(i, 0);
			break;
		case 1: //carry=0; bitA+bitB=1 || carry=1; bitA=0; bitB=0;
			carry = 0;
			res.setBit(i, 1);
			break;
		case 2: // carry=1; bitA+bitB=1; || carry=0; bitA=1; bitB=1;
			carry = 1;
			res.setBit(i, 0);
			break;
		case 3://carry=1; bitA=1; bitB=1;
			carry = 1;
			res.setBit(i, 1);
		}
	}
	return res;
}

string Upstring(string a)
{
	for (int i = 0; i < a.length(); i++)
	{
		if (isalpha(a[i]) && a[i] >= 'a')
		{
			a[i] -= 32;
		}
	}
	return a;
}

bool QInt::isNegative() const
{
	return ((this->getBit(2 * 64 - 1)) == 1);
}

QInt QInt::toInverse() const
{
	if (*this == QInt::Zero())
		return *this;

	QInt res;

	//Dao bit
	res = ~(*this);

	//+1
	for (int i = 2 - 1; i >= 0; i--)
	{
		if (res.abc[i] < 9223372036854775807)
		{
			res.abc[i] += 1;
			break;
		}
		else
			res.abc[i] = 0;
	}
	return res;
}

void QInt::scanQInt(string src, int base)
{
	int bitPos = 0;
	switch (base)
	{
	case 2:
		*this = QInt::Zero();
		for (int i = src.length() - 1; i >= 0; i--)
		{
			setBit(bitPos, src[i] == '1');
			bitPos++;
		}
		break;
	case 10:
		*this = decToBin(src);
		break;
	case 16:
		*this = hexToBin(src);
	}
}

void QInt::printQInt(int base)
{
	string res;
	switch (base)
	{
	case 2:
		res = toBinStr();
		while (res[0] == '0' && res.length() > 1)
			res.erase(0, 1);
		cout << res;
		break;
	case 10:
		cout << binToDec(*this);
		break;
	case 16:
		cout << binToHex(*this);
		break;
	}
}

string QInt::toBinStr() const
{
	string res = "";
	for (int i = 0; i < 2; i++)
	{
		res += bitset<64>(this->abc[i]).to_string();
	}
	return res;
}
