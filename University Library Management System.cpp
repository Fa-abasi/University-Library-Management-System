/* University Library Management System */
/*Fatemeh Abbasi*/

#include "stdafx.h"
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<conio.h>
#include<fstream>
#include<sstream>// for stringstream
using namespace std;
//For simplicity, the file names are enclosed in a variable
string MemberFile = "Member.csv";
string LibrarianFile = "Librarian.csv";
string BookFile = "Book.csv";
string MagazineFile = "Magazine.csv";
string ThesisFile = "Thesis.csv";


long int get_size_file(string filename)
{
	long int size = 0;
	ifstream ifile(filename);
	ifile.seekg(0, ios::end);
	size = ifile.tellg();
	return size;

}



class BadArgument {//For invalid data
private:
	string message;
public:
	BadArgument(string ms) : message(ms) {}
	~BadArgument() {}
	//getter function
	string getMessage()const { return message; }
};
class Invalid {//For invalid data
private:
	string message;
public:
	Invalid(string ms) : message(ms) {}
	~Invalid() {}
	//getter function
	string getMessage()const { return message; }
};
class Error {//For invalid data
private:
	string message;
public:
	Error(string ms) : message(ms) {}
	~Error() {}
	//getter function
	string getMessage()const { return message; }
};


vector<vector<string>> getData(string Filename)
{/*It stores all the file information in a two-dimensional vector
 and returns it as the output of the function.*/
	ifstream input(Filename, ios::in);
	if (!input)
		throw invalid_argument("");
	vector<vector<string>> dataList;
	vector <string> vec;
	string line, word;
	while (getline(input, line))
	{
		vec.clear();
		stringstream splits(line);
		while (getline(splits, word, ',')) {
			vec.push_back(word);
		}

		dataList.push_back(vec);
	}
	input.close();
	return dataList;
}

bool find(string Filename, string requested, int index) {
	/*Searches the desired data in the file and returns 1 if found,
	and 0 if not.*/
	try {
		vector<vector<string>> v;
		v = getData(LibrarianFile);
		for (int i = 0; i < v.size(); i++)
		{

			if (v[i][index] == requested)
				return 1;

		}

		return 0;



		/*vector<vector<string>> list = getData(Filename);
		for (vector<string> vec : list) {
		if (vec[index] == requested)
		return true;
		}
		return false;*/
	}
	catch (invalid_argument) {
		return false;
	}



}

string get_password() {
	char ch;
	string pass;

	ch = _getch();
	//Get an ID
	while (ch != 13)//Enter ski code = 13
	{

		if (ch == 8) {
			cout << "\b \b";
			pass.pop_back();

		}

		if (ch != 8)// backspace ski code = 8
		{
			cout << "*";

			pass.push_back(ch);

		}



		if (ch == 13)
			break;

		ch = _getch();
		ch = _getch();
	}

	return pass;
}

class Document {
protected:
	string numberPages;
protected:
	bool IsValidArgument(string argument) {/*This function checks whether the entered argument is made up
										   entirely of letters of the alphabet*/
		int size = argument.length();
		for (int i = 0; i < size; i++)
		{
			if (!isalpha(argument[i]))
				return false;
		}
		return true;
	}
public:
	Document(string numberPages = "notdefined")
	{
		setNumberPages(numberPages);
	}
	~Document() {}
	//setter function
	void  setNumberPages(string numberPages) {
		bool again = true;
		while (again) {
			try {
				int y = numberPages.size();
				for (int i = 0; i < y; i++)
				{
					if (!isdigit(numberPages[i]))
						throw BadArgument("The numberPages consists only of numbers!!!");
				}
				if (numberPages <= "0")
					throw BadArgument("The number of pages cannot be zero or negative!");

				this->numberPages = numberPages;
				again = false;
			}
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the number of pages : ";
				cin >> numberPages;
			}
		}
	}
	//getter function
	virtual string  getID() = 0;
	string getNumberPages() const { return numberPages; }
	virtual void report()const = 0;
};

class Book :public Document {
	//Fields are moved from the object into the file
	friend ostream & operator << (ostream &output, const Book & object)
	{
		output << object.ID << "," << object.Title << "," << object.numberPages << "," << object.Category
			<< "," << to_string(object.numberBooks) << "," << object.Inventory << "\n";
		return output;
	}
	//Fields are moved from the file into the object
	friend istream & operator >> (istream &input, Book & object)
	{

		string word;
		string line;
		getline(input, line);
		stringstream split(line);
		getline(split, word, ',');
		object.ID = word;
		getline(split, word, ',');
		object.Title = word;
		getline(split, word, ',');
		object.numberPages = word;
		getline(split, word, ',');
		object.Category = word;
		getline(split, word, ',');
		object.numberBooks = stoi(word);
		getline(split, word, ',');
		object.Inventory = stoi(word);
		return input;
	}
private:
	string ID;
	string Title;
	string Category;//(Engineering-Basic-Literature-Theology-other)
	int numberBooks;//original number of books
	int Inventory;
public:
	Book(string ID = "12345678", string Title = "notdefined", string numberPages = "10", int numberBooks = 0, int Inventory = 0) :Document(numberPages)
	{
		setID(ID);
		setTitle(Title);
		//setCategory();
		Category = "notdefined";
		setNumberBooks(numberBooks);
		setInventory(Inventory);
	}
	~Book() {}
	//setter functions
	void setID(string ID)
	{
		bool again = true;
		while (again) {
			try {
				int size = ID.length();
				for (int i = 0; i < size; i++)
				{
					if (!isdigit(ID[i]))
						throw BadArgument("The ID consists only of numbers!!!");
				}
				if (ID < "0")
					throw BadArgument("The ID entered is not in the correct range!");
				if (size != 8)
					throw BadArgument("The limit of ID is 8 digits");

				if (find(BookFile, ID, 0))
					throw BadArgument("This ID is available, the ID must be unique");
				/*
				else if (string(typeid(*P).name()) == "class Magazine")
				{
				if (find(MagazineFile, ID, 1))
				throw "This ID is available, the ID must be unique";
				}
				else if (string(typeid(*P).name()) == "class Thesis")
				{
				if (find(ThesisFile, ID, 1))
				throw "This ID is available, the ID must be unique";
				}*/
				this->ID = ID;
				again = false;

			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter ID (The limit of ID is 8 digits) : ";
				cin >> ID;
			}
		}//end of while
	}//end of function
	void setTitle(string Title) {
		bool again = true;
		while (again) {
			try {
				if (find(BookFile, Title, 1))
				{
					numberBooks++;
					cout << "This book is available in the library" << endl;
					cout << "Book Name : " << Title << "     " << "Number in the library : " << numberBooks << endl;
					return;
				}
				else
				{
					int size = Title.length();
					for (int i = 0; i < size; i++) {
						if (ispunct(Title[i]))/*If it is not made up of letters and numbers,
											  it returns 0, otherwise it returns 1*/
							throw BadArgument("The title is not valid!! The title can only contain letters and numbers");
					}
					this->Title = Title;
					again = false;
				}//end of else
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the book title : ";
				cin >> Title;
			}
		}//end of while

	}
	void setCategory() {
		bool again = true;
		while (again) {
			int ans;
			cout << "Select the desired book category" << endl;
			cout << "1.Engineering" << "\t2.the base" << "\t3.the literature" << "\t4.Theology" << "\t5.Other" << endl;
			cin >> ans;
			again = false;
			switch (ans)
			{
			case 1:
				Category = "Engineering";
				break;
			case 2:
				Category = "base";
				break;
			case 3:
				Category = "literature";
				break;
			case 4:
				Category = "Theology";
				break;
			case 5:
				Category = "Other";
				break;
			default:
				again = true;
				cout << "The number entered is incorrect!" << endl;
				break;
			}
		}
	}
	void  setNumberBooks(int numberBooks) {
		bool again = true;
		while (again) {
			try {
				if (numberBooks < 0)
					throw BadArgument("The number of books can not be negative!!");
				else {
					this->numberBooks = numberBooks;
					again = false;
				}
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the number of books : ";
				cin >> numberBooks;
			}
		}//end of while
	}
	void setInventory(int Inventory) {
		bool again = true;
		while (again) {
			try {
				if (Inventory < 0)
					throw BadArgument("Inventory cannot be negative!");
				if (Inventory > numberBooks)
					throw BadArgument("Inventory is more than the number of books!!");
				this->Inventory = Inventory;
				again = false;
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the book balance to borrow : ";
				cin >> Inventory;
			}
		}//end of while
	}
	//getter functions
	string getID() override { return ID; }
	string getTitle() const {

		return Title;
	}
	string getCategory()const {
		return Category;
	}
	int getNumberBooks()const {
		return numberBooks;
	}
	int getInventory() const { return Inventory; }

	void report()const override {
		cout << setw(12) << Title << setw(10) << ID << setw(4) << numberPages
			<< setw(12) << Category << setw(4) << numberBooks << setw(4) << Inventory << endl;
	}

};

class Magazine :public Document {
	//Fields are moved from the object into the file
	friend ostream & operator << (ostream &output, const Magazine & object)
	{
		output << object.ID << "," << object.Title << "," << object.numberPages << "," << to_string(object.numberMagazines)
			<< "," << object.Inventory << "\n";
		return output;
	}
	//Fields are moved from the file into the object
	friend istream & operator >> (istream &input, Magazine & object)
	{

		string word;
		string line;
		getline(input, line);
		stringstream split(line);
		getline(split, word, ',');
		object.ID = word;
		getline(split, word, ',');
		object.Title = word;
		getline(split, word, ',');
		object.numberPages = word;
		getline(split, word, ',');
		object.numberMagazines = stoi(word);
		getline(split, word, ',');
		object.Inventory = stoi(word);
		return input;
	}
private:
	string ID;
	string Title;
	int numberMagazines;//original number of Magazines
	int Inventory;
public:
	Magazine(string ID = "01234567", string Title = "notdefined", string numberPages = "10", int numberMagazines = 0, int Inventory = 0) :Document(numberPages)
	{
		setID(ID);
		setTitle(Title);
		setNumberMagazine(numberMagazines);
		setInventory(Inventory);
	}
	~Magazine() {}
	//setter functions
	void setID(string ID)
	{
		bool again = true;
		while (again) {
			try {
				int size = ID.length();
				for (int i = 0; i < size; i++)
				{
					if (!isdigit(ID[i]))
						throw BadArgument("The ID consists only of numbers!!!");
				}
				if (ID < "0")
					throw BadArgument("The ID entered is not in the correct range!");
				if (size != 8)
					throw BadArgument("The limit of ID is 8 digits");



				if (find(MagazineFile, ID, 0))
					throw BadArgument("This ID is available, the ID must be unique");

				this->ID = ID;
				again = false;

			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter ID (The limit of ID is 8 digits) : ";
				cin >> ID;
			}
		}//end of while
	}//end of function
	void setTitle(string Title) {
		bool again = true;
		while (again) {
			try {
				if (find(MagazineFile, Title, 1)) {
					numberMagazines++;
					cout << "This Magazine is available in the library" << endl;
					cout << "Magazine Name : " << Title << "     " << "Number in the library : " << numberMagazines << endl;
					return;
				}
				else {



					int size = Title.length();
					for (int i = 0; i < size; i++) {
						if (ispunct(Title[i]))
							throw BadArgument("The title is not valid!! The title can only contain letters and numbers");
					}
					this->Title = Title;
					again = false;
				}//end of else
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the Magazines title : ";
				cin >> Title;
			}
		}//end of while
	}

	void  setNumberMagazine(int numberMagazines) {
		bool again = true;
		while (again) {
			try {
				if (numberMagazines < 0)
					throw BadArgument("The number of books can not be negative!!");
				else {
					this->numberMagazines = numberMagazines;
					again = false;
				}
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the number of Magazines : ";
				cin >> numberMagazines;
			}
		}//end of while
	}

	void setInventory(int Inventory) {
		bool again = true;
		while (again) {
			try {
				if (Inventory < 0)
					throw BadArgument("Inventory cannot be negative!");
				if (Inventory > numberMagazines)
					throw BadArgument("Inventory is more than the number of Magazines!!");
				this->Inventory = Inventory;
				again = false;
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Magazine inventory (for lending) : ";
				cin >> Inventory;
			}
		}//end of while
	}

