#include "QInt.h"

QInt::QInt()//Hàm khởi tạo QInt mặc định
{
	fill(dataArr, dataArr + DATA_ARR_SIZE, 0);//Gán tất cả các bit = 0
}

QInt::~QInt()//Hàm phá hủy mặc định
{
}

QInt QInt::One()//Hàm tạo số 1 kiểu QInt
{
	static QInt staticOne;//Khởi tạo biến static để chỉ cần khởi tạo 1 lần nhưng sử dụng trong toàn bộ chương trình
	fill(staticOne.dataArr, staticOne.dataArr + DATA_ARR_SIZE, 0);//Gán tất cả các bit = 0
	staticOne.dataArr[DATA_ARR_SIZE - 1] = 1;//Gán bit cuối = 1
	return staticOne;
}

QInt QInt::Zero()
{
	static QInt staticZero;//Khởi tạo biến static để chỉ cần khởi tạo 1 lần nhưng sử dụng trong toàn bộ chương trình
	fill(staticZero.dataArr, staticZero.dataArr + DATA_ARR_SIZE, 0);//Gán tất cả các bit = 0
	return staticZero;
}

bool QInt::getBit(int pos) const//Truyền vào vị trí pos của bit cần lấy (tính từ trái sang phải)
{
	int dataArrPos = DATA_ARR_SIZE - 1 - (QINT_BIT_SIZE - 1 - pos) / LLONG_BIT_SIZE;//Tính vị trí pos truyền vào nằm ở phần tử nào trong dataArr
	int bitShift = (QINT_BIT_SIZE - 1 - pos) % 64;//Tính số bit cần dịch phải để khiến cho bit ở vị trí pos dịch tới cuối cùng
	return ((this->dataArr[dataArrPos] >> bitShift) & 1);//thực hiện toán tử & với 1 để trả về bit cần lấy
}

void QInt::setBit(int pos, bool value)//Truyền vào vị trí pos của bit cần gán (tính từ trái sang phải), và giá trị gán là 0 hay 1 
{
	int dataArrPos = DATA_ARR_SIZE - 1 - (QINT_BIT_SIZE - 1 - pos) / LLONG_BIT_SIZE;//Tương tự hàm getBit ở trên
	int bitPos = (QINT_BIT_SIZE - 1 - pos) % 64;									//Tương tự hàm getBit ở trên
	if (value)//Nếu value = 1
		this->dataArr[dataArrPos] = ((long long)1 << bitPos) | this->dataArr[dataArrPos];//thực hiện dịch trái và toán tử | để gán bit ở vị trí pos = value
	else//Nếu value = 0
		this->dataArr[dataArrPos] = (~((long long)1 << bitPos)) & this->dataArr[dataArrPos];//tương tự như trường hợp value = 1 nhưng ngược lại dùng toán tử ~ và toán tử &
}

QInt QInt::operator+(const QInt& B) const//Toán tử +
{
	QInt res = plusQInt(*this, B);//gọi hàm plusQInt để thực hiện phép cộng
	return res;
}

QInt QInt::operator-(const QInt& B) const//Toán tử -
{
	QInt res = plusQInt(*this, B.toInverse());//Gọi hàm plusQInt để thực hiện phép cộng với số bù 2 của B
	return res;
}

QInt QInt::operator*(const QInt& B) const//Toán tử nhân, thực hiện theo thuật toán Booth
{
	if (*this == QInt::Zero() || B == QInt::Zero())//Kiểm tra 1 trong 2 thừa số là 0 thì trả về Qint mặc định (Qint mặc định = 0)
		return QInt();

	QInt A;//A đóng vai trò là 1 phần trong kết quả 
	QInt res = B;//đóng vai trò kết quả sau cùng
	bool flag = false;//đóng vai trò 1 bit nhớ 
	for (int i = 0; i < QINT_BIT_SIZE; i++)
	{
		bool lsbRes = res.getBit(QINT_BIT_SIZE - 1); // lấy LSB của res
		//Kiểm tra 2 bit cuối (bit cuối của res và flag)
		if (lsbRes == false && flag == true) // 2 bit là 01
			A = A + *this;
		if (lsbRes == true && flag == false) // 2 bit là 10
		{
			A = A - *this;
		}
		//Thực hiện dịch phải
		flag = lsbRes;//gán bit lsb của res vào flag
		res = res >> 1;//dịch phải res
		res.setBit(0, A.getBit(QINT_BIT_SIZE - 1));//lấy bit cuối của A gán vào bit đầu của res
		A = A >> 1;//dịch phải A
	}
	return res;
}

QInt QInt::operator/(const QInt& B) const//Toán tử /
{
	if (B == QInt::One() || *this == QInt::Zero())//Kiểm tra nếu số bị chia = 0 hoặc số chia = 1 thì trả về *this (số bị chia)
		return *this;
	QInt div;//Thương
	QInt mod;//Số dư
	this->divide(B, div, mod);//gọi hàm divide
	return div;
}



