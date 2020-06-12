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
	long long dataArr[2];//Dùng kiểu long long (64 bit) để lưu 128 bit dữ liệu
public:
	QInt();//Hàm constructor mặc định
	~QInt();//Hàm destructor mặc định

	static QInt One();//Hàm khởi tạo số 1 dưới kiểu QInt
	static QInt Zero();//Hàm khởi tạo số 0 dưới kiểu QInt

	void scanQInt(string src, int base);//Hàm đọc chuỗi đầu vào dưới dạng chỉ thị base (2, 10 hay 16)
	void printQInt(int base);//Hàm in chuỗi dưới dạng base (2, 10 hay 16)

	bool getBit(int pos) const;//Hàm lấy bit ở vị trí pos
	void setBit(int pos, bool value);//Hàm gán bit ở vị trí pos = 1 value

	QInt operator+ (const QInt& B) const;//Toán tử +
	QInt operator- (const QInt& B) const;//Toán tử -
	QInt operator* (const QInt& B) const;//Toán tử *
	QInt operator/ (const QInt& B) const;//Toán tử /


	bool operator== (const QInt& B) const;//Toán tử so sánh bằng
	bool operator!= (const QInt& B) const;//Toán tử so sánh khác
	QInt& operator= (const QInt& B);//Toán tử gán mặc định

	QInt operator& (const QInt& B) const;//Toán tử &(and)
	QInt operator| (const QInt& B) const;//Toán tử |(or)
	QInt operator^ (const QInt& B) const;//Toán tử ^(xor)
	QInt operator~ () const;//Toán tử ~ (not)

	QInt operator>> (int nums) const;//Toán tử dịch phải nums bit (dịch số học)
	QInt operator<< (int nums) const;//Toán tử dịch trái nums bit (dịch số học)
	QInt rol() const;//phép xoay trái
	QInt ror() const;//phép xoay phải


	void divide(const QInt& divisor, QInt& div, QInt& mod) const;//Hàm chia (hỗ trợ cho toán tử chia phía trên) truyền vào số QInt divisor, thương là div, số dư là mod
	friend QInt plusQInt(const QInt& A, const QInt& B);//Hàm cộng (hỗ trợ toán tử cộng)

	friend QInt decToBin(string src);//Hàm chuyển đổi từ thập phân sang nhị phân (truyền vào chuỗi src dạng dec)
	friend QInt hexToBin(string src);//Hàm chuyển đổi từ thập lục phân sang nhị phân (truyền vào chuỗi src dạng hex)
	friend string binToDec(const QInt& src);//Hàm chuyển đổi từ nhị phân sang thập phân (truyền vào số QInt src)
	friend string binToHex(const QInt& src);//Hàm chuyển đổi từ nhị phân sang thập lục phân (truyền vào số QInt src)

	bool isNegative() const;//Hàm kiểm tra số âm
	QInt toInverse() const;//Hàm bù 2

	string toBinStr() const;//Hàm chuyển chuỗi sang dạng chuỗi nhị phân
};