	//getter functions
	string getID() override { return ID; }
	string getTitle() const {

		return Title;
	}

	int getNumberMagazines()const {
		return numberMagazines;
	}

	int getInventory() const { return Inventory; }

	void report()const override {
		cout << setw(14) << Title << setw(10) << ID << setw(4) << numberPages
			<< setw(4) << numberMagazines << setw(4) << Inventory << endl;
	}

};

class Thesis :public Document {
	//Fields are moved from the object into the file
	friend ostream & operator << (ostream &output, const Thesis & object)
	{
		output << object.ID << "," << object.Title << "," << object.numberPages << "," << object.Grade
			<< "," << object.College << "," << object.group << "," << object.Inventory << "\n";
		return output;
	}
	//Fields are moved from the file into the object
	friend istream & operator >> (istream &input, Thesis & object)
	{
		string word;
		string line;
		getline(input, line);
		stringstream split(line);
		getline(split, word, ',');
		object.ID = word;
		getline(split, word, ',');
		object.Title = word;
		getline(split, word, ',');
		object.numberPages = word;
		getline(split, word, ',');
		object.Grade = word;
		getline(split, word, ',');
		object.group = word;
		getline(split, word, ',');
		object.College = word;
		getline(split, word, ',');
		object.Inventory = stoi(word);


		return input;
	}
private:
	string ID;
	string Title;
	string Grade;
	string College;
	string group;
	bool Inventory;//if available = 1 ;  if on loan = 0
public:
	Thesis(string ID = "21345687",
		string Title = "notdefined",
		string numberPages = "10",
		string Grade = "notdefined",
		string College = "notdefined",
		string group = "notdefined",
		bool Inventory = false) :Document(numberPages)
	{
		setID(ID);
		setTitle(Title);
		setGrade(Grade);
		setCollege(College);
		setGroup(group);
		setInventory(true);
	}
	~Thesis() {}
	//setter functions
	void setID(string ID)
	{
		bool again = true;
		while (again) {
			try {
				int size = ID.length();
				for (int i = 0; i < size; i++)
				{
					if (!isdigit(ID[i]))
						throw BadArgument("The ID consists only of numbers!!!");
				}
				if (ID < "0")
					throw BadArgument("The ID entered is not in the correct range!");
				if (size != 8)
					throw BadArgument("The limit of ID is 8 digits");




				if (find(ThesisFile, ID, 0))
					throw BadArgument("This ID is available, the ID must be unique");

				this->ID = ID;
				again = false;

			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter ID (The limit of ID is 8 digits) : ";
				cin >> ID;
			}
		}//end of while
	}//end of function
	void setTitle(string Title) {
		bool again = true;
		while (again) {
			try {
				if (find(ThesisFile, Title, 2))
					throw BadArgument("This Magazine is available in the library");
				int size = Title.length();
				for (int i = 0; i < size; i++) {
					if (ispunct(Title[i]))
						throw BadArgument("The title is not valid!! The title can only contain letters and numbers");
				}
				this->Title = Title;
				again = false;

			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the Thesis title : ";
				cin >> Title;
			}
		}//end of while
	}

	void setGrade(string Grade) {
		bool again = true;
		while (again) {
			try {
				if (!IsValidArgument(Grade))
					throw BadArgument("The Grade entered must contain only letters!");
				this->Grade = Grade;
				again = false;
			}
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the Thesis Grade : ";
				cin >> Grade;
			}
		}//end of while
	}

	void setCollege(string College) {
		bool again = true;
		while (again) {
			try {
				if (!IsValidArgument(College))
					throw BadArgument("The College entered must contain only letters!");
				this->College = College;
				again = false;
			}
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the Thesis College : ";
				cin >> College;
			}
		}//end of while
	}

	void setGroup(string group) {
		bool again = true;
		while (again) {
			try {
				if (!IsValidArgument(group))
					throw BadArgument("The Group entered must contain only letters!");
				this->group = group;
				again = false;
			}
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter the Thesis Group : ";
				cin >> group;
			}
		}//end of while
	}

	void setInventory(bool Inventory) {
		this->Inventory = Inventory;
	}

	//getter functions
	string getID()override { return ID; }
	string getTitle()const { return Title; }
	string getGrade()const { return Grade; }
	string setCollege()const { return College; }
	string getGroup()const { return group; }
	bool getInventory()const { return Inventory; }

	void report()const override {
		cout << setw(10) << ID << setw(12) << Title << setw(4) << numberPages
			<< setw(12) << Grade << setw(12) << College << setw(12) << group
			<< setw(2) << Inventory << endl;
	}

};


class Birth {
	friend class trusteeship;
	//Fields are moved from the object into the file
	friend ostream & operator << (ostream &output, const Birth & object)
	{
		output << to_string(object.year) + "/" + to_string(object.month) + "/" + to_string(object.day);
		return output;
	}
	//Fields are moved from the file into the object
	friend istream & operator >> (istream &input, Birth & object)
	{
		string line;

		input >> line;
		string word;
		stringstream split(line);
		getline(split, word, '/');


		object.year = stoi(word);
		getline(split, word, '/');
		object.month = stoi(word);
		getline(split, word, '/');
		object.day = stoi(word);
		return input;
	}

	friend bool operator==(Birth b1, Birth b2) {
		if ((b1.day == b2.day) && (b1.month == b2.month) && (b1.year == b2.year))
			return true;
		else
			return false;



	}

private:
	int year;
	int month;
	int day;
	bool isLeapYear(int y)
	{
		int r = y % 33;
		return r == 1 || r == 5 || r == 9 || r == 13 || r == 17 || r == 22 || r == 26 || r == 30;
	}
	int days_of_month(int m, int y)
	{

		if (m < 7)
			return 31;
		if (m < 12)
			return 30;
		if (m == 12)
			return isLeapYear(y) ? 30 : 29;
	}
public:
	Birth(int year = 1399, int month = 1, int day = 1) {
		setDate(year, month, day);
	}
	~Birth() {}




	//setter function
	//Because setting the fields were related, only one setter was used.
	void setDate(int  year, int month, int day) { //The maximum age is 120 years
		bool again = true;
		while (again) {
			try {
				if (year < 1280 || month < 1 || month > 12 || day < 1 || day > days_of_month(month, year)) {
					string error = "The date entered to record the date of birth is not valid!!";
					throw error;
				}
				this->year = year;
				this->month = month;
				this->day = day;
				again = false;
			}
			catch (string message) {
				cout << message << endl;
				cout << "Enter your date of birth(respectively  year, month, day) :";
				cin >> year >> month >> day;
			}
		}
	}
	//getter function
	int getYear() const { return year; }
	int getMonth() const { return month; }
	int getDay() const { return day; }
	void printBirth() {
		cout << year << "/";

		cout << setfill('0') << setw(2) << month << "/";

		cout << setfill('0') << setw(2) << day;
	}
};
//The Person class is the parent of the Librarian class and Member class.
class Person {
protected:
	string fistName;
	string lastName;
	string nationalCode;
	Birth Date_of_birth;
protected:
	bool IsValidArgument(string argument) {/*This function checks whether the entered argument is made up
										   entirely of letters of the alphabet*/
		int size = argument.length();
		for (int i = 0; i < size; i++)
		{
			if (!isalpha(argument[i]))
				return false;
		}
		return true;
	}
	/*bool IsValidNcode(string nationalCode) {//The validity of the code is checked
	int number = stoi(nationalCode);
	int num[10];
	int i;
	for (i = 9; i >= 0; i--) {
	num[i] = number % 10;
	number = number / 10;
	}
	if (i >= 0)
	{
	for (; i < 0; i--)
	num[i] = 0;
	}
	int A = num[9];
	int sum = 0;

	for (int s = 0, k = 10; s < 9, k >1; s++, k--) {
	sum = (num[s] * k) + sum;
	//(اولین رقم * ۱۰) + ( دومین رقم * ۹ ) + ( سومین رقم * ۸ ) + ( چهارمین رقم * ۷ ) + ( پنجمین رقم * ۶) + ( ششمین رقم * ۵ ) + ( هفتمین رقم * ۴ ) + ( هشتمین رقم * ۳ ) + ( نهمین رقم * ۲ )
	}
	int B = sum;
	int C = B - (B / 11) * 11;
	if ((C == 0 && A == C) || (C == 1 && A == 1) || (C > 1 && A == C - 11))

	return true;

	else

	return false;

	}*/

	bool IsThereNcode(string nationalCode) {
		/*Searches the national code in both Member
		and Librarian files ،Returns 0 if found and otherwise 0*/

		if (find(MemberFile, nationalCode, 3))
			return 1;
		else if (find(LibrarianFile, nationalCode, 3))
			return 1;
		else {
			return 0;
		}
	}
public:
	Person(string firstName = "notdefined",
		string lastName = "notdefined",
		string nationalCode = "372184790",
		Birth bd = Birth(1399, 1, 1)) :Date_of_birth(bd) {
		setFistName(firstName);
		setLastName(lastName);
		setNationalCode(nationalCode);
		//Date_of_birth=bd;
	}

	~Person() {}
	//setter function
	/*The type of the called object class is specified and according to it,
	the file type is searched for the ID in it.*/

	/*This function performs the necessary validation for the firstName
	and retrieves it from the user in case of an error.*/
	void setFistName(string firstName) {

		bool again = true;
		while (again) {
			try {

				if (!IsValidArgument(firstName))
					throw BadArgument("The firstname entered must contain only letters!");



				fistName = firstName;
				again = false;
			}
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter your firstname : ";
				cin >> firstName;
			}
		}
	}
	/*This function performs the necessary validation for the lastName
	and retrieves it from the user in case of an error.*/
	void setLastName(string lastName) {
		bool again = true;
		while (again) {
			try {
				if (!IsValidArgument(lastName))
					throw BadArgument("The lastname entered must contain only letters!");
				this->lastName = lastName;
				//(*this).lastName = lastName;
				again = false;
			}
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter your lastName : ";
				cin >> lastName;
			}
		}
	}
	/*This function performs the necessary validation for the national Code
	and retrieves it from the user in case of an error.*/
	void setNationalCode(string nationalCode) {
		bool again = true;
		while (again) {
			try {
				int size = nationalCode.length();
				for (int i = 0; i < size; i++)
				{
					if (!isdigit(nationalCode[i]))
						throw BadArgument("The national code consists only of numbers!!!");
				}
				if (nationalCode[0] == '0')
					throw BadArgument("Enter the code without zero");
				if (size != 9)
					throw BadArgument("The national code contains 9 digits!");
				/*if (!IsValidNcode(nationalCode))
				throw  BadArgument("Your national code is not valid !!");*/
				if (IsThereNcode(nationalCode))
					throw BadArgument("This code has already been entered !!");

				this->nationalCode = nationalCode;
				//(*this).nationalCode=nationalCode;
				again = false;
			}
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter your national code : ";
				cin >> nationalCode;
			}
		}
	}

	void setBirth(Birth bd) {
		Date_of_birth = bd;
	}

	//getter function
	virtual string getID() = 0;
	string getFistName()const { return fistName; }
	string getLastName()const { return lastName; }
	string getNationalCode()const { return nationalCode; }
	Birth getBirth()const { return Date_of_birth; }

	virtual void report() = 0;


};
class Member : public Person {
	//Fields are moved from the object into the file
	friend ostream & operator << (ostream &output, const Member & object)
	{
		output << object.ID << "," << object.fistName << "," << object.lastName << "," << object.nationalCode
			<< "," << object.Date_of_birth << "," << object.Type << "," << object.Std_stf_number << "," << to_string(object.fine) << "\n";
		return output;
	}
	//Fields are moved from the file into the object
	friend istream & operator >> (istream &input, Member & object)
	{
		string word;
		string line;
		getline(input, line);
		stringstream split(line);
		getline(split, word, ',');
		object.ID = word;
		getline(split, word, ',');
		object.fistName = word;
		getline(split, word, ',');
		object.lastName = word;
		getline(split, word, ',');
		object.nationalCode = word;
		/*************************/
		getline(split, word, ',');



		int y, m, d;
		string wor;
		stringstream spleat(word);
		getline(spleat, wor, '/');
		y = stoi(wor);
		getline(spleat, wor, '/');
		m = stoi(wor);
		getline(spleat, wor, '/');
		d = stoi(wor);


		object.Date_of_birth = Birth(y, m, d);



		getline(split, word, ',');
		object.Type = word;

		getline(split, word, ',');
		object.Std_stf_number = word;

		getline(split, word, ',');
		object.fine = stoi(word);

		return input;
	}

private:
	string ID;
	string Type;//Student or staff
	string Std_stf_number;//Student or staff number
	int fine;
	/*This function specifies whether the user is a student or a staff member*/
	bool IsStudent() {
		while (true) {
			int ans;
			cout << "Choose your side" << endl;
			cout << "1. Student" << endl;
			cout << "2. staff" << endl;
			cin >> ans;
			if (ans == 1)
				return true;
			else if (ans == 2)
				return false;
			else
				cout << "Choose carefully!" << endl;
		}
	}
public:
	Member(string ID = "645321789", string fistName = "notdefined",
		string lastName = "notdefined",
		string nationalCode = "372184791",
		Birth bd = Birth(1399, 1, 1)
		, int fine = 0) :Person(fistName, lastName, nationalCode, bd) {
		setID(ID);
		setType();
		setNumber();
		setFine(fine);
	}