bool QInt::operator==(const QInt& B) const//Toán tử so sánh bằng nhau
{
	for (int i = 0; i < DATA_ARR_SIZE; i++)//vòng lặp qua các phần tử trong mảng dataArr của QInt
	{
		if (this->dataArr[i] != B.dataArr[i])//So sánh từng phần tử với nhau
			return false;//nếu tồn tại 1 phần tử khác nhau thì trả về false 
	}
	return true;//nếu tất cả đều không có phần tử nào khác nhau thì trả về true
}

bool QInt::operator!=(const QInt& B) const//toán tử so sánh khác nhau
{
	return !(*this == B);//gọi phủ định của toán tử so sánh bằng nhau
}


QInt& QInt::operator=(const QInt& B)//toán tử gán
{
	for (int i = 0; i < DATA_ARR_SIZE; i++)//lặp qua tất cả các phần tử trong mảng dataArr
	{
		this->dataArr[i] = B.dataArr[i];//gán từng phần tử
	}
	return *this;
}

QInt QInt::operator&(const QInt& B) const//toán tử & (and)
{
	QInt res;
	for (int i = 0; i < DATA_ARR_SIZE; i++)//lặp qua từng phần tử
	{
		res.dataArr[i] = this->dataArr[i] & B.dataArr[i];//gọi toán tử & cho từng cặp phần tử
	}
	return res;
}

QInt QInt::operator|(const QInt& B) const//toán tử | (or)
{
	QInt res;
	for (int i = 0; i < DATA_ARR_SIZE; i++)//lặp qua từng phần tử
	{
		res.dataArr[i] = this->dataArr[i] | B.dataArr[i];//gọi toán tử | cho từng cặp phần tử
	}
	return res;
}

QInt QInt::operator^(const QInt& B) const//toán tử ^ (xor)
{
	QInt res;
	for (int i = 0; i < DATA_ARR_SIZE; i++)//lặp qua từng phần tử
	{
		res.dataArr[i] = this->dataArr[i] ^ B.dataArr[i];//gọi toán tử | cho từng cặp phần tử
	}
	return res;
}

QInt QInt::operator~() const//Toán tử ~ (not)
{
	QInt res;
	for (int i = 0; i < DATA_ARR_SIZE; i++)//lặp qua từng phần tử
	{
		res.dataArr[i] = ~this->dataArr[i];//gọi toán tử ~ cho từng phần tử
	}
	return res;
}

QInt QInt::operator>>(int nums) const//toán tử dịch phải, truyền vào nums là số lần dịch
{
	QInt res = *this;//res đóng vai trò là kết quả sau khi dịch
	bool msb = this->getBit(0);//lưu bit MSB
	while (nums > 0)//lặp lại nums lần
	{
		res.dataArr[1] = res.dataArr[1] >> 1;//dịch phải phần tử thứ 2 của mảng dataArr
		res.setBit(64, res.getBit(63));//gán bit đầu tiên của phần tử thứ 2 bằng bit cuối cùng của phần tử thứ nhất
		res.dataArr[0] = res.dataArr[0] >> 1;//dịch phải phần tử đầu tiên của mảng dataArr
		res.setBit(0, msb);//gán bit đầu tiên của phần tử thứ nhất là bit MSB
		nums--;
	}
	return res;
}

QInt QInt::operator<<(int nums) const//toán tử dịch trái, truyền vào nums là số lần dịch
{
	QInt res = *this;//res lưu kết quả sau khi dịch
	while (nums > 0)//lặp lại nums lần
	{
		res.dataArr[0] = res.dataArr[0] << 1;//thực hiện dịch trái phần tử đầu tiên trong mảng dataArr
		res.setBit(63, res.getBit(64));//gán bit cuối cùng của phần tử đầu tiên bằng bit đầu tiên của phần tử thứ 2
		res.dataArr[1] = res.dataArr[1] << 1;//dịch trái phần tử thứ hai trong mảng dataArr
		nums--;
	}
	return res;
}

string divByTwo(string src)//hàm chia số lưu dưới dạng chuỗi (src) cho 2
{
	string res = "";//biến lưu kết quả
	int additive = 0;//biến nhớ
	for (int i = 0; i < src.length(); i++)//lặp qua từng số trong chuỗi
	{
		int digit = src[i] - '0';//chuyển số sang kiểu int
		res += ((digit / 2 + additive) + '0');//chia số đó cho 2 và chuyển sang kiểu string sau đó thêm vào res
		if (digit % 2 == 0)//nếu số đó chia hết cho 2
			additive = 0;//lưu biến nhớ = 0
		else//nếu không chia hết cho 2
			additive = 5;//lưu biến nhớ = 5
	}
	if (res[0] == '0' && res.length() > 1)//vòng lặp xóa số 0 dư ở đầu chuỗi res
		res.erase(0, 1);
	return res;
}

