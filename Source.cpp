#define _CRT_SECURE_NO_WARNINGS
#include "QInt.h"


void process()
{
	string unaryOperators = "~,rol,ror";
	string binaryOperators = "+,-,*,/,&,|,^,<<,>>";
	QInt QIntNumber1, QIntNumber2;
	int shiftNum = 0;
	while (!cin.eof())
	{
		string src = "", temp = "";
		string index1 = "", index2 = "";
		string unaOpr = "", binOpr = "";
		string num1 = "", num2 = "";
		getline(cin, src);
		if (src == "")
			break;
		int blankPos = src.find(' ');
		while (blankPos != -1)
		{
			temp = src.substr(0, blankPos);
			if (temp == "2" || temp == "10" || temp == "16")
			{
				if (index1 == "")
					index1 += temp;
				else
					index2 += temp;
			}
			else if (binaryOperators.find(temp) != -1)
			{
				binOpr += temp;
			}
			else if (unaryOperators.find(temp) != -1)
			{
				unaOpr += temp;
			}
			else
			{
					num1 += temp;
			}
			src = src.erase(0, blankPos + 1);
			blankPos = src.find(' ');
		}
		if (num1 == "")
			num1 += src;
		else
			num2 += src;
		//Phân loại phép toán
		if (index1 != "")
		{
			if (num1 != "")
				QIntNumber1.scanQInt(num1, stoi(index1));
			if (num2 != "")
			{
				if (binOpr != "<<" && binOpr != ">>")
					QIntNumber2.scanQInt(num2, stoi(index1));
				else
					shiftNum = stoi(num2);
			}
		}
		if (index2 != "")
		{
			QIntNumber1.printQInt(stoi(index2));
		}
		else if (unaOpr != "")
		{
			if (unaOpr == "~")
				(~QIntNumber1).printQInt(stoi(index1));
			else if (unaOpr == "rol")
				(QIntNumber1.rol()).printQInt(stoi(index1));
			else
				(QIntNumber1.ror()).printQInt(stoi(index1));
		}
		else if (binOpr != "")
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
	freopen(argv[1], "rt", stdin);
	freopen(argv[2], "wt", stdout);
	process();
	return 1;
}