	~Member() {}
	//setter function
	//According to the IsStudent() function, sets the user type
	void setID(string ID)
	{
		bool again = true;
		while (again) {
			try {
				int size = ID.length();
				for (int i = 0; i < size; i++)
				{
					if (!isdigit(ID[i]))
						throw BadArgument("The ID consists only of numbers!!!");
				}
				if (size != 8)
					throw BadArgument("The limit of ID is 8 digits");




				if (find(MemberFile, ID, 0))
					throw BadArgument("This ID is available, the ID must be unique");




				this->ID = ID;
				again = false;

			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter your ID (The limit of ID is 8 digits) : ";
				cin >> ID;
			}
		}//end of while
	}//end of function
	void setType() {
		if (IsStudent())
			Type = "Student";
		else if (!IsStudent())
			Type = "Staff";
	}
	/*Takes the student / staff number from the user, validates it
		and asks the user to log in again in case of an error.*/
	void setNumber() {//set Student or staff number
		string number;
		bool again = true;
		while (true) {
			try {
				if (Type == "Student") {
					cout << "Please enter your student number :";
					cin >> number;
				}
				else if (Type == "Staff") {
					cout << "Please enter your personal code :";
					cin >> number;
				}

				int size = number.length();
				for (int i = 0; i < size; i++)
				{
					if (!isdigit(number[i]))
						throw BadArgument("The number consists only of numbers!!!");
				}
				if (number < "0")
					throw BadArgument("The number entered is not in the correct range!");

				if (size > 10)
					throw BadArgument("The number of ID is 10 digits");

				if (find(MemberFile, number, 6))
					throw BadArgument("This number is available !!");
				Std_stf_number = number;
				again = false;
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Try again..." << endl;
			}
		}//end of while
	}//end of function

	void setFine(int fine) {
		bool again = true;
		while (true) {
			try {
				if (fine < 0)
					throw"The amount of the fine can not be negative!!";
				this->fine = fine;
				again = false;
			}
			catch (string message) {
				cout << message << endl;
				cout << "Enter the amount of the fine : ";
				cin >> fine;
			}
		}
	}

	//getter function
	string getID()override { return ID; }
	string getType()const { return Type; }
	string getnumber()const { return Std_stf_number; }
	int getFine()const { return fine; }

	void PaymentFine() {
		int amount;
		cout << "The amount of fines : " << fine << endl;
		if (fine == 0)
			cout << "No need to pay a fine" << endl;
		else if (fine != 0) {
			cout << "Enter the amount of the fine:";
			cin >> amount;
			fine = fine - amount;
			cout << "Thank you for your payment" << endl;
			cout << "The remaining amount of the fine : " << fine << endl;
		}
	}

	void report()override {
		cout << setw(9) << ID << setw(12) << fistName << setw(12) << lastName << setw(14) << nationalCode;
		cout << "   ";
		Date_of_birth.printBirth();
		cout << setfill(' ') << setw(10) << Type << setfill(' ') << setw(12) << Std_stf_number << setfill(' ') << setw(8) << fine << endl;

	}

};

class Librarian : public Person
{
	//Fields are moved from the object into the file
	friend ostream & operator << (ostream &output, const Librarian & object)
	{
		output << object.ID << "," << object.fistName << "," << object.lastName << "," << object.nationalCode
			<< "," << object.Date_of_birth << "," << object.Evidence << "," << object.Position << "\n";
		return output;
	}
	//Fields are moved from the file into the object
	friend istream & operator >> (istream &input, Librarian & object)
	{
		string word;
		string line;
		getline(input, line);
		stringstream split(line);
		getline(split, word, ',');
		object.ID = word;
		getline(split, word, ',');
		object.fistName = word;
		getline(split, word, ',');
		object.lastName = word;
		getline(split, word, ',');
		object.nationalCode = word;
		/*************************/
		getline(split, word, ',');



		int y, m, d;
		string wor;
		stringstream spleat(word);
		getline(spleat, wor, '/');
		y = stoi(wor);
		getline(spleat, wor, '/');
		m = stoi(wor);
		getline(spleat, wor, '/');
		d = stoi(wor);


		object.Date_of_birth = Birth(y, m, d);



		getline(split, word, ',');
		object.Evidence = word;

		getline(split, word, ',');
		object.Position = word;

		return input;
	}

private:
	string ID;
	string Evidence;
	string  Position; //(Simple - Supervisor)
public:
	Librarian(string ID = "11111111", string firstName = "notdefined",
		string lastName = "notdefined",
		string nationalCode = "382184790",
		Birth bd = Birth(1399, 1, 1), string Evidence = "notdefined",
		string  Position = "Simple") :Person(firstName, lastName, nationalCode, bd)
	{
		setID(ID);
		setEvidence(Evidence);
		setPosition(Position);
	}
	~Librarian() {}
	//setter function
	void setID(string ID)
	{
		bool again = true;
		while (again) {
			try {
				int size = ID.length();
				for (int i = 0; i < size; i++)
				{
					if (!isdigit(ID[i]))
						throw BadArgument("The ID consists only of numbers!!!");
				}
				if (size != 8)
					throw BadArgument("The limit of ID is 8 digits");



				/*if (string(typeid(this).name()) == "class Member")
				{
				if (find(MemberFile, ID, 1))
				throw BadArgument("This ID is available, the ID must be unique");
				}*/

				if (find(LibrarianFile, ID, 1))
					throw BadArgument("This ID is available, the ID must be unique");

				this->ID = ID;
				again = false;

			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter your ID (The limit of ID is 8 digits) : ";
				cin >> ID;
			}
		}//end of while
	}//end of function
	void setEvidence(string Evidence) {
		bool again = true;
		while (again) {
			try {
				if (!IsValidArgument(Evidence))
					throw BadArgument("The degree entered does not consist entirely of letters!!!");
				this->Evidence = Evidence;
				again = false;
			}//end of try
			catch (string message) {
				cout << message;
				cout << "Enter your degree level : ";
				cin >> Evidence;
			}//end of catch
		}//end of while
	}//end of function

	void setPosition(string Position) {
		bool again = true;
		while (again) {
			try {
				if (Position == "Simple" || Position == "simple" || Position == "Supervisor" || Position == "supervisor")
				{
					this->Position = Position;
					again = false;
				}
				else
					throw BadArgument("The entered side is not valid! The position can only be simple or supervisory");
			}//end of try
			catch (BadArgument ba) {
				cout << ba.getMessage() << endl;
				cout << "Enter your position : ";
				cin >> Position;
			}//end of catch
		}
	}

	//getter function
	string getID()override { return ID; }
	string getEvidence()const { return Evidence; }
	string getPosition()const { return Position; }

	void report()override {
		cout << setw(9) << ID << setw(12) << fistName << setw(12) << lastName << setw(14) << nationalCode;
		cout << "   ";
		Date_of_birth.printBirth();
		cout << setfill(' ') << setw(14) << Evidence << setfill(' ') << setw(10) << Position << endl;
	}
};


template <typename T>
vector<T> File_to_vector(string filename, T object) {
	vector<T> vec;
	ifstream input(filename, ios::in);
	if (!input)
		return vec;//Because no file has been created yet
	while (input >> object)
		vec.push_back(object);
	input.close();
	return vec;
}

class trusteeship {
	//Fields are moved from the object into the file
	friend ostream & operator << (ostream &output, const trusteeship & object)
	{
		output << object.MemberID << "," << object.TextID << "," << object.LoanDate << "," << object.DeliveryDate << "\n";
		return output;
	}
	//Fields are moved from the file into the object
	friend istream & operator >> (istream &input, trusteeship & object)
	{
		string word;
		string line;
		getline(input, line);
		stringstream split(line);
		getline(split, word, ',');
		object.MemberID = word;
		getline(split, word, ',');
		object.TextID = word;


		/*************************/
		getline(split, word, ',');



		int y, m, d;
		string wor;
		stringstream spleat(word);
		getline(spleat, wor, '/');
		y = stoi(wor);
		getline(spleat, wor, '/');
		m = stoi(wor);
		getline(spleat, wor, '/');
		d = stoi(wor);


		object.LoanDate = Birth(y, m, d);
		/*************************/
		getline(split, word, ',');



		int yea, mon, da;
		string w;
		stringstream splet(w);
		getline(splet, w, '/');
		yea = stoi(w);
		getline(splet, w, '/');
		mon = stoi(w);
		getline(splet, w, '/');
		da = stoi(w);


		object.DeliveryDate = Birth(yea, mon, da);



		return input;
	}

private:
	string MemberID;
	string TextID;
	Birth LoanDate;
	Birth DeliveryDate;
	vector<Book>listBook;
	vector<Magazine>listMagazine;
	vector<Thesis>listThesis;
	/*******************************/
	//To convert the lunar month to its corresponding number
	int MonthConversion(string month) {
		if (month == "Jan")
			return 1;
		else if (month == "Feb")
			return 2;
		else if (month == "Mar")
			return 3;
		else if (month == "Apr")
			return 4;
		else if (month == "May")
			return 5;
		else if (month == "Jun")
			return 6;
		else if (month == "Jul")
			return 7;
		else if (month == "Aug")
			return 8;
		else if (month == "Sep")
			return 9;
		else if (month == "Oct")
			return 10;
		else if (month == "Nov")
			return 11;
		else if (month == "Dec")
			return 12;
	}
	//Returns system history
	void SystemHistory(int &y, int &m, int & d) {
		string s[5];
		time_t result = time(NULL);
		char str[26];
		ctime_s(str, sizeof str, &result);
		//printf("%s", str);
		stringstream word(str);
		int i = 0;
		while (getline(word, s[i], ' ')) {
			i++;
		}
		m = MonthConversion(s[1]);
		d = stoi(s[2]);
		y = stoi(s[4]);
	}
	int day_month(int month) {
		//Returns the days of the lunar year
		if (month == 1)
			return 31;
		if (month == 2)
			return 	29;
		if (month == 3)
			return 31;
		if (month == 4)
			return 30;
		if (month == 5)
			return 31;
		if (month == 6)
			return 31;
		if (month == 7)
			return 31;
		if (month == 8)
			return 31;
		if (month == 9)
			return 30;
		if (month == 10)
			return 31;
		if (month == 11)
			return 30;
		if (month == 12)
			return 31;
	}
	/******************************/
public:
	trusteeship(string MemberID = "1212121212",
		Birth DeliveryDate = Birth(1399, 5, 22)) {
		setMemberID(MemberID);
		setTextID(1);
		setLoanDate();
		setDeliveryDate();
	}
	~trusteeship() {}
	int CalculateFine() {
		int da, mo, ye;
		int d, m, y;
		SystemHistory(y, m, d);
		DeliveryDate.day > d ? (da = DeliveryDate.day - d) : (da = d - DeliveryDate.day);
		DeliveryDate.month > m ? (mo = DeliveryDate.month - m) : (mo = m - DeliveryDate.month);
		DeliveryDate.year > y ? (ye = DeliveryDate.year - y) : (ye = y - DeliveryDate.year);
		ye = ye * 365;
		mo = mo * 30;
		int sum = ye + mo + da;
		return sum;

	}


