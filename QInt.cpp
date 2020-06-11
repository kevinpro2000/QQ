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

string mulByTwo(string src, int additive)//Hàm nhân chuỗi lên 2
{
	string res = "";//biến lưu kết quả
	for (int i = src.length() - 1; i >= 0; i--)//Kiểm tra từ phải sang trái chuỗi 
	{
		int temp = src[i] - '0';//Chuyển src[i] từ char sang int rồi gán vào temp
		temp = temp * 2 + additive;
		res = (char)(temp % 10 + '0') + res;
		additive = temp / 10;
	}
	if (additive > 0)
		res = (char)(additive + '0') + res;
	return res;
}
QInt QInt::rol() const//Hàm quay trái
{
	QInt res = *this;//Biến kết quả
	bool additive = ((res.dataArr[0] >> 63) & 1);//Lấy giá trị của bit dấu lưu vào additive
	res.dataArr[0] = res.dataArr[0] << 1;//Dịch 64 bit cuối qua bên trái 1 lần
	res.setBit(63, res.getBit(64));//lưu bit cuối của dataArr[1] vào bit đầu của dataArr[0] 
	res.dataArr[1] = res.dataArr[1] << 1;//Dịch 64 bit đầu qua bên trái 1 lần
	if (additive)//Gán lại giá trị cho bit dấu
		res.setBit(127, additive);
	return res;
}

QInt QInt::ror() const//Hàm quay phải
{
	QInt res = *this;//Biến kết quả
	bool additive = (res.dataArr[1] & 1);//Lấy giá trị của bit dấu lưu vào additive
	res.dataArr[1] = res.dataArr[1] >> 1;//Dịch 64 bit đầu qua bên phải 1 lần
	res.setBit(64, res.getBit(63));//lưu bit đầu của dataArr[0] vào bit cuối của dataArr[1] 
	res.dataArr[0] = res.dataArr[0] >> 1;//Dịch 64 bit đầu qua bên phải 1 lần
	if (additive)//Gán lại giá trị cho bit dấu
		res.setBit(0, additive);
	return res;
}

//Ham ho tro / 2 so QInt
void QInt::divide(const QInt& divisor, QInt& div, QInt& mod) const
{
	mod = QInt::Zero();//Biến lưu giá trị dư trong phép chia
	div = *this;//Biến
	QInt M = divisor;//Lưu lại số chia
	// Luu lai dau cua phep chia.
	bool isDNegative = div.isNegative();//Biến lưu dấu của thương
	bool isMNegative = M.isNegative();//Biến lưu dấu của số chia
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
	for (int i = 0; i < QINT_BIT_SIZE; i++) // Thuc hien chia theo thuat toan
	{
		mod = mod << 1;//Dịch trái số dư 1 lần
		mod.setBit(QINT_BIT_SIZE - 1, div.getBit(0)); // Copy MSB cua Q vao LSB cua A.
		div = div << 1;//Dịch trái thương 1 lần
		//QInt tmp = mod - M;
		QInt tmp = plusQInt(mod, M.toInverse());
		if (tmp.getBit(0) == false) // tmp >= 0
		{
			mod = tmp;
			div.setBit(QINT_BIT_SIZE - 1, 1);//MSB trả về bằng 1
		}
	}

	if (isDNegative)		//So du va so bi chia cung dau
		mod = mod.toInverse();
	if (isDNegative + isMNegative == 1) //Neu so bi chia va so chia trai dau thi doi dau thuong
		div = div.toInverse();
}

QInt decToBin(string src)
{
	QInt res;//Biến kết quả
	bool isNegative = false;//Biến dấu
	if (src[0] == '-')//Kiểm tra xem chuỗi dec là âm hay dương
	{
		isNegative = true;
		src.erase(0, 1);//Nếu âm thì xóa bớt dấu trừ để dễ tho tác
	}

	string binStr = "";//Biên lưu chuỗi nhị phân
	do
	{
		int lastDigit = src[src.length() - 1] - '0';//Lấy số hàng đơn vị trong chuỗi thập phân
		binStr += ((lastDigit % 2 == 0) ? '0' : '1');//Chia số đó cho 2 và lưu số dư vào binStr 
		src = divByTwo(src);//Chia hai chuỗi src
	} while (src != "0");//Kiểm tra src bằng 0 hay chưa

	reverse(binStr.begin(), binStr.end());//Đảo lại chuỗi nhị phân
	res.scanQInt(binStr, 2);//Lưu chuỗi nhị phân vào kết quả
	if (isNegative)//Nếu là số âm thì bù 2 kết quả
		res = res.toInverse();
	return res;
}

