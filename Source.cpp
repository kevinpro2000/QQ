#define _CRT_SECURE_NO_WARNINGS
#include "QInt.h"


void process()//hàm thực hiện chương trình
{
	string unaryOperators = "~,rol,ror";//Khởi tạo chuỗi toán tử 1 ngôi
	string binaryOperators = "+,-,*,/,&,|,^,<<,>>";//Khởi tạo chuỗi toán tử 2 ngôi
	QInt QIntNumber1, QIntNumber2;//2 biến lưu 2 số QInt
	int shiftNum = 0;//biến lưu số lần dịch 
	while (!cin.eof())//đọc đến hết file
	{
		string src = "", temp = "";//Khởi tạo biến src để lưu từng dòng đọc được, biến tạm temp
		string index1 = "", index2 = "";//Khởi tạo 2 biến index1, index2 để lưu chỉ thị (2, 10 hay 16)
		string unaOpr = "", binOpr = "";//Khởi tạo biến unaOpr để lưu toán tử 1 ngôi, binOpr lưu toán tử 2 ngôi
		string num1 = "", num2 = "";//Khởi tạo 2 biến num1, num2 để lưu 2 toán hạng
		getline(cin, src);//đọc vào từng dòng 
		if (src == "")//nếu dòng đọc vào là rỗng thì break
			break;
		int blankPos = src.find(' ');//tìm vị trí phần tử ' ' đầu tiên trong chuỗi
		while (blankPos != -1)//Lặp đến khi không còn phần tử ' ' nào nữa
		{
			temp = src.substr(0, blankPos);//tách từ đầu tiên trong chuỗi và gán vào temp
			if (temp == "2" || temp == "10" || temp == "16")//nếu temp là chỉ thị 2, 10 hoặc 16
			{
				if (index1 == "")//nếu index1 đang trống
					index1 += temp;//gán index1 bằng temp
				else//ngược lại tức đã có index1
					index2 += temp;//gán index2 bằng temp
			}
			else if (binaryOperators.find(temp) != -1)//nếu temp là toán tử 2 ngôi
			{
				binOpr += temp;//lưu temp vào binOpr
			}
			else if (unaryOperators.find(temp) != -1)//nếu temp là toán tử 1 ngôi
			{
				unaOpr += temp;//lưu temp vào unaOpr
			}
			else//còn lại trường hợp temp là toán hạng
			{
					num1 += temp;//lưu temp vào num1
			}
			src = src.erase(0, blankPos + 1);//sau mỗi lần tách 1 từ thì ta xóa từ đó đi khỏi chuỗi src
			blankPos = src.find(' ');//tìm phần tử ' ' tiếp theo trong chuỗi
		}
		if (num1 == "")//nếu tách hết từ trong chuỗi mà vẫn chưa có num1, tức trong chuỗi chỉ có 1 toán hạng
			num1 += src;//gán chuỗi còn lại cho num1
		else//ngược lại nếu đã có num1 thì chuỗi còn lại trong src là num2
			num2 += src;//gán chuỗi còn lại trong src cho num2
		//Phân loại phép toán
		if (index1 != "")//Nếu có chỉ thị index1 (2, 10 hay 16)
		{
			if (num1 != "")
				QIntNumber1.scanQInt(num1, stoi(index1));//đọc chuỗi num1 vào QIntNumber1 dưới dạng chỉ thị index1
			if (num2 != "")
			{
				if (binOpr != "<<" && binOpr != ">>")//nếu toán tử 2 ngôi không phải là toán tử dịch bit
					QIntNumber2.scanQInt(num2, stoi(index1));//đọc chuỗi num2 vào QIntNumber2 dưới dạng chỉ thị index2
				else//nếu toán tử 2 ngôi là toán tử dịch bit 
					shiftNum = stoi(num2);//ta lưu số bit cần dịch vào biến shiftNum
			}
		}
		if (index2 != "")//nếu tồn tại chỉ thị 2, tức đây là phép toán chuyển đổi từ kiểu index1 sang index2
		{
			QIntNumber1.printQInt(stoi(index2));//in số QIntNumber1 dưới dạng index2
		}
		else if (unaOpr != "")//nếu là toán tử 1 ngôi
		{
			if (unaOpr == "~")//nếu là toán tử ~
				(~QIntNumber1).printQInt(stoi(index1));//gọi toán tử ~ và in ra
			else if (unaOpr == "rol")//nếu là toán tử rol
				(QIntNumber1.rol()).printQInt(stoi(index1));//gọi toán tử rol và in ra
			else//còn lại trường hợp toán tử ror 
				(QIntNumber1.ror()).printQInt(stoi(index1));//gọi toán tử ror và in ra
		}
		else if (binOpr != "")//nếu là toán tử 2 ngôi, kiểm tra xem là toán tử nào để gọi hàm tương ứng và in ra
		{
			if (binOpr == "+")
				(QIntNumber1 + QIntNumber2).printQInt(stoi(index1));
			else if (binOpr == "-")
				(QIntNumber1 - QIntNumber2).printQInt(stoi(index1));
			else if (binOpr == "*")
				(QIntNumber1 * QIntNumber2).printQInt(stoi(index1));
			else if (binOpr == "/")
				(QIntNumber1 / QIntNumber2).printQInt(stoi(index1));
			else if (binOpr == "&")
				(QIntNumber1 & QIntNumber2).printQInt(stoi(index1));
			else if (binOpr == "|")
				(QIntNumber1 | QIntNumber2).printQInt(stoi(index1));
			else if (binOpr == "^")
				(QIntNumber1 ^ QIntNumber2).printQInt(stoi(index1));
			else if (binOpr == "<<")
				(QIntNumber1 << shiftNum).printQInt(stoi(index1));
			else if (binOpr == ">>")
				(QIntNumber1 >> shiftNum).printQInt(stoi(index1));
		}
		cout << endl;
	}
}

int main(int argc, char** argv)
{
	if (argc < 3) return 0;
	freopen(argv[1], "rt", stdin);//mở file input
	freopen(argv[2], "wt", stdout);//mở file ouput
	process();//gọi hàm thực hiện chương trình
	return 1;
}