	trusteeship& operator+=(int number)
	{
		int da, mo, ye;
		ye = DeliveryDate.year;
		mo = DeliveryDate.month;
		da = DeliveryDate.day + number;
		if (da > day_month(da)) {
			da = da - day_month(da);
			mo++;
			if (mo > 12) {
				mo = 1;
				ye++;
			}
		}
		DeliveryDate.day = da;
		DeliveryDate.month = mo;
		DeliveryDate.year = ye;
		return *this;
	}

	//setter function
	void setMemberID(string MemberID) {
		bool again = true;
		while (again) {

			try {
				vector<string>ID;
				ID.clear();
				ifstream file1("Trustbooks.csv", ios::in);
				if (!file1)/*The file is checked before each action,
						  because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				ifstream file2("TrustThesis.csv", ios::in);
				if (!file2)/*The file is checked before each action,
						  because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				ifstream file3("TrustMagazine.csv", ios::in);
				if (!file3)/*The file is checked before each action,
						  because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				trusteeship obj;
				while (file1 >> obj)
					ID.push_back(obj.MemberID);
				while (file2 >> obj)
					ID.push_back(obj.MemberID);
				while (file3 >> obj)
					ID.push_back(obj.MemberID);
				file1.close();
				file2.close();
				file3.close();
				if (!find(MemberFile, MemberID, 1))
					throw BadArgument("There is also no member ID!!");

				int count = 0;

				for (int h = 0; h < ID.size(); h++) {
					if (ID[h] == MemberID)
						count++;
				}
				if (count >= 3)
					throw Error("Each member can only borrow up to 3 articles(books,Magazine,Thesis)");
				this->MemberID = MemberID;
				again = false;
			}
			catch (BadArgument str) {
				string i;
				cout << str.getMessage() << endl;
				cout << "Enter Member Id : ";
				cin >> i;
			}
		}
	}
	void setTextID(int m) {
		string id;
		bool again = true;
		while (again) {
			again = false;
			try {
				switch (m)
				{
				case 1: {
					cout << "Enter Book id : ";
					cin >> id;
					if (!find(BookFile, id, 2))
						throw BadArgument("The login ID is not available!!");
					Book b;
					listBook.clear();
					/******************/
					Book object;
					vector<Book> vec;
					ifstream input(BookFile, ios::in);
					while (input >> object)
						vec.push_back(object);
					input.close();
					listBook = vec;
					/******************/
					int v = listBook.size();
					for (int i = 0; i < v; i++) {
						if (listBook[i].getID() == id) {
							if (listBook[i].getInventory() == 0)
								throw Error("Book inventory is 0!");
							else {
								int t = listBook[i].getInventory();
								listBook[i].setInventory(t--);
							}
						}
					}
					ofstream output("TEXT.csv", ios::app);

					if (!output)/*The file is checked before each action,
								because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}

					for (int i = 0; i < v; i++)
						output << listBook[i];
					output.close();
					input.close();
					remove("Book.csv");
					rename("TEXT.csv", "Book.csv");
					TextID = id;
					break;
				}
				case 2: {
					cout << "Enter Magazine id : ";
					cin >> id;
					Magazine M;
					listMagazine.clear();
					listMagazine = File_to_vector(MagazineFile, M);
					int f = listMagazine.size();
					bool temp = false;
					for (int i = 0; i < f; i++) {
						if (listMagazine[i].getID() == id)
						{
							temp = true;
							if (listMagazine[i].getInventory() == 0)
								throw Error("Magazine inventory is 0!");
							else {

								int r = listMagazine[i].getInventory();
								listMagazine[i].setInventory(r--);
								ifstream input(MagazineFile, ios::in);
								ofstream output("TEXT.csv", ios::app);
								if (!input)/*The file is checked before each action,
											because all actions depend on the file*/
								{
									cout << "Oops!!The file will not open!\nTry another time.\n";
									abort();
								}
								if (!output)/*The file is checked before each action,
											because all actions depend on the file*/
								{
									cout << "Oops!!The file will not open!\nTry another time.\n";
									abort();
								}
								for (int i = 0; i < f; i++)
									output << listMagazine[i];
								output.close();
								input.close();
								remove("Magazine.csv");
								rename("TEXT.csv", "Magazine.csv");
							}

						}
					}
					if (temp == false)
						throw BadArgument("The login ID is not available!!");
					TextID = id;
					break;
				}
				case 3: {
					cout << "Enter Thesis id : ";
					cin >> id;
					Thesis TH;
					listThesis.clear();
					listThesis = File_to_vector(ThesisFile, TH);
					int e = listThesis.size();
					bool temp = false;
					for (int i = 0; i < e; i++) {
						if (listThesis[i].getID() == id)
						{
							temp = true;
							if (listThesis[i].getInventory() == 0)
								throw Error("Thesis inventory is 0!");
							else {
								listThesis[i].setInventory(false);
								ifstream input(ThesisFile, ios::in);
								ofstream output("TEXT.csv", ios::app);
								if (!input)/*The file is checked before each action,
										   because all actions depend on the file*/
								{
									cout << "Oops!!The file will not open!\nTry another time.\n";
									abort();
								}
								if (!output)/*The file is checked before each action,
											because all actions depend on the file*/
								{
									cout << "Oops!!The file will not open!\nTry another time.\n";
									abort();
								}
								for (int i = 0; i < e; i++)
									output << listThesis[i];
								output.close();
								input.close();
								remove("Thesis.csv");
								rename("TEXT.csv", "Thesis.csv");
							}

						}
					}
					if (temp == false)
						throw"The login ID is not available!!";
					TextID = id;
					break; }
				default:

					break;
				}
			}
			catch (BadArgument error) {
				cout << error.getMessage() << endl;
				cout << "Try again..." << endl;
				again = true;
			}
		}
	}
	void setLoanDate() {
		int d, m, y;
		SystemHistory(y, m, d);
		LoanDate.day = d;
		LoanDate.month = m;
		LoanDate.year = y;
	}
	void setDeliveryDate() {
		int dy, mth, yr;
		dy = LoanDate.day;
		mth = LoanDate.month;
		yr = LoanDate.year;
		dy = dy + 7;
		if (dy > day_month(dy)) {
			dy = dy - day_month(dy);
			mth++;
			if (mth > 12) {
				mth = 1;
				yr++;
			}
		}
		DeliveryDate.day = dy;
		DeliveryDate.month = mth;
		DeliveryDate.year = yr;
	}
	//getter function
	string getMemberID()const { return MemberID; }
	string getTextID()const { return TextID; }
	Birth getLoanDate()const { return LoanDate; }
	Birth getDeliveryDate()const { return DeliveryDate; }


	void report() {

		cout << left << setw(9) << MemberID << left << setw(9) << TextID;
		LoanDate.printBirth();
		DeliveryDate.printBirth();
		cout << endl;
	}




};