string mulByTwo(string src, int additive)//hàm nhân số lưu dưới dạng chuỗi (src) với 2 
										 //hàm này chỉ sử dụng để hỗ trợ cho hàm binToDec nên có thêm tham số additive để lưu biến nhớ
{
	string res = "";//biến lưu kết quả
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

QInt QInt::rol() const//need to repair
{
	QInt res = *this;
	bool additive = ((res.dataArr[0] >> 63) & 1);
	res.dataArr[0] = res.dataArr[0] << 1;
	res.setBit(63, res.getBit(64));
	res.dataArr[1] = res.dataArr[1] << 1;
	if (additive)
		res.setBit(127, additive);
	return res;
}

QInt QInt::ror() const//need to repair
{
	QInt res = *this;
	bool additive = (res.dataArr[1] & 1);
	res.dataArr[1] = res.dataArr[1] >> 1;
	res.setBit(64, res.getBit(63));
	res.dataArr[0] = res.dataArr[0] >> 1;
	if (additive)
		res.setBit(0, additive);
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
	if (div.getBit(0) == true) // Bao dam div va M luon >= 0
	{
		QInt A;
		//div = A-div;
		div = plusQInt(A, div.toInverse());
	}
	if (M.getBit(0) == true)
	{
		QInt A;
		//M = A-M;
		M = plusQInt(A, M.toInverse());
	}
	for (int i = 0; i < QINT_BIT_SIZE; i++) // Thuc hien chia theo thuat toan trong tai lieu.
	{
		mod = mod << 1;
		mod.setBit(QINT_BIT_SIZE - 1, div.getBit(0)); // Copy MSB cua Q vao LSB cua A.
		div = div << 1;
		//QInt tmp = mod - M;
		QInt tmp = plusQInt(mod, M.toInverse());
		if (tmp.getBit(0) == false) // tmp >= 0
		{
			mod = tmp;
			div.setBit(QINT_BIT_SIZE - 1, 1);
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

string binToDec(const QInt& src)//hàm chuyển đổi từ dãy nhị phân sang thập phân
{
	string res = "";//biến lưu kết quả
	if (src == QInt::Zero())//nếu chuỗi là 0
	{
		res = "0";//gán res = 0 và trả về res
		return res;
	}

	QInt tempSrc = src;//gán src vào 1 QInt tạm
	if (src.isNegative())//Nếu là số âm
	{
		tempSrc = tempSrc.toInverse();//chuyển qua bù 2 để thành số dương
	}

	string binStr = tempSrc.toBinStr();//chuyển từ dạy nhị phân trong QInt sang chuỗi nhị phân
	int pos = binStr.find('1', 0) + 1;//Tìm vị trí của bit 1 đầu tiên trong chuỗi
	res = "1";//
	while (pos < binStr.length())//duyệt qua từng phần tử trong chuỗi binStr
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
	string stringSrc = src.toBinStr();
	int num = stringSrc.length() % 4;
	while (num != 0 && num < 4)
	{
		stringSrc = '0' + stringSrc;
		num++;
	}
	int L = stringSrc.length();
	for (int i = 0; i < L; i += 4)
	{
		int pos = bitset<4>(stringSrc.substr(i, 4)).to_ulong();
		res += HEX_CHAR[pos];
	}
	while (res[0] == '0' && res.length() > 1)
		res.erase(0, 1);
	return res;
}
QInt plusQInt(const QInt& A, const QInt& B)
{
	QInt res;
	bool carry = 0;
	for (int i = 127; i >= 0; i--)
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
	return ((this->getBit(0)) == 1);
}

QInt QInt::toInverse() const
{
	if (*this == QInt::Zero())
		return *this;

	QInt res;

	//Dao bit
	res = ~(*this);

	//+1
	for (int i = DATA_ARR_SIZE - 1; i >= 0; i--)
	{
		if (res.dataArr[i] < LLONG_MAX)
		{
			res.dataArr[i] += 1;
			break;
		}
		else
		{
			res.dataArr[i] = 0;
		}	
	}
	return res;
}

void QInt::scanQInt(string src, int base)
{
	int bitPos = QINT_BIT_SIZE - 1;
	switch (base)
	{
	case 2:
		*this = QInt::Zero();
		for (int i = src.length() - 1; i >= 0; i--)
		{
			setBit(bitPos, src[i] == '1');
			bitPos--;
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
	for (int i = 0; i < DATA_ARR_SIZE; i++)
	{
		res += bitset<LLONG_BIT_SIZE>(this->dataArr[i]).to_string();
	}
	return res;
}