QInt hexToBin(string src)//Chuyễn chuỗi HEX thành chuỗi BIN
{
	QInt res;//Biến kết quả
	string binStr = "";//Biến lưa chuỗi nhị phân
	for (int i = 0; i < src.length(); i++)//Tiến hành quét hết chuỗi src từ trái qua phải
	{
		int pos = ((string)HEX_CHAR).find(src[i], 0);//Lấy giá trị thập phân trong chuỗi src
		binStr += bitset<4>(pos).to_string();//Chuyển giá trị thập phân thành nhị phân
	}
	res.scanQInt(binStr, 2);//Lưu chuỗi nhị phân vào biến kết quả
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
		int additive = binStr[pos] - '0';//Chuyển binStr[pos] từ char sang int rồi lưu vào additive
		res = mulByTwo(res, additive);//Nhân hai biến kết quả
		pos++;//Duyệt phần tử tiếp theo
	}

	if (src.isNegative())//Nếu là số âm thì thêm dấu trừ
		res = '-' + res;

	return res;
}

string binToHex(const QInt& src)//chuyển chuỗi BIN sang chuỗi HEX
{
	string res = "";//Biến kết quả
	string stringSrc = src.toBinStr();//Lấy chuỗi nhị phân từ src
	int L = stringSrc.length();//Độ dài chuỗi
	for (int i = 0; i < L; i += 4)//Quét qua mỗi 4 kí tự trong chuỗi nhị phân
	{
		int pos = bitset<4>(stringSrc.substr(i, 4)).to_ulong();//Chuyển 4 kí tự vừa quét sang giá trị thập phân
		res += HEX_CHAR[pos];//Chuyển sang hệ HEX
	}
	while (res[0] == '0' && res.length() > 1)//Xóa bớt số 0 thừa phái trước chuỗi nhị phân
		res.erase(0, 1);
	return res;
}
QInt plusQInt(const QInt& A, const QInt& B)//Hàm cộng 2 số QInt
{
	QInt res;//Biến kết quả
	bool carry = 0;//Biến nhớ
	for (int i = 127; i >= 0; i--)//Quét qua từng bit trong QInt
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

string Upstring(string a)//Hàm in hoa chuỗi
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

bool QInt::isNegative() const//Hàm kiểm tra QInt có âm không
{
	return ((this->getBit(0)) == 1);
}

QInt QInt::toInverse() const//Hàm bù 2
{
	if (*this == QInt::Zero())//Nếu QInt=0 thì trả về 0
		return *this;

	QInt res;//Biến kết quả

	//Dao bit
	res = ~(*this);

	//+1
	for (int i = DATA_ARR_SIZE - 1; i >= 0; i--)//Quét qua mỗi 64 bit trong QInt
	{
		if (res.dataArr[i] < LLONG_MAX)//Nếu 64 bit thứ i nhỏ hơn giá trị lớn nhất của số long long thì cộng thêm 1
		{
			res.dataArr[i] += 1;
			break;
		}
		else//Nếu 64 bit thứ i lớn hơn giá trị lớn nhất của số long long thì mang giá trị 0
		{
			res.dataArr[i] = 0;
		}	
	}
	return res;
}

void QInt::scanQInt(string src, int base)//Hàm đọc chuỗi đầu vào
{
	int bitPos = QINT_BIT_SIZE - 1;//Vị trí bit tính từ trái sang phải
	switch (base)
	{
	case 2://Hệ nhị phân
		*this = QInt::Zero();
		for (int i = src.length() - 1; i >= 0; i--)//Quét qua từng kí tự trong chuỗi
		{
			setBit(bitPos, src[i] == '1');//Gán giá trị vào từng bit trong số QInt
			bitPos--;
		}
		break;
	case 10://Hệ thập phân
		*this = decToBin(src);
		break;
	case 16://Hệ thập lục
		*this = hexToBin(src);
	}
}

void QInt::printQInt(int base)//Hàm in số QInt
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

string QInt::toBinStr() const//Hàm chuyển số QInt thành chuỗi nhị phân
{
	string res = "";
	for (int i = 0; i < DATA_ARR_SIZE; i++)
	{
		res += bitset<LLONG_BIT_SIZE>(this->dataArr[i]).to_string();
	}
	return res;
}