/**********************Book********************/
vector<Book>listBook;
void AddBook() {
	Book Assumption;
	listBook.clear();
	listBook = File_to_vector(BookFile, Assumption);
	ofstream outputBook(BookFile, ios::app);
	if (!outputBook)/*The file is checked before each action,
					because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	Book newBook;
	string ID, Title, numberPages, Category;
	int numberBooks, Inventory;
	cout << "Enter the book title : ";
	cin >> Title;
	newBook.setTitle(Title);
	cout << "Enter the book ID (The limit of ID is 8 digits) : ";
	cin >> ID;
	newBook.setID(ID);
	cout << "Enter the number of pages in the book : ";
	cin >> numberPages;
	newBook.setNumberPages(numberPages);
	newBook.setCategory();
	cout << "Enter the number of books : ";
	cin >> numberBooks;
	newBook.setNumberBooks(numberBooks);
	cout << "Enter the book balance to borrow : ";
	cin >> Inventory;
	newBook.setInventory(Inventory);
	outputBook << newBook;
	listBook.push_back(newBook);
	outputBook.close();

}
void EditBook(string id) {
	Book Assumption;
	listBook.clear();
	bool check;
	listBook = File_to_vector(BookFile, Assumption);
	int r = listBook.size();
	for (int i = 0; i < r; i++) {
		if (listBook[i].getID() == id) {
			/************/
			check = true;
			string ID, Title, numberPages, Category;
			int numberBooks, Inventory;
			int ans;
			/*************/
			bool again = true;
			while (again) {
				cout << "\n\tSelect Menu\n\n" << endl;
				cout << "\t1.book title\n" << endl;
				cout << "\t2.book ID\n" << endl;
				cout << "\t3.number of pages\n" << endl;
				cout << "\t4.book category\n" << endl;
				cout << "\t5.number of books\n" << endl;
				cout << "\t6.Inventory\n" << endl;
				cin >> ans;
				again = false;
				switch (ans) {
				case 1:
					cout << "Enter the book title : ";
					cin >> Title;
					listBook[i].setTitle(Title);
					break;
				case 2:
					cout << "Enter the book ID (The limit of ID is 8 digits) : ";
					cin >> ID;
					listBook[i].setID(ID);
					break;
				case 3:
					cout << "Enter the number of pages in the book : ";
					cin >> numberPages;
					listBook[i].setNumberPages(numberPages);
					break;
				case 4:
					listBook[i].setCategory();
					break;
				case 5:
					cout << "Enter the number of books : ";
					cin >> numberBooks;
					listBook[i].setNumberBooks(numberBooks);
					break;
				case 6:
					cout << "Enter the book balance to borrow : ";
					cin >> Inventory;
					listBook[i].setInventory(Inventory);
					break;
				default:
					cout << "The number entered is not in the range of 1 to 6!/n" << endl;
					again = true;
				}
			}//end of while
		}
	}
	if (check) {
		ifstream input(BookFile, ios::in);
		if (!input)/*The file is checked before each action,
					because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		ofstream output("TEXT.csv", ios::app);
		if (!output)/*The file is checked before each action,
				  because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		int size = listBook.size();
		for (int i = 0; i < size; i++)
			output << listBook[i];
		output.close();
		input.close();
		remove("Book.csv");
		rename("TEXT.csv", "Book.csv");
	}
	if (!check)
		throw Invalid("The login ID is not available!!");

}
void DeleteBook(string id) {
	Book Assumption;
	bool check;
	listBook.clear();
	listBook = File_to_vector(BookFile, Assumption);
	int r = listBook.size();
	for (int i = 0; i < r; i++) {
		if (listBook[i].getID() == id) {
			check = true;
			int temp = listBook[i].getNumberBooks();
			temp--;
			if (temp <= 0) {
				int u = listBook.size();
				for (int j = i; j < u - 1; j++) {
					listBook[i] = listBook[i + 1];
				}
				listBook.pop_back();
				break;
			}
			else if (temp > 0) {
				listBook[i].setNumberBooks(temp);
				int inv = listBook[i].getInventory();
				if (inv > listBook[i].getNumberBooks())
					listBook[i].setInventory(listBook[i].getNumberBooks());
				break;
			}
		}
	}
	if (check) {
		ifstream input(BookFile, ios::in);
		if (!input)/*The file is checked before each action,
				   because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		ofstream output("TEXT.csv", ios::app);
		if (!output)/*The file is checked before each action,
					because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		int size = listBook.size();
		for (int i = 0; i < size; i++)
			output << listBook[i];
		output.close();
		input.close();
		remove("Book.csv");
		rename("TEXT.csv", "Book.csv");
	}
	if (!check)
		throw Invalid("The login ID is not available!!");

}
//Using dynamic binding via base class refrence
void Search_D(vector<Document*> baseref, string id) {
	bool check;
	int y = baseref.size();
	for (int i = 0; i < y; i++) {
		if (baseref[i]->getID() == id) {
			check = true;
			baseref[i]->report();
		}
	}
	if (!check)
		throw Invalid("The login ID is not available!!");
}
//Using dynamic binding via base class refrence
void Report_D(vector<Document*> baseref) {
	int e = baseref.size();
	for (int i = 0; i < e; i++)
	{
		baseref[i]->report();
	}
}

void RSpecialCategory() {
	Book obj;
	int ans;
	ifstream input(BookFile, ios::in);
	if (!input)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	cout << "Select the category to report";
	cout << "1.Engineering" << "\t2.the base" << "\t3.the literature" << "\t4.Theology" << "\t5.Other" << endl;
	cin >> ans;
	switch (ans)
	{
	case 1:
		while (input >> obj) {
			if (obj.getCategory() == "Engineering")
				obj.report();
		}
		break;
	case 2:
		while (input >> obj) {
			if (obj.getCategory() == "the base")
				obj.report();
		}
		break;
	case 3:
		while (input >> obj) {
			if (obj.getCategory() == "the literature")
				obj.report();
		}
		break;
	case 4:

		while (input >> obj) {
			if (obj.getCategory() == "Theology")
				obj.report();
		}
		break;
	case 5:


		while (input >> obj) {
			if (obj.getCategory() == "Other")
				obj.report();
		}
		break;
	default:
		cout << "Incorrect number!!" << endl;
		return;
		break;
	}


}

/********************Magazine******************/
vector<Magazine>listMagazine;
void AddMagazine() {
	Magazine Assumption;
	listMagazine.clear();
	listMagazine = File_to_vector(MagazineFile, Assumption);
	ofstream outputMagazine(MagazineFile, ios::app);
	if (!outputMagazine)/*The file is checked before each action,
					because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	Magazine newMagazine;
	string ID, Title, numberPages;
	int numberMagazines, Inventory;
	cout << "Enter the Magazines title : ";
	cin >> Title;
	newMagazine.setTitle(Title);
	cout << "Enter the Magazines ID (The limit of ID is 8 digits) : ";
	cin >> ID;
	newMagazine.setID(ID);
	cout << "Enter the number of pages of the magazine : ";
	cin >> numberPages;
	newMagazine.setNumberPages(numberPages);
	cout << "Enter the number of Magazines : ";
	cin >> numberMagazines;
	newMagazine.setNumberMagazine(numberMagazines);
	cout << "Magazine inventory (for lending) : ";
	cin >> Inventory;
	newMagazine.setInventory(Inventory);
	outputMagazine << newMagazine;//write in file
	listMagazine.push_back(newMagazine);
	outputMagazine.close();
}
void EditMagazine(string id) {
	bool check;
	ifstream input(MagazineFile, ios::in);
	if (!input)/*The file is checked before each action,
			   because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	ofstream output("TEXT.csv", ios::app);
	if (!output)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	Magazine Mgz;
	while (input >> Mgz) {
		if (Mgz.getID() == id) {
			check = true;
			/************/
			string ID, Title, numberPages;
			int numberMagazines, Inventory;
			int ans;
			/*************/
			bool again = true;
			while (again) {
				cout << "\n\tSelect Menu\n\n" << endl;
				cout << "\t1.Magazine title\n" << endl;
				cout << "\t2.Magazine ID\n" << endl;
				cout << "\t4.number of pages\n" << endl;
				cout << "\t5.number of Magazines\n" << endl;
				cout << "\t6.Inventory\n" << endl;
				cin >> ans;
				again = false;
				switch (ans) {
				case 1:
					cout << "Enter the Magazines title : ";
					cin >> Title;
					Mgz.setTitle(Title);
					break;
				case 2:
					cout << "Enter the Magazines ID (The limit of ID is 8 digits) : ";
					cin >> ID;
					Mgz.setID(ID);
					break;
				case 3:
					cout << "Enter the number of pages of the magazine : ";
					cin >> numberPages;
					Mgz.setNumberPages(numberPages);
					break;
				case 4:
					cout << "Enter the number of Magazines : ";
					cin >> numberMagazines;
					Mgz.setNumberMagazine(numberMagazines);
					break;
				case 5:
					cout << "Magazine inventory (for lending) : ";
					cin >> Inventory;
					Mgz.setInventory(Inventory);
					break;
				default:
					cout << "The number entered is not in the range of 1 to 5!/n" << endl;
					again = true;
				}
			}//end of while
			output << Mgz;
		}//end of if
		else {
			output << Mgz;
		}
	}
	input.close();
	output.close();
	remove("Magazine.csv");
	rename("TEXT.csv", "Magazine.csv");
	if (!check)
		throw Invalid("The login ID is not available!!");
}
void DeleteMagazine(string id) {
	Magazine Assumption;
	bool check;
	listMagazine.clear();
	listMagazine = File_to_vector(MagazineFile, Assumption);
	int w = listMagazine.size();
	for (int i = 0; i < w; i++) {
		if (listMagazine[i].getID() == id) {
			check = true;
			int temp = listMagazine[i].getNumberMagazines();
			temp--;
			if (temp <= 0) {
				int b = listMagazine.size();
				for (int j = i; j < b - 1; j++) {
					listMagazine[i] = listMagazine[i + 1];
				}
				listMagazine.pop_back();
				break;
			}
			else if (temp > 0) {
				listMagazine[i].setNumberMagazine(temp);
				int inv = listMagazine[i].getInventory();
				if (inv > listMagazine[i].getNumberMagazines())
					listMagazine[i].setInventory(listMagazine[i].getNumberMagazines());
				break;

			}
		}
	}

	if (check) {
		ifstream input(MagazineFile, ios::in);
		if (!input)/*The file is checked before each action,
				   because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		ofstream output("TEXT.csv", ios::app);
		if (!output)/*The file is checked before each action,
					because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}

		int size = listMagazine.size();
		for (int i = 0; i < size; i++)
			output << listMagazine[i];
		output.close();
		input.close();
		remove("Magazine.csv");
		rename("TEXT.csv", "Magazine.csv");
	}
	if (!check)
		throw Invalid("The login ID is not available!!");
}
/*********************Thesis*******************/
vector<Thesis>listThesis;
void AddThesis() {
	Thesis Assumption;
	listThesis.clear();
	listThesis = File_to_vector(ThesisFile, Assumption);
	ofstream outputThesis(ThesisFile, ios::app);
	if (!outputThesis)/*The file is checked before each action,
					because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	Thesis newThesis;
	string ID, Title, numberPages, Grade, College, group;
	bool Inventory;
	cout << "Enter the Thesis title : ";
	cin >> Title;
	newThesis.setTitle(Title);
	cout << "Enter the Thesis ID (The limit of ID is 8 digits) : ";
	cin >> ID;
	newThesis.setID(ID);
	cout << "Enter the number of pages of the Thesis : ";
	cin >> numberPages;
	newThesis.setNumberPages(numberPages);
	cout << "Enter the author's degree : ";
	cin >> Grade;
	newThesis.setGrade(Grade);
	cout << "Enter author school : ";
	cin >> College;
	newThesis.setCollege(College);
	cout << "Enter author group : ";
	cin >> group;
	newThesis.setGroup(group);
	newThesis.setInventory(true);
	outputThesis << newThesis;
	listThesis.push_back(newThesis);
	outputThesis.close();
}
void EditThesis(string id) {
	bool check;
	ifstream input(ThesisFile, ios::in);
	if (!input)/*The file is checked before each action,
			   because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	ofstream output("TEXT.csv", ios::app);
	if (!output)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	Thesis Ths;
	while (input >> Ths) {
		if (Ths.getID() == id) {
			check = true;
			/************/
			string ID, Title, numberPages, Grade, College, group;
			bool Inventory;
			int ans;
			/*************/
			bool again = true;
			while (again) {
				cout << "\n\tSelect Menu\n\n" << endl;
				cout << "\t1.Thesis title\n" << endl;
				cout << "\t2.Thesis ID\n" << endl;
				cout << "\t3.number of pages\n" << endl;
				cout << "\t4.Author's degree\n" << endl;
				cout << "\t5.Author school\n" << endl;
				cout << "\t6.Author group\n" << endl;
				cout << "\t6.Inventory\n" << endl;
				cin >> ans;
				again = false;
				switch (ans) {
				case 1:
					cout << "Enter the Thesis title : ";
					cin >> Title;
					Ths.setTitle(Title);
					break;
				case 2:
					cout << "Enter the Thesis ID (The limit of ID is 8 digits) : ";
					cin >> ID;
					Ths.setID(ID);
					break;
				case 3:
					cout << "Enter the number of pages of the Thesis  : ";
					cin >> numberPages;
					Ths.setNumberPages(numberPages);
					break;
				case 4:
					cout << "Enter the author's degree : ";
					cin >> Grade;
					Ths.setGrade(Grade);
					break;
				case 5:
					cout << "Enter author school : ";
					cin >> College;
					Ths.setCollege(College);
					break;
				case 6:
					cout << "Enter author group : ";
					cin >> group;
					Ths.setGroup(group);
					break;
				case 7: {
					bool repeat = false;
					do {
						cout << "0.Not available" << "\t1.Is available" << endl;
						cin >> Inventory;
						if (Inventory != 1 && Inventory != 0)
						{
							cout << "The number entered is invalid!" << endl;
							repeat = true;
						}
					} while (repeat);
					Ths.setInventory(Inventory);
					break; }
				default:
					cout << "The number entered is not in the range of 1 to 7!/n" << endl;
					again = true;
					break;
				}
			}//end of while
			output << Ths;
		}//end of if
		else {
			output << Ths;
		}
	}
	input.close();
	output.close();
	remove("Thesis.csv");
	rename("TEXT.csv", "Thesis.csv");
	if (!check)
		throw Invalid("The login ID is not available!!");
}
void DeleteThesis(string id) {
	Thesis Assumption;
	bool check;
	listThesis.clear();
	listThesis = File_to_vector(ThesisFile, Assumption);
	for (int i = 0; i < listThesis.size(); i++) {
		if (listThesis[i].getID() == id) {
			check = true;
			for (int j = i; j < listThesis.size() - 1; j++) {
				listThesis[i] = listThesis[i + 1];
			}
			listThesis.pop_back();
			break;
		}
	}
	if (check) {
		ifstream input(ThesisFile, ios::in);
		if (!input)/*The file is checked before each action,
				   because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		ofstream output("TEXT.csv", ios::app);
		if (!output)/*The file is checked before each action,
					because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		int size = listThesis.size();
		for (int i = 0; i < size; i++)
			output << listThesis[i];
		output.close();
		input.close();
		remove("Thesis.csv");
		rename("TEXT.csv", "Thesis.csv");
	}
	if (!check)
		throw Invalid("The login ID is not available!!");

}

/*********************Member*******************/
vector<Member> listMember;
void AddMember() {
	Member Assumption;
	listMember.clear();
	listMember = File_to_vector(MemberFile, Assumption);
	ofstream outputMember(MemberFile, ios::app);
	if (!outputMember)/*The file is checked before each action,
					because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	int year, month, day, fine;
	string ID, fistName, lastName, nationalCode;
	Member newMember;
	cout << " Enter your ID (The limit of ID is 8 digits) : ";
	cin >> ID;
	newMember.setID(ID);
	cout << " Enter your firstname : ";
	cin >> fistName;
	newMember.setFistName(fistName);
	cout << "Enter your lastname : ";
	cin >> lastName;
	newMember.setLastName(lastName);
	cout << "Enter your national code : ";
	cin >> nationalCode;
	newMember.setNationalCode(nationalCode);
	cout << "Enter your date of birth(respectively  year, month, day) :";
	cin >> year >> month >> day;
	newMember.setBirth(Birth(year, month, day));
	newMember.setType();
	newMember.setNumber();
	//fine
	listMember.push_back(newMember);
	outputMember << newMember;//write in file
	outputMember.close();
}
void EditMember(string id) {
	bool check;
	Member Assumption;
	listMember.clear();
	listMember = File_to_vector(MemberFile, Assumption);
	for (int i = 0; i < listMember.size(); i++) {
		if (listMember[i].getID() == id) {
			/************/
			check = true;
			int year, month, day, fine;
			string ID, fistName, lastName, nationalCode;
			int ans;
			/*************/
			bool again = true;
			while (again) {
				cout << "\n\tSelect Menu\n\n" << endl;
				cout << "\t1.Member ID\n" << endl;
				cout << "\t2.Member Firstname\n" << endl;
				cout << "\t3.Member Lastname\n" << endl;
				cout << "\t4.Member NationalCode\n" << endl;
				cout << "\t5.Member date of birth\n" << endl;
				cout << "\t6.Member Type\n" << endl;
				cout << "\t7.Student / staff number\n" << endl;
				cout << "\t8.The amount of fines\n" << endl;
				cin >> ans;
				again = false;
				switch (ans) {
				case 1:
					cout << "Enter your ID (The limit of ID is 8 digits) : ";
					cin >> ID;
					listMember[i].setID(ID);
					break;
				case 2:
					cout << " Enter your firstname : ";
					cin >> fistName;
					listMember[i].setFistName(fistName);
					break;
				case 3:
					cout << "Enter your lastname : ";
					cin >> lastName;
					listMember[i].setLastName(lastName);
					break;
				case 4:
					cout << "Enter your national code : ";
					cin >> nationalCode;
					listMember[i].setNationalCode(nationalCode);
					break;
				case 5:
					cout << "Enter your date of birth(respectively  year, month, day) :";
					cin >> year >> month >> day;
					listMember[i].setBirth(Birth(year, month, day));
					break;
				case 6:
					listMember[i].setType();
					break;
				case 7:
					listMember[i].setNumber();
					break;
				case 8:
					cout << "Enter the amount of the fine : ";
					cin >> fine;
					listMember[i].setFine(fine);
					break;
				default:
					cout << "The number entered is not in the range of 1 to 8!/n" << endl;
					again = true;
				}
			}//end of while
		}

	}

	if (check) {
		ifstream input(MemberFile, ios::app);
		ofstream output("TEXT.csv", ios::app);
		if (!output)/*The file is checked before each action,
						  because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		int size = listMember.size();
		for (int i = 0; i < size; i++)
			output << listMember[i];
		output.close();
		input.close();
		remove("Member.csv");
		rename("TEXT.csv", "Member.csv");
	}

	if (!check)
		throw Invalid("The login ID is not available!!");
}
void DeleteMember(string id) {
	bool check;
	Member Assumption;
	listMember.clear();
	listMember = File_to_vector(MemberFile, Assumption);
	for (int i = 0; i < listMember.size(); i++) {
		if (listMember[i].getID() == id) {
			/**************************************/
			for (int j = i; j < listMember.size() - 1; j++) {
				listMember[i] = listMember[i + 1];
			}
			listMember.pop_back();
			check = true;
			break;
		}
	}

	if (check) {
		ifstream input(MemberFile, ios::app);
		if (!input)/*The file is checked before each action,
					because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		ofstream output("TEXT.csv", ios::app);
		if (!output)/*The file is checked before each action,
					because all actions depend on the file*/
		{
			cout << "Oops!!The file will not open!\nTry another time.\n";
			abort();
		}
		int size = listBook.size();
		for (int i = 0; i < size; i++)
			output << listBook[i];
		remove("Member.csv");
		rename("TEXT.csv", "Member.csv");
	}

	if (!check)
		throw Invalid("The login ID is not available!!");
}

/***********************************************/

//Using dynamic binding via base class refrence
void Search(vector<Person*> baseref, string id) {
	for (int i = 0; i < baseref.size(); i++)
		if (baseref[i]->getID() == id) {
			baseref[i]->report();
		}
		else
		{
			throw Invalid("The login ID is not available!!");
		}

}
//Using dynamic binding via base class refrence
void Report(vector<Person*> baseref) {
	for (int i = 0; i < baseref.size(); i++)
	{
		baseref[i]->report();
	}
}

/********************Librarian*****************/
vector<Librarian> listLibrarian;
void AddLibrarian() {
	Librarian Assumption;
	listLibrarian.clear();
	listLibrarian = File_to_vector(LibrarianFile, Assumption);
	ofstream outputLibrarian(LibrarianFile, ios::app);
	if (!outputLibrarian)/*The file is checked before each action,
					  because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	int year, month, day;
	string ID, fistName, lastName, nationalCode, Evidence, Position;
	Librarian newLibrarian;
	cout << " Enter your ID (The limit of ID is 8 digits) : ";
	cin >> ID;
	newLibrarian.setID(ID);
	cout << " Enter your firstname : ";
	cin >> fistName;
	newLibrarian.setFistName(fistName);
	cout << "Enter your lastname : ";
	cin >> lastName;
	newLibrarian.setLastName(lastName);
	cout << "Enter your national code : ";
	cin >> nationalCode;
	newLibrarian.setNationalCode(nationalCode);
	cout << "Enter your date of birth(respectively  year, month, day) :";
	cin >> year >> month >> day;
	newLibrarian.setBirth(Birth(year, month, day));
	cout << "Enter your degree level : ";
	cin >> Evidence;
	newLibrarian.setEvidence(Evidence);
	cout << "Enter your position (simple / supervisory): ";
	cin >> Position;
	newLibrarian.setPosition(Position);

	listLibrarian.push_back(newLibrarian);
	outputLibrarian << newLibrarian;//write in file
	outputLibrarian.close();
}
void EditLibrarian(string id) {
	bool check;
	ifstream input(LibrarianFile, ios::in);
	if (!input)/*The file is checked before each action,
					  because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	ofstream output("TEXT.csv", ios::app);
	if (!output)/*The file is checked before each action,
			   because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	Librarian Lbr;
	while (input >> Lbr) {
		if (Lbr.getID() == id) {
			check = true;
			/************/
			int year, month, day;
			string ID, fistName, lastName, nationalCode, Evidence, Position;
			int ans;
			/*************/
			bool again = true;
			while (again) {
				cout << "\n\tLibrarian Menu\n\n" << endl;
				cout << "\t1.Librarianr ID\n" << endl;
				cout << "\t2.Librarian Firstname\n" << endl;
				cout << "\t3.Librarian Lastname\n" << endl;
				cout << "\t4.Librarian NationalCode\n" << endl;
				cout << "\t5.Librarian date of birth\n" << endl;
				cout << "\t6.Librarian degree\n" << endl;
				cout << "\t7.Librarian position\n" << endl;
				cin >> ans;
				again = false;
				switch (ans) {
				case 1:
					cout << "Enter your ID (The limit of ID is 8 digits) : ";
					cin >> ID;
					Lbr.setID(ID);
					break;
				case 2:
					cout << " Enter your firstname : ";
					cin >> fistName;
					Lbr.setFistName(fistName);
					break;
				case 3:
					cout << "Enter your lastname : ";
					cin >> lastName;
					Lbr.setLastName(lastName);
					break;
				case 4:
					cout << "Enter your national code : ";
					cin >> nationalCode;
					Lbr.setNationalCode(nationalCode);
					break;
				case 5:
					cout << "Enter your date of birth(respectively  year, month, day) :";
					cin >> year >> month >> day;
					Lbr.setBirth(Birth(year, month, day));
					break;
				case 6:
					cout << "Enter your degree level : ";
					cin >> Evidence;
					Lbr.setEvidence(Evidence);
					break;
				case 7:
					cout << "Enter your position (simple / supervisory): ";
					cin >> Position;
					Lbr.setPosition(Position);
					break;
				default:
					cout << "The number entered is not in the range of 1 to 7!/n" << endl;
					again = true;
				}//end of swith
			}//end of while
			output << Lbr;
		}//end of if
		else {
			output << Lbr;
		}
	}
	input.close();
	output.close();
	remove("Librarian.csv");
	rename("TEXT.csv", "Librarian.csv");
	if (!check)
		throw Invalid("The login ID is not available!!");
}
void DeleteLibrarian(string id) {
	bool check;
	ifstream input(LibrarianFile, ios::in);
	if (!input)/*The file is checked before each action,
					  because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	ofstream output("TEXT.csv", ios::app);
	if (!output)/*The file is checked before each action,
			   because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	Librarian Lbrn;
	while (input >> Lbrn) {
		if (!(Lbrn.getID() == id)) {
			output << Lbrn << endl;
			check = true;
		}
	}
	output.close();
	input.close();
	remove("Librarian.csv");
	rename("TEXT.csv", "Librarian.csv");
	if (!check)
		throw Invalid("The login ID is not available!!");

}
/*******************trusteeship****************/
template <typename T>
void IncreaseInventory(string id, T object, string filename)
{
	ofstream write("TEXT.csv", ios::app);
	if (!write)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	ifstream myfile(filename, ios::in);
	if (!myfile)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	while (myfile >> object) {
		if (object.getID() == id) {
			int temp = object.getInventory();
			object.setInventory(temp++);

			write << object;
		}
		else
			write << object;
	}
	write.close();
	myfile.close();

}



void Addloan() {
	int ans;
	string id;
	trusteeship loan;
	bool again = true;
	while (again) {
		again = false;
		try {
			cout << "Select the text you want to borrow" << endl;
			cout << "1 . Book" << endl;
			cout << "2 . Magazine" << endl;
			cout << "3 . Thesis" << endl;
			cin >> ans;
			switch (ans)
			{
			case 1: {
				ofstream loanBook("Trustbooks.csv", ios::app);
				if (!loanBook)/*The file is checked before each action,
							because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				cout << "Enter Member Id : ";
				cin >> id;
				loan.setMemberID(id);
				loan.setTextID(1);
				loan.setLoanDate();
				loan.setDeliveryDate();
				cout << "/n Loan Date: Today\n Delivery date : next week\n";
				loanBook << loan;
				loanBook.close();
				break;
			}
			case 2:
			{
				ofstream loanMagazine("TrustMagazine.csv", ios::app);
				if (!loanMagazine)/*The file is checked before each action,
							  because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				cout << "Enter Member Id : ";
				cin >> id;
				loan.setMemberID(id);
				loan.setTextID(2);
				loan.setLoanDate();
				loan.setDeliveryDate();
				cout << "/n Loan Date: Today\n Delivery date : next week\n";
				loanMagazine << loan;
				loanMagazine.close();
				break;
			}
			case 3:
			{
				ofstream loanThesis("TrustThesis.csv", ios::app);
				if (!loanThesis)/*The file is checked before each action,
								  because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				cout << "Enter Member Id : ";
				cin >> id;
				loan.setMemberID(id);
				loan.setTextID(3);
				loan.setLoanDate();
				loan.setDeliveryDate();
				cout << "/n Loan Date: Today\n Delivery date : next week\n";
				loanThesis << loan;
				loanThesis.close();
				break;
			}

			default:
				cout << "Incorrect number!!" << endl;
				again = true;
				break;
			}

		}//end of try
		catch (Error obj) {
			cout << obj.getMessage() << endl;
			return;
		}

	}
}

void Editloan() {
	bool again = true;
	bool check = false;
	int ans;
	string memberId, textId;
	cout << "Enter the member ID to edit : ";
	cin >> memberId;
	while (again) {
		again = false;
		cout << "Select the text you borrowed" << endl;
		cout << "1 . Book" << endl;
		cout << "2 . Magazine" << endl;
		cout << "3 . Thesis" << endl;
		cin >> ans;
		switch (ans)
		{
		case 1: {
			cout << "Enter the Book ID to Edit : ";
			cin >> textId;
			ifstream input("Trustbooks.csv", ios::in);
			if (!input)/*The file is checked before each action,
					   because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			ofstream output("TEXT.csv", ios::app);
			if (!output)/*The file is checked before each action,
						because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			trusteeship objct;
			while (input >> objct) {
				if (objct.getMemberID() == memberId && objct.getTextID() == textId) {
					check = true;
					objct += 7;
					output << objct;
				}
				else
					output << objct;
			}
			output.close();
			input.close();
			remove("Trustbooks.csv");
			rename("TEXT.csv", "Trustbooks.csv");
			break;
		}// end of case 1
		case 2:
		{
			cout << "Enter the Magazine ID to Edit : ";
			cin >> textId;
			ifstream input("TrustMagazine.csv", ios::in);
			if (!input)/*The file is checked before each action,
					   because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			ofstream output("TEXT.csv", ios::app);
			if (!output)/*The file is checked before each action,
						because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			trusteeship objct;
			while (input >> objct) {
				if (objct.getMemberID() == memberId && objct.getTextID() == textId) {
					check = true;
					objct += 7;
					output << objct;
				}
				else
					output << objct;
			}
			output.close();
			input.close();
			remove("TrustMagazine.csv");
			rename("TEXT.csv", "TrustMagazine.csv");
			break;
		}// end of case 2
		case 3: {
			cout << "Enter the Thesis ID to Edit : ";
			cin >> textId;
			ifstream input("TrustThesis.csv", ios::in);
			if (!input)/*The file is checked before each action,
					   because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			ofstream output("TEXT.csv", ios::app);
			if (!output)/*The file is checked before each action,
						because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			trusteeship objct;
			while (input >> objct) {
				if (objct.getMemberID() == memberId && objct.getTextID() == textId) {
					check = true;
					objct += 7;
					output << objct;
				}
				else
					output << objct;
			}
			output.close();
			input.close();
			remove("TrustThesis.csv");
			rename("TEXT.csv", "TrustThesis.csv");
			break;
		}// end of case 3
		default:
			cout << "Incorrect number!!" << endl;
			again = true;
			break;

		}

	}//end of while

	if (!check)
		throw Invalid("The login ID is not available!!");
}

void Deleteloan() {
	bool check = false;
	bool again = true;
	int ans;
	string memberId, textId;
	cout << "Enter the member ID to Delete : ";
	cin >> memberId;
	while (again) {
		again = false;
		cout << "Select the text you borrowed" << endl;
		cout << "1 . Book" << endl;
		cout << "2 . Magazine" << endl;
		cout << "3 . Thesis" << endl;
		cin >> ans;
		switch (ans)
		{
		case 1: {
			cout << "Enter the Book ID to Delete : ";
			cin >> textId;
			trusteeship test;
			ifstream input("Trustbooks.csv", ios::in);
			if (!input)/*The file is checked before each action,
					   because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			ofstream output("Test.csv", ios::in);
			if (!output)/*The file is checked before each action,
					   because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			while (input >> test) {
				if (test.getMemberID() == memberId && test.getTextID() == textId) {
					check = true;
					Book b;
					IncreaseInventory(textId, b, BookFile);
					remove("BookFile.csv");
					rename("TEXT.csv", "BookFile.csv");
					/*****************************/
					int f = test.CalculateFine();
					listMember.clear();
					Member m;
					listMember = File_to_vector(MemberFile, m);
					int size = listMember.size();
					for (int i = 0; i < size; i++) {
						if (listMember[i].getID() == memberId)
							listMember[i].setFine(f);
					}

					ifstream file1(MemberFile, ios::in);
					if (!file1)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}
					ofstream file2("File.csv", ios::app);
					if (!file2)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}

					for (int i = 0; i < size; i++)
						file2 << listMember[i];
					file1.close();
					file2.close();
					remove("MemberFile.csv");
					rename("File.csv", "MemberFile.csv");
					/****************************/

				}
				else
					output << test;
			}
			output.close();
			input.close();
			remove("Trustbooks.csv");
			rename("Test.csv", "Trustbooks.csv");
			if (!check)
				throw Invalid("The login ID is not available!!");
		}

		case 2: {
			cout << "Enter the Magazine ID to Delete : ";
			cin >> textId;
			trusteeship test;
			ifstream input("TrustMagazine.csv", ios::in);
			if (!input)/*The file is checked before each action,
					   because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			ofstream output("Test.csv", ios::in);
			if (!output)/*The file is checked before each action,
						because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			while (input >> test) {
				if (test.getMemberID() == memberId && test.getTextID() == textId) {
					check = true;
					Magazine b;
					IncreaseInventory(textId, b, MagazineFile);
					remove("BookFile.csv");
					rename("TEXT.csv", "BookFile.csv");
					/*****************************/
					int f = test.CalculateFine();
					listMember.clear();
					Member m;
					listMember = File_to_vector(MemberFile, m);
					int size = listMember.size();
					for (int i = 0; i < size; i++) {
						if (listMember[i].getID() == memberId)
							listMember[i].setFine(f);
					}

					ifstream file1(MemberFile, ios::in);
					if (!file1)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}
					ofstream file2("File.csv", ios::app);
					if (!file2)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}

					for (int i = 0; i < size; i++)
						file2 << listMember[i];
					file1.close();
					file2.close();
					remove("MemberFile.csv");
					rename("File.csv", "MemberFile.csv");
					/****************************/

				}
				else
					output << test;
			}
			output.close();
			input.close();
			remove("TrustMagazine.csv");
			rename("Test.csv", "TrustMagazine.csv");
			if (!check)
				throw Invalid("The login ID is not available!!");
		}

		case 3:
		{
			cout << "Enter the Thesis ID to Delete : ";
			cin >> textId;
			trusteeship test;
			ifstream input("TrustThesis.csv", ios::in);
			if (!input)/*The file is checked before each action,
					   because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			ofstream output("Test.csv", ios::in);
			if (!output)/*The file is checked before each action,
						because all actions depend on the file*/
			{
				cout << "Oops!!The file will not open!\nTry another time.\n";
				abort();
			}
			while (input >> test) {
				if (test.getMemberID() == memberId && test.getTextID() == textId) {
					check = true;
					Thesis b;
					IncreaseInventory(textId, b, ThesisFile);
					remove("BookFile.csv");
					rename("TEXT.csv", "BookFile.csv");
					/*****************************/
					int f = test.CalculateFine();
					listMember.clear();
					Member m;
					listMember = File_to_vector(MemberFile, m);
					int size = listMember.size();
					for (int i = 0; i < size; i++) {
						if (listMember[i].getID() == memberId)
							listMember[i].setFine(f);
					}

					ifstream file1(MemberFile, ios::in);
					if (!file1)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}
					ofstream file2("File.csv", ios::app);
					if (!file2)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}

					for (int i = 0; i < size; i++)
						file2 << listMember[i];
					file1.close();
					file2.close();
					remove("MemberFile.csv");
					rename("File.csv", "MemberFile.csv");
					/****************************/

				}
				else
					output << test;
			}
			output.close();
			input.close();
			remove("TrustThesis.csv");
			rename("Test.csv", "TrustThesis.csv");
			if (!check)
				throw Invalid("The login ID is not available!!");
		}
		default:
			cout << "Incorrect number!!" << endl;
			again = true;
			break;
		}
	}


}

void Searchloan() {//function
	bool again = true;
	while (again)
	{
		again = false;
		int ans;
		cout << "1 . View members who have borrowed a specific article" << endl;
		cout << "2 . View borrowed entries" << endl;
		cin >> ans;
		switch (ans)
		{
		case 1:
		{vector<string>MemberId;
		bool agn = false;
		do {
			string idbook;
			int a;
			cout << "Select the text to display" << endl;
			cout << "1 . Book" << endl;
			cout << "2 . Magazine" << endl;
			cout << "3 . Thesis" << endl;
			cin >> a;
			switch (a)
			{
			case 1:
			{
				bool ch = true;
				do {

					cout << "Enter the book ID : ";
					cin >> idbook;
					ifstream ifile("Trustbooks.csv", ios::in);
					if (!ifile)/*The file is checked before each action,
								because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}
					trusteeship loan;
					while (ifile >> loan) {
						if (loan.getTextID() == idbook) {
							ch = false;
							MemberId.push_back(loan.getMemberID());
						}
					}
				} while (ch);
				break;
			}
			case 2: {
				bool c = true;
				do {


					cout << "Enter the Magazine ID : ";
					cin >> idbook;
					ifstream ifile("TrustMagazine.csv", ios::in);
					if (!ifile)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}
					trusteeship loan;
					while (ifile >> loan) {
						if (loan.getTextID() == idbook) {
							c = false;
							MemberId.push_back(loan.getMemberID());
						}
					}
				} while (c);
				break; }
			case 3:
			{
				bool ch = true;
				do {


					cout << "Enter the Thesis ID : ";
					cin >> idbook;
					ifstream ifile("TrustThesis.csv", ios::in);
					if (!ifile)/*The file is checked before each action,
							   because all actions depend on the file*/
					{
						cout << "Oops!!The file will not open!\nTry another time.\n";
						abort();
					}
					trusteeship loan;
					while (ifile >> loan) {
						if (loan.getTextID() == idbook) {
							ch = false;
							MemberId.push_back(loan.getMemberID());
						}
					}
				} while (ch);
				break;
			}
			default:
				cout << "Incorrect number!!" << endl;
				agn = true;
				break;
			}
		} while (agn);
		Member person;
		listMember.clear();
		listMember = File_to_vector(MemberFile, person);
		int s = listMember.size();
		for (int g = 0; g < MemberId.size(); g++) {
			for (int i = 0; i < s; i++)
			{
				if (listMember[i].getID() == MemberId[g])
					listMember[i].report();
			}
		}

		}//end of case 1

		case 2: {
			bool b = false;
			do {
				bool k = false;
				vector<string>book;
				vector<string>magazine;
				vector<string>thesis;
				string id;
				cout << "Enter Member ID : ";
				cin >> id;
				ifstream ifile1("Trustbooks.csv", ios::in);
				ifstream ifile2("TrustThesis.csv", ios::in);
				ifstream ifile3("TrustMagazine.csv", ios::in);
				if (!ifile1)/*The file is checked before each action,
						   because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				if (!ifile2)/*The file is checked before each action,
						   because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				if (!ifile3)/*The file is checked before each action,
						   because all actions depend on the file*/
				{
					cout << "Oops!!The file will not open!\nTry another time.\n";
					abort();
				}
				trusteeship trus;
				while (ifile1 >> trus) {
					if (trus.getMemberID() == id) {
						k = true;
						book.push_back(trus.getTextID());
					}

				}
				while (ifile2 >> trus) {
					if (trus.getMemberID() == id) {
						k = true;
						thesis.push_back(trus.getTextID());
					}

				}
				while (ifile3 >> trus) {
					if (trus.getMemberID() == id) {
						k = true;
						magazine.push_back(trus.getTextID());
					}

				}
				if (k) {
					listBook.clear();
					listMagazine.clear();
					listThesis.clear();
					Book b;
					Magazine M;
					Thesis T;
					listBook = File_to_vector(BookFile, b);
					listMagazine = File_to_vector(MagazineFile, M);
					listThesis = File_to_vector(ThesisFile, T);
					int sizebook = listBook.size();
					int sizemagazine = listMagazine.size();
					int sizethesis = listThesis.size();
					cout << "\nBorrowed writings\n";
					if (sizebook > 0) {
						cout << "\n***************Book****************\n";
						for (int i = 0; i < book.size(); i++)
						{
							for (int j = 0; j < sizebook; j++) {
								if (listBook[j].getID() == book[j])
									listBook[j].report();
							}
						}
					}
					if (sizemagazine > 0) {
						cout << "\n***************Magazine****************\n";
						for (int i = 0; i < magazine.size(); i++)
						{
							for (int j = 0; j < sizemagazine; j++) {
								if (listMagazine[j].getID() == magazine[j])
									listMagazine[j].report();
							}
						}
					}
					if (sizethesis > 0) {
						cout << "\n***************Thesis****************\n";
						for (int i = 0; i < thesis.size(); i++)
						{
							for (int j = 0; j < sizethesis; j++) {
								if (listThesis[j].getID() == thesis[j])
									listThesis[j].report();
							}
						}
					}


				}






				if (!k)
				{
					cout << "The login ID is not available!!\n";
					b = true;
				}

			} while (b);


		}


		default:
			cout << "Incorrect number!!" << endl;
			again = true;
			break;
		}//end of switch

	}//end of while

}


void Reportloan() {
	int year, month, day;
	cout << "\nEnter a date to view loans(year,month,day)\n";
	cin >> year >> month >> day;
	Birth b1(year, month, day);
	ifstream input1("Trustbooks.csv", ios::in);
	ifstream input2("TrustMagazine.csv", ios::in);
	ifstream input3("TrustThesis.csv", ios::in);
	if (!input1)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	if (!input2)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	if (!input3)/*The file is checked before each action,
				because all actions depend on the file*/
	{
		cout << "Oops!!The file will not open!\nTry another time.\n";
		abort();
	}
	trusteeship object;
	bool h = false;
	while (input1 >> object) {
		if (object.getDeliveryDate() == b1) {
			object.report();
			h = true;
		}
	}

	while (input2 >> object) {
		if (object.getDeliveryDate() == b1) {
			object.report();
			h = true;
		}
	}
	while (input3 >> object) {
		if (object.getDeliveryDate() == b1) {
			object.report();
			h = true;
		}
	}
	input1.close();
	input2.close();
	input3.close();
	if (!h) {
		cout << "\nThere is no such date\n";
		return;
	}

}




void LibrarianMenu() {
	bool t = true;
	while (t) {
		t = false;
		int a;
		system("cls");
		cout << "\n\n************************************************************************************************************************\n";
		cout << "\t\t\t\t\t\t     Dear Librarian\n\n";
		cout << "\t\t\t\t\t Welcome to the Library Management System\n\n";
		cout << "************************************************************************************************************************\n\n";
		cout << "\t\t\t\t\t\t1 . Book\n";
		cout << "\t\t\t\t\t\t2 . Magazine\n";
		cout << "\t\t\t\t\t\t3 . Thesis\n";
		cout << "\t\t\t\t\t\t4 . Member\n";
		cout << "\t\t\t\t\t\t5 . Librarian\n";
		cout << "\t\t\t\t\t\t6 . trusteeship\n";
		cout << "\t\t\t\t\t\t7 . Return to the previous menu\n";
		cin >> a;
		switch (a)
		{
		case 1: {
			bool r = false;
			do {
				int b;
				cout << "\t\t\t\t\t\tPlease Choose from the following options\n";
				cout << "1 . Record" << endl;
				cout << "2 . Edit" << endl;
				cout << "3 . Delete" << endl;
				cout << "4 . Search" << endl;
				cout << "5 . Report" << endl;
				cin >> b;
				switch (b)
				{
				case 1: {
					char s;
					do {
						AddBook();
						cout << "Do you want to continue?(y/n)";
						cin >> s;
					} while (s == 'y' || s == 'Y');
					break;
				}
				case 2:
				{
					string idb;
					while (true) {
						try {
							cout << "Enter Book ID to editing : ";
							cin >> idb;
							EditBook(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break; }
				case 3:
				{
					string idb;
					while (true) {
						try {
							cout << "Enter Book ID to Deleting : ";
							cin >> idb;
							DeleteBook(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 4:
				{
					string idb;

					Book b2;
					vector<Document*> vecD;
					ifstream input(BookFile, ios::in);
					while (input >> b2)
						vecD.push_back(&b2);
					while (true) {

						try {
							cout << "Enter Book ID to Searching : ";
							cin >> idb;
							Search_D(vecD, idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 5:
				{

					Book b2;
					vector<Document*> vecD;
					ifstream input(BookFile, ios::in);
					while (input >> b2)
						vecD.push_back(&b2);
					Report_D(vecD);
					break;
				}
				default:
					r = true;
					break;
				}
			} while (r);
			break;
		}
		case 2:
		{
			bool r = false;
			do {
				int b;
				cout << "\t\t\t\t\t\tPlease Choose from the following options\n";
				cout << "1 . Record" << endl;
				cout << "2 . Edit" << endl;
				cout << "3 . Delete" << endl;
				cout << "4 . Search" << endl;
				cout << "5 . Report" << endl;
				cin >> b;
				switch (b)
				{
				case 1: {
					char s;
					do {
						AddMagazine();
						cout << "Do you want to continue?(y/n)";
						cin >> s;
					} while (s == 'y' || s == 'Y');
					break;
				}
				case 2:
				{
					string idb;
					while (true) {
						try {
							cout << "Enter Magazine ID to editing : ";
							cin >> idb;
							EditMagazine(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 3:
				{
					string idb;
					while (true) {
						try {
							cout << "Enter Magazine ID to Deleting : ";
							cin >> idb;
							DeleteMagazine(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 4:
				{
					string idb;
					Magazine b2;
					vector<Document*>vD;
					ifstream input(MagazineFile, ios::in);
					while (input >> b2)
						vD.push_back(&b2);
					input.close();
					while (true) {

						try {
							cout << "Enter Magazine ID to Searching : ";
							cin >> idb;
							Search_D(vD, idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 5:
				{
					Magazine b2;
					vector<Document*>vecD;
					ifstream input(MagazineFile, ios::in);
					while (input >> b2)
						vecD.push_back(&b2);
					input.close();

					Report_D(vecD);
					break;
				}
				default:
					r = true;
					break;
				}
			} while (r);
			break;
		}
		case 3:
		{
			bool r = false;
			do {
				int b;
				cout << "\t\t\t\t\t\tPlease Choose from the following options\n";
				cout << "1 . Record" << endl;
				cout << "2 . Edit" << endl;
				cout << "3 . Delete" << endl;
				cout << "4 . Search" << endl;
				cout << "5 . Report" << endl;
				cin >> b;
				switch (b)
				{
				case 1: {
					char s;
					do {
						AddThesis();
						cout << "Do you want to continue?(y/n)";
						cin >> s;
					} while (s == 'y' || s == 'Y');
					break;
				}
				case 2: {
					string idb;
					while (true) {
						try {
							cout << "Enter Thesis ID to editing : ";
							cin >> idb;
							EditThesis(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break; }
				case 3:
				{
					string idb;
					while (true) {
						try {
							cout << "Enter Thesis ID to Deleting : ";
							cin >> idb;
							DeleteThesis(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 4:
				{
					string idb;
					Thesis b2;
					vector<Document*>vecD;
					ifstream input(ThesisFile, ios::in);
					while (input >> b2)
						vecD.push_back(&b2);
					input.close();
					while (true) {

						try {
							cout << "Enter Thesis ID to Searching : ";
							cin >> idb;
							Search_D(vecD, idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 5:
				{

					Thesis b2;
					vector<Document*>vecD;
					ifstream input(ThesisFile, ios::in);
					while (input >> b2)
						vecD.push_back(&b2);
					input.close();
					Report_D(vecD);
					break;
				}
				default:
					r = true;
					break;
				}
			} while (r);
			break;
		}
		case 4:
		case 5:
		{
			bool r = false;
			do {
				int b;
				cout << "\t\t\t\t\t\tPlease Choose from the following options\n";
				cout << "1 . Record" << endl;
				cout << "2 . Edit" << endl;
				cout << "3 . Delete" << endl;
				cout << "4 . Search" << endl;
				cout << "5 . Report" << endl;
				cin >> b;
				switch (b)
				{
				case 1: {
					char s;
					do {
						AddLibrarian();
						cout << "Do you want to continue?(y/n)";
						cin >> s;
					} while (s == 'y' || s == 'Y');
					break;
				}
				case 2:
				{
					string idb;
					while (true) {
						try {
							cout << "Enter Librarian ID to editing : ";
							cin >> idb;
							EditLibrarian(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 3:
				{
					string idb;
					while (true) {
						try {
							cout << "Enter Librarian ID to Deleting : ";
							cin >> idb;
							DeleteLibrarian(idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 4:
				{
					ifstream out(LibrarianFile, ios::in);
					vector<Person*> vecP;
					Librarian b2;
					while (out >> b2)
						vecP.push_back(&b2);
					string idb;
					while (true) {

						try {
							cout << "Enter Librarian ID to Searching : ";
							cin >> idb;
							Search(vecP, idb);
							break;
						}
						catch (Invalid obj) {
							cout << obj.getMessage() << endl;
						}
					}
					break;
				}
				case 5:
				{

					ifstream outFile(LibrarianFile, ios::in);
					vector<Person*> vecP;
					Librarian b2;
					while (outFile >> b2)
						vecP.push_back(&b2);
					Report(vecP);
					outFile.close();
					break;
				}
				default:
					r = true;
					break;
				}
			} while (r);
			break;
		}
		case 6:
		case 7:
			break;
		default:
			cout << "Incorrect number!..Try again.." << endl;
			t = true;
			break;
		}//end switch




	}// end of while


}



void MemberMenu() {

	bool t = true;
	while (t) {
		t = false;
		int a;
		system("cls");
		cout << "\n\n************************************************************************************************************************\n";
		cout << "\t\t\t\t\t\t     Dear Member\n\n";
		cout << "\t\t\t\t\t Welcome to the Library Management System\n\n";
		cout << "************************************************************************************************************************\n\n";
		cout << "\t\t\t\t\t\t1 . Lend registration\n";
		cout << "\t\t\t\t\t\t2 . Search articles\n";
		cout << "\t\t\t\t\t\t3 . Report articles\n";
		cout << "\t\t\t\t\t\t4 . Return to the previous menu\n";
		cin >> a;
		switch (a)
		{
		case 1:
			char s;
			do {
				Addloan();
				cout << "Do you want to continue?(y/n)";
				cin >> s;
			} while (s == 'y' || s == 'Y');
			break;

		case 2:
			Searchloan();
			break;
		case 3:
			Reportloan();
			break;



		case 4:
			break;
		default:
			cout << "Incorrect number!..Try again.." << endl;
			t = true;
			break;
		}//end switch




	}// end of while




}




int main()
{
	char ans;
	bool repeat = true;
	while (repeat)
	{
		//system("cls");

		cout << "\n\n*********************************************  Library Management System  **********************************************\n\n";
		cout << "\n\t\t\t\t\t\t       Login as : \n\n\n";
		cout << "\t\t\t\t\t\t     1 . Librarian  \n\n\n";
		cout << "\t\t\t\t\t\t     2 . Member     \n\n\n";
		cout << "\t\t\t\t   -Enter 'B' to go to the previous step and 'E' to exit- \n\n\n\n";
		cout << "\t\t\t\t\t\t   ";
		cin >> ans;
		switch (ans)
		{
		case '1': {
			int count = -1;
			while (count <= 3) {
				try {
					//login as librarian
					string ncode;
					string id;

					cout << "\n\n\t\t\t\t\t\t*********************\n\n";
					cout << "\t\t\t\t\t\t        Log in\n\n";
					cout << "\t\t\t\t\t\t*********************\n\n";
					cout << "\t\t\t\t\t\tEnter your national code : ";
					cin >> ncode;
					cout << "\t\t\t\t\t\tEnter your ID : ";
					id = get_password();
					Librarian object;
					listLibrarian.clear();
					long int size = get_size_file(LibrarianFile);
					int Size = listLibrarian.size();
					bool chec = false;
					/***********/
					ifstream input(LibrarianFile, ios::in);
					for (long int i = 0; i < size; i++) {
						input >> object;
						listLibrarian.push_back(object);
					}
					input.close();
					for (int i = 0; i < Size; i++)
					{
						if (listLibrarian[i].getNationalCode() == ncode && listLibrarian[i].getID() == id) {
							chec = true;
							LibrarianMenu();
							break;
						}
					}

					if (!chec)
						throw Invalid("The login ID is not available!");


				}
				catch (Invalid obj) {
					cout << "\n\n\n";
					cout << "                                            ";
					cout << obj.getMessage() << endl;
					cout << "\t\t\t\t\t\t   Try again!!\n\n\n";
					count++;
				}
			}


			//end of case
			break;
		}
		case '2':
		{
			int count = -1;
			while (count <= 3) {
				try {
					//login as Member
					string ncode;
					string id;
					system("cls");
					cout << "\n\n\t\t\t\t\t\t*********************\n\n";
					cout << "\t\t\t\t\t\t        Log in\n\n";
					cout << "\t\t\t\t\t\t*********************\n\n";
					cout << "\t\t\t\t\t\tEnter your national code : ";
					cin >> ncode;
					cout << "\t\t\t\t\t\tEnter your ID : ";
					id = get_password();
					Member object;
					listMember.clear();
					int Size = listMember.size();
					bool chec = false;
					listMember = File_to_vector(MemberFile, object);
					for (int i = 0; i < Size; i++)
					{
						if (listMember[i].getNationalCode() == ncode && listMember[i].getID() == id) {
							chec = true;
							MemberMenu();
							break;
						}
					}

					if (!chec)
						throw Invalid("The login ID is not available!");


				}
				catch (Invalid obj) {
					cout << "\t\t\t\t\t\t   " << obj.getMessage() << endl;
					cout << "\t\t\t\t\t\t   Try again!!";
					count++;
				}
			}


			//end of case
			break;
		}
		case 'B': {
			repeat = false;
			break; }
		case 'b':
		{
			repeat = false;
			break;
		}
		case 'E':
		{
			repeat = false;
			break;
		}
		case 'e':
		{
			repeat = false;
			break;
		}
		default: {
			cout << "\n\t\t\t\t\t\t        wrong!!\n" << endl;
			cout << "\t\t\t\t\t\t   Choose carefully!\n" << endl;
			break; }
		}//end of switch



	}//end of while


	return 0;
}



