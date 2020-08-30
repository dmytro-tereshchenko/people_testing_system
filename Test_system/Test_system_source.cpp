//Реализовать полнофункциональную систему тестирования.
//В системе должны быть реализовано 2 режима:
//■■ Администратор;
//■■ Тестируемый.
//Описание режима работа для Тестируемого(в дальнейшем гость)
//Для входа в систему гость должен зарегистрироваться.Данная процедура вы -
//полняется один раз, при дальнейших входах в систему доступ идет по логину
//и паролю.
//При регистрации нужно указывать ФИО, домашний адрес, телефон.
//Важно, чтобы логины для пользователей были уникальными.
//После входа гость имеет возможность просмотреть свои предыдущие резуль -
//таты тестирования, сдать новое тестирование.
//Тестирование может осуществляться по различным категориям знаний.
//Например:
//Математика(раздел) – > Дискретная математика(конкретный тест);
//– > Математический анализ(конкретный тест).
//Физика(раздел) – > Квантовая физика(конкретный тест);
//– > Механика(конкретный тест).
//После сдачи теста гость видит результат тестирования, количество правильно
//отвеченных вопросов, процент правильных ответов и полученную оценку.
//Студент имеет возможность прервать тестирование и продолжить его тогда,
//когда ему это будет удобно.
//Оценивание нужно вести на основании 12 - балльной системы, привязанной
//к количеству вопросов теста.
//Пароли и логины гостей хранятся в зашифрованном виде.
//Описание режима работа для Администратора(в дальнейшем админ)
//В системе может быть только один админ, логин и пароль админа задается
//при первом входе в программу.
//В дальнейшем пароль и логин можно изменить(но данную возможность имеет
//	только админ).
//	Пароль и логин необходимо хранить только в зашифрованном виде.
//	При работе с системой админ имеет следующие возможности :
//• Управление пользователями – создание, удаление, модификация поль -
//зователей.
//• Просмотр статистики – просмотр результатов тестирования в общем
//по категориям, по конкретным тестам, по конкретным пользователям.
//Результаты просмотра статистики можно вывести в файл.
//• Управление тестированием – админ имеет возможность добавлять ка -
//тегории, тесты, вопросы к тестам, задавать правильные и неправиль -
//ные ответы, импортировать и экспортировать категории и тесты с
//вопросами из файла(и в файл).

#pragma once

#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>
#include <direct.h>


using namespace std;

class Admin;
class Category;
class Test;
class PassedTest;

class User {//абстрактный класс
private:
	bool typeAccount;//тип аккаунта, 1 - админ, 0 - пользователь (тестируемый)
	int idAccount;//номер учетной записи
	static int countAccounts;//колличество учетных записей
protected:
	string login;//логин учетной записи
	string password;//пароль учетной записи
	string name;//ФИО учетной записи
	string address;//Адрес
	string telephone;//номер телефона
	string path;//путь хранинения файла с архивом
	friend Admin;
public:
	User(string telephone, string address, string name, string password, string login, string path)
		:login(login), password(password), name(name), address(address), telephone(telephone), path(path)
	{
		typeAccount = countAccounts == 0 ? 1 : 0;
		idAccount = countAccounts++;

	};
	virtual ~User() = 0;
	virtual void SetLogin(User* user, string newLogin) {}
	virtual void SetName(User* user, string newName) {}
	virtual void SetPassword(User* user, string newPassword) {}
	virtual void SetAddress(User* user, string newAddress) {};
	virtual void SetTelephone(User* user, string newTelephone) {}
	virtual void DelUser(vector <User*>& users, vector<User*>::const_iterator position) {}
	virtual void AddCategory(vector <Category> &categorys, string newNameCategory, string pathCategory) {}
	virtual void AddTest(vector <Test>& tests, string newNameTest, string nameCategory, string pathCategory) {}
	virtual void ChooseQuestion_SetCorrectUnswers(Test& test) {}
	virtual void AddQuestion(Test& test, string textQuestion, vector <string> &textAnsw, vector <bool> correctAnsw) {}
	virtual void ShowPassedTestName(vector <PassedTest>& resultsTest, string name) {}
	virtual void ShowPassedTestCategory(vector <PassedTest>& resultsTest, string category) {}
	virtual void ShowPassedTestTest(vector <PassedTest>& resultsTest, string test) {}
	string GetLogin() {
		return login;
	}
	string GetPassword() {
		return password;
	}
	string GetName() {
		return name;
	}
	string GetAddress() {
		return address;
	}
	string GetTelephone() {
		return telephone;
	}
	bool GetTypeAccount() {
		return typeAccount;
	}
	int GetIdAccount() {
		return idAccount;
	}
	static int GetCountAccounts() {
		return countAccounts;
	}
	string GetPathFile() {
		return path;
	}
	void Print() {
		SetConsoleOutputCP(1251);
		cout << "логин: " << login << " имя: " << name << " адрес: " << address << " телефон: " << telephone << endl;
		SetConsoleOutputCP(866);
	}
};

int User::countAccounts = 0;
User::~User() {}

class Category {
	vector <Test> tests;//массив тестов по предметам данной категории
protected:
	string nameCategory;//имя категории
	string path;//путь хранинения файла с архивом
public:
	Category(string nameCategory, string path) :nameCategory(nameCategory), path(path) {}
	~Category() {
	}
	string GetNameCategory() {
		return nameCategory;
	}
	vector <Test> &GetTests() {
		return tests;
	}
	Test &GetTest(int index) {
		return tests.at(index);
	}
};

class Test :public Category {
protected:
	class Question {
	public:
		string textQuestion;//текст вопроса
		vector <string> textAnswers;//текст возможных четверых ответов
		vector <bool> correctAnswers;//массив с метками правильных ответов 1 - верно, 0 - неверно
		Question(const Question &obj) :textQuestion(obj.textQuestion) {
			this->textAnswers = obj.textAnswers;
			this->correctAnswers = obj.correctAnswers;
		}
		Question(string textQuestion, vector <string>& textAnsw, vector <bool> &correctAnsw) :textQuestion(textQuestion) {
			this->textAnswers = textAnsw;
			this->correctAnswers = correctAnsw;
		}
		~Question() {}
		void Print() {
			SetConsoleOutputCP(1251);
			cout << textQuestion << endl;
			for (int i = 0; i < 4; i++) {
				cout << " " << i + 1 << " - " << textAnswers.at(i) << endl;
			}
			SetConsoleOutputCP(866);
		}
	};
	string nameTest;//имя теста по предмету
	vector <Question> questions;//массив вопросов даного теста
	void AddQuestion(string textQuestion, vector <string>& textAnsw, vector <bool>& correctAnsw) {
		Question newQuestion(textQuestion, textAnsw, correctAnsw);
		questions.push_back(newQuestion);
	}
	void WriteStringToFile(ofstream& fout, string str) {
		int tempSize = 0;//переменная для хранения размера записываемой строки
		tempSize = str.length() * sizeof(char);//расчет размера записываемой строки
		fout.write((char*)&tempSize, sizeof(int));//запись в файл размера строки
		fout.write((char*)str.c_str(), tempSize);//запись в файл самой строки (string конвертируем в Char перед записью)
	}
	string ReadStringFromFile(ifstream& fin) {//считывание из файла одной строки типа string
		string tempData;//переменная для строки
		char* tempStr = nullptr;//временная строка для хранения данных
		int tempSize = 0;//временная переменная для считывания из файла размера считываемой строки
		fin.read((char*)&tempSize, sizeof(int)); //читаем размер строки
		tempStr = new char[tempSize / sizeof(char) + 1];//выделяем память под запись из файла строки, +1 для символка окончания строки '\0'
		fin.read((char*)&*tempStr, tempSize);//читаем строку
		tempStr[tempSize / sizeof(char)] = '\0';//записываем в строку символ окончания строки (для отсечения мусора)
		tempData = tempStr;//записываем строку в переменную для хранения
		if (tempStr != nullptr) {//высвобождаем память временной строки
			delete[] tempStr;
			tempStr = nullptr;
		}
		return tempData;
	}
	friend Admin;
public:
	Test(string nameTest, string nameCategory, string path) : Category(nameCategory, path), nameTest(nameTest) {}
	Test(const Test& obj) :Category(obj.nameCategory, obj.path) {
		this->nameTest = obj.nameTest;
		this->questions.reserve(obj.questions.size());
		for (int i = 0; i < obj.questions.size(); i++) {
			this->questions.push_back(obj.questions.at(i));
		}
	}
	~Test() {}
	void PrintQuestion(int index) {
		if (index < questions.size()) {
			questions.at(index).Print();
		}
	}
	string GetNameTest() {
		return nameTest;
	}
	int GetSizeQuestions() {
		return questions.size();
	}
	void SaveFileQuestions(ofstream& fout) {
		int sizeQuestions = questions.size();
		bool tempBool;
		fout.write((char*)&sizeQuestions, sizeof(int));//запись в файл колличества вопросов
		for (int index_quest = 0; index_quest < sizeQuestions; index_quest++) {
			WriteStringToFile(fout, questions.at(index_quest).textQuestion);
			int sizeUnswer = questions.at(index_quest).textAnswers.size();
			fout.write((char*)&sizeUnswer, sizeof(int));//запись в файл колличества ответов
			for (int index_unsw = 0; index_unsw < sizeUnswer; index_unsw++) {
				WriteStringToFile(fout, questions.at(index_quest).textAnswers.at(index_unsw));
				tempBool = questions.at(index_quest).correctAnswers.at(index_unsw);
				fout.write((char*)&tempBool, sizeof(bool));//метки правильного ответа
			}
		}
	}
	void LoadFileQuestions(ifstream& fin) {
		if (fin.is_open()) {
			int sizeQuestions;
			int sizeUnswers;
			bool tempBool;
			string tempStr;
			string tempTextQuestion;
			vector <string> tempTextAnsw;
			vector <bool> tempCorrectAnsw;
			fin.read((char*)&sizeQuestions, sizeof(int));//считываем количество записанных в файле вопросов
			for (int index_quest = 0; index_quest < sizeQuestions; index_quest++) {
				tempTextQuestion = ReadStringFromFile(fin);
				fin.read((char*)&sizeUnswers, sizeof(int));//считываем количество записанных в файле ответов на вопросы
				tempTextAnsw.reserve(sizeUnswers);
				tempCorrectAnsw.reserve(sizeUnswers);
				for (int index_unsw = 0; index_unsw < sizeUnswers; index_unsw++) {
					tempStr = ReadStringFromFile(fin);
					tempTextAnsw.push_back(tempStr);
					fin.read((char*)&tempBool, sizeof(bool));//считываем метки правильных ответов
					tempCorrectAnsw.push_back(tempBool);
				}
				Question tempQuestion(tempTextQuestion, tempTextAnsw, tempCorrectAnsw);
				questions.push_back(tempQuestion);
				tempTextAnsw.clear();
				tempCorrectAnsw.clear();
			}
		}
	}
};

class PassedTest :public Test {
private:
	vector<int>answers;//результаты ответов на вопросы (позиция выбраного ответа 1-4)
	int sizeCorrectAnswers;//количество правильных ответов
	float percCorrectAnsw;//процент правильных ответов
	int grade;//оценка, 12-ти бальная системма
	int currentQuestion;//позиция текущего вопроса
	bool passedTest;//метка сданого теста: 1 - сдан, 0 - в процессе
	string nameUser;//имя студента сдающего тест
	int ChooseAnswerQuestion() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			if (error != "") {
				cout << error << endl;
			}
			cout << "Введите вариант ответа ";
			cin >> key;
			if (key < 49 || key > 52) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 52);
		SetConsoleOutputCP(866);
		return (int)key - 49;
	}
	bool ChooseExitTest() {//проверка досрочного выхода из теста, 1 - да, 0 - продолжаем
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			if (error != "") {
				cout << error << endl;
			}
			cout << "Продолжить тест - 1, выйти из тестирования - 2" << endl;
			cin >> key;
			if (key < 49 || key > 50) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 50);
		SetConsoleOutputCP(866);
		return (int)key - 49;
	}
public:
	PassedTest(Test &test, string nameUser): Test(test), sizeCorrectAnswers(0), percCorrectAnsw(0), grade(0), currentQuestion(0), passedTest(0), nameUser(nameUser){}
	PassedTest(Test &test, string nameUser, int sizeCorrectAnswers, float percCorrectAnsw, int grade, int currentQuestion, bool passedTest, vector<int>&answers)
		: Test(test), sizeCorrectAnswers(sizeCorrectAnswers), percCorrectAnsw(percCorrectAnsw), grade(grade), currentQuestion(currentQuestion), passedTest(passedTest), nameUser(nameUser)
	{
		this->answers = answers;
	}
	~PassedTest() {}
	void PassTest() {
		for (int i = currentQuestion; i < questions.size(); i++) {//ответы на вопросы
			system("cls");
			cout << currentQuestion + 1 << ". ";
			questions.at(i).Print();
			answers.push_back(ChooseAnswerQuestion());
			if (questions.at(i).correctAnswers.at(answers.at(currentQuestion++)) == 1) {
				sizeCorrectAnswers++;
			}
			if (ChooseExitTest() && currentQuestion != questions.size()) { //не конец теста и желаем выйти досрочно
				return;
			}
		}
		percCorrectAnsw = sizeCorrectAnswers * 100 / questions.size();
		grade = 2 + 10 * percCorrectAnsw / 100;
		passedTest = 1;
		system("cls");
		Print();
		SetConsoleOutputCP(1251);
		cout << "Произведите любой ввод с клавиатуры для продолжения...";
		char temp;
		cin >> temp;
		SetConsoleOutputCP(866);
	}
	void Print() {
		SetConsoleOutputCP(1251);
		cout << "Тест по " << nameCategory << "/" << nameTest;
		cout << " Правильных ответов " << sizeCorrectAnswers << " Процент ответов " << percCorrectAnsw << "% Оценка: " << grade << endl;
		SetConsoleOutputCP(866);
	}
	string GetNameUser() {
		return nameUser;
	}
	int GetSizeCorrectAnswers() {
		return sizeCorrectAnswers;
	}
	float GetPercCorrectAnsw() {
		return percCorrectAnsw;
	}
	int GetGrade() {
		return grade;
	}
	int GetCurrentQuestion() {
		return currentQuestion;
	}
	bool GetPassedTest() {
		return passedTest;
	}
	vector<int> GetAnswers() {
		return answers;
	}
};

class TestUser :public User {
public:
	TestUser(string telephone, string address, string name, string password, string login, string path)
		:User(telephone, address, name, password, login, path) {}
	~TestUser() {}
	void ShowPassedTestName(vector <PassedTest>& resultsTest, string name) override {
		system("cls");
		int index = 1;
		for (auto passTest : resultsTest) {
			if (name == passTest.GetNameUser() && passTest.GetPassedTest()) {
				cout << index++ << ". ";
				passTest.Print();
			}
		}
		SetConsoleOutputCP(1251);
		cout << "Произведите любой ввод с клавиатуры для продолжения...";
		char temp;
		cin >> temp;
		SetConsoleOutputCP(866);
	}
};

class Admin :public User {
private:
	int ChooseQuestion(Test& test) {
		int key;
		string error = "";
		int index;
		SetConsoleOutputCP(1251);
		if (!test.questions.size()) {//если база с вопросами пустая тогда выходим с кодом "-1"
			return -1;
		}
		do {
			index = 1;
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "Выберите тест" << endl;
			for (auto question : test.questions) {
				cout << index++ << " - " << question.textQuestion << endl;
			}
			cin >> key;
			if (key < 1 || key > test.questions.size()) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 1 || key > test.questions.size());
		SetConsoleOutputCP(866);
		return key - 1;
	}
	void SetCorrectUnswers(Test& test, int indexQuestion) {
		if (indexQuestion == -1) {//если вопросов нет, то выходим
			return;
		}
		SetConsoleOutputCP(1251);
		bool temp;
		cout << "Введите правильные ответы (1 - верно, 0 - неверно)" << endl;
		for (int i = 0; i < 4; i++) {
			cout << test.questions.at(indexQuestion).textAnswers.at(i) << " ";
			cin >> temp;
			test.questions.at(indexQuestion).correctAnswers.at(i) = temp;
			if (cin.peek() == '\n') {
				cin.ignore();
			}
		}
		SetConsoleOutputCP(866);
	}
public:
	Admin(string telephone, string address, string name, string password, string login, string path) :User(telephone, address, name, password, login, path) {}
	~Admin() {}
	void SetLogin(User* user, string newLogin) override {
		if (this->GetTypeAccount()) {
			user->login = newLogin;
		}
	}
	void SetName(User* user, string newName) override {
		if (this->GetTypeAccount()) {
			user->name = newName;
		}
	}
	void SetPassword(User* user, string newPassword) override {
		if (this->GetTypeAccount()) {
			user->password = newPassword;
		}
	}
	void SetAddress(User* user, string newAddress) override {
		if (this->GetTypeAccount()) {
			user->address = newAddress;
		}
	}
	void SetTelephone(User* user, string newTelephone) override {
		if (this->GetTypeAccount()) {
			user->telephone = newTelephone;
		}
	}
	void DelUser(vector <User*>& users, vector<User*>::const_iterator position) override {
		if (this->GetTypeAccount()) {
			users.erase(position);
		}
	}
	void AddCategory(vector <Category>& categorys, string newNameCategory, string pathCategory) override {
		if (this->GetTypeAccount()) {
			Category tempCat(newNameCategory, pathCategory);
			categorys.push_back(tempCat);
		}
	}
	void AddTest(vector <Test>& tests, string newNameTest, string nameCategory, string pathCategory) override {
		if (this->GetTypeAccount()) {
			Test tempTest(newNameTest, nameCategory, pathCategory);
			tests.push_back(tempTest);
		}
	}
	void ChooseQuestion_SetCorrectUnswers(Test& test) override {
		if (this->GetTypeAccount()) {
			SetConsoleOutputCP(1251);
			if (!test.questions.size()) {//если вопросов в тесте нет тогда выходим с кодом
				return;
			}
			SetCorrectUnswers(test, ChooseQuestion(test));
		}
	}
	void AddQuestion(Test& test, string textQuestion, vector <string>& textAnsw, vector <bool> correctAnsw) override {
		if (this->GetTypeAccount()) {
			test.AddQuestion(textQuestion, textAnsw, correctAnsw);
		}
	}
	void ShowPassedTestName(vector <PassedTest>& resultsTest, string name) override {
		if (this->GetTypeAccount()) {
			system("cls");
			int index = 1;
			for (auto passTest : resultsTest) {
				if (name == passTest.GetNameUser() && passTest.GetPassedTest()) {
					cout << index++ << ". ";
					passTest.Print();
				}
			}
			SetConsoleOutputCP(1251);
			cout << "Произведите любой ввод с клавиатуры для продолжения...";
			char temp;
			cin >> temp;
			SetConsoleOutputCP(866);
		}
	}
	void ShowPassedTestCategory(vector <PassedTest>& resultsTest, string category) override {
		if (this->GetTypeAccount()) {
			system("cls");
			int index = 1;
			for (auto passTest : resultsTest) {
				if (category == passTest.GetNameCategory() && passTest.GetPassedTest()) {
					cout << index++ << ". ";
					passTest.Print();
				}
			}
			SetConsoleOutputCP(1251);
			cout << "Произведите любой ввод с клавиатуры для продолжения...";
			char temp;
			cin >> temp;
			SetConsoleOutputCP(866);
		}
	}
	void ShowPassedTestTest(vector <PassedTest>& resultsTest, string test) override {
		if (this->GetTypeAccount()) {
			system("cls");
			int index = 1;
			for (auto passTest : resultsTest) {
				if (test == passTest.GetNameTest() && passTest.GetPassedTest()) {
					cout << index++ << ". ";
					passTest.Print();
				}
			}
			SetConsoleOutputCP(1251);
			cout << "Произведите любой ввод с клавиатуры для продолжения...";
			char temp;
			cin >> temp;
			SetConsoleOutputCP(866);
		}
	}
};

class Encryptor {
	int keyEncryption;
public:
	Encryptor(int keyEncryption) :keyEncryption(keyEncryption) {}
	~Encryptor() {}
	string Encryption(string str) {
		string temp;
		for (int i = 0; i < str.length(); i++) {
			temp.push_back(str[i] - keyEncryption);
		}
		return temp;
	}
	string UnEncryption(string str) {
		string temp;
		for (int i = 0; i < str.length(); i++) {
			temp.push_back(str[i] + keyEncryption);
		}
		return temp;
	}
};

class Menu {
public:
	Menu() {}
	~Menu() {}
	int InitialLogin(int sizeMembers) {
		char key = '1';
		if (sizeMembers) {
			string error = "";
			SetConsoleOutputCP(1251);
			do {
				system("cls");
				if (error != "") {
					cout << error << endl;
				}
				cout << "Система тестирования" << endl;
				cout << "1 - Создать учетную запись" << endl;
				cout << "2 - Войти в учетную запись" << endl;
				cout << "3 - Выйти из программы" << endl;
				cin >> key;
				if (key < 49 || key>51) {
					error = "Ошибка, повторите ввод";
				}
			} while (key < 49 || key>51);
			SetConsoleOutputCP(866);
		}
		return (int)key - 48;
	}
	int TestUserChoosAction() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Просмотреть свои предыдущие результаты тестирования" << endl;
			cout << "2 - Пройти новое тестирование" << endl;
			cout << "3 - Продолжить старое тестирование" << endl;
			cout << "4 - Выход из программы" << endl;
			cin >> key;
			if (key < 49 || key>52) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key>52);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}
	int AdminChoosAction() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Управление пользователями" << endl;
			cout << "2 - Просмотр статистики" << endl;
			cout << "3 - Управление тестированием" << endl;
			cout << "4 - Выход из программы" << endl;
			cin >> key;
			if (key < 49 || key>52) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key>52);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}
	int ChooseCategory(vector <Category> &categorys) {
		char key;
		string error = "";
		int index;
		SetConsoleOutputCP(1251);
		if (!categorys.size()) {//если база с тестами пустая тогда выходим с кодом "-1"
			return -1;
		}
		do {
			index = 1;
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "Выберите категорию" << endl;
			for (auto category : categorys) {
				cout << index++ << " - " << category.GetNameCategory() << endl;
			}
			cin >> key;
			if (key < 49 || key > categorys.size() + 48) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > categorys.size() + 48);
		SetConsoleOutputCP(866);
		return (int)key - 49;
	}
	int ChooseTest(vector <Test>& tests) {
		char key;
		string error = "";
		int index;
		SetConsoleOutputCP(1251);
		if (!tests.size()) {//если база с тестами пустая тогда выходим с кодом "-1"
			return -1;
		}
		do {
			index = 1;
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "Выберите тест" << endl;
			for (auto test : tests) {
				cout << index++ << " - " << test.GetNameTest() << endl;
			}
			cin >> key;
			if (key < 49 || key > tests.size() + 48) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > tests.size() + 48);
		SetConsoleOutputCP(866);
		return (int)key - 49;
	}
	int ChooseAdminActionCategory() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Выбор категории" << endl;
			cout << "2 - Создание категории" << endl;
			cout << "3 - Сохранить категории с тестами в файл" << endl;
			cout << "4 - Загрузить категории с тестами из файла" << endl;
			cin >> key;
			if (key < 49 || key > 52) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 52);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}
	int ChooseAdminActionTest() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Выбор теста" << endl;
			cout << "2 - Создание теста" << endl;
			cin >> key;
			if (key < 49 || key > 50) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 50);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}
	int ChooseAdminActionQuestion() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Выбор вопроса для определения правильных/неправильных ответов" << endl;
			cout << "2 - Добавление вопроса в тест" << endl;
			cin >> key;
			if (key < 49 || key > 50) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 50);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}
	int UserManagement() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Создание пользователя" << endl;
			cout << "2 - Удаление пользователя" << endl;
			cout << "3 - Модификация пользователя" << endl;
			cout << "4 - Выход из программы" << endl;
			cin >> key;
			if (key < 49 || key > 52) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 52);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}
	int ChooseUser(vector <User*>& users) {
		char key;
		int index;
		string error = "";
		do {
			SetConsoleOutputCP(1251);
			system("cls");
			index = 1;
			if (error != "") {
				cout << error << endl;
			}
			cout << "Выберите пользователя" << endl;
			for (auto user : users) {
				//cout << index++ << " - Login: " << user->GetLogin() << " Name: " << user->GetName() << endl;
				cout << index++ << " ";
				user->Print();
			}
			cin >> key;
			if (key < 49 || key > users.size() + 48) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > users.size() + 48);
		SetConsoleOutputCP(866);
		return (int)key - 49;
	}
	int ChooseUserProperty() {//выбор свойства (поля) аккаунта
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Логин учетной записи" << endl;
			cout << "2 - Пароль учетной записи" << endl;
			cout << "3 - ФИО учетной записи" << endl;
			cout << "4 - Адрес" << endl;
			cout << "5 - Номер телефона" << endl;
			cout << "6 - Назад" << endl;
			cin >> key;
			if (key < 49 || key > 54) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 54);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}
	int ChooseTestForFinish(vector <PassedTest> &resultsTest, string nameUser) {
		char key;
		string error = "";
		int indexBase;
		int index;
		vector<int>tempIndex;//запоминаем найденную позицию незавершеного теста в общей базе
		do {
			system("cls");
			index = 1;
			indexBase = 0;
			tempIndex.clear();
			if (error != "") {
				SetConsoleOutputCP(1251);
				cout << error << endl;
				SetConsoleOutputCP(866);
			}
			for (auto passTest : resultsTest) {
				if (nameUser == passTest.GetNameUser() && !passTest.GetPassedTest()) {
					cout << index++ << ". ";
					passTest.Print();
					tempIndex.push_back(indexBase);
				}
				indexBase++;
			}
			if (tempIndex.empty()) {
				return -1;//если незавершеных тестов нет - выводим позицию -1
			}
			cin >> key;
			if (key < 49 || key > index + 47) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > index + 47);
		return tempIndex.at(((int)key - 49));
	}
	void ListIsEmpty() {
		SetConsoleOutputCP(1251);
		cout << "Список пуст" << endl;
		cout << "Произведите любой ввод с клавиатуры для продолжения...";
		char temp;
		cin >> temp;
		SetConsoleOutputCP(866);
	}
	int ChooseAdminActionView() {
		char key;
		string error = "";
		SetConsoleOutputCP(1251);
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "1 - Просмотр результатов тестирования по категориям" << endl;
			cout << "2 - Просмотр результатов тестирования по конкретным тестам" << endl;
			cout << "3 - Просмотр результатов тестирования по по конкретным пользователям" << endl;
			cin >> key;
			if (key < 49 || key > 51) {
				error = "Ошибка, повторите ввод";
			}
		} while (key < 49 || key > 51);
		SetConsoleOutputCP(866);
		return (int)key - 48;
	}

};

class MainProgram :protected Menu
{
private:
	vector <User*> users;//база аккаунтов (массив)
	vector <Category> categorys;//база шаблонов тестов (массив)
	vector <PassedTest> resultsTest;//база пройденных тестов (массив)
	string pathUsers;
	string pathCategorys;
	string pathResultTest;
	int currentAccount;
	int currentCategory;
	int currentTest;
	int currentQuestion;
	Encryptor encryptor;
	string ReadStringFromFile(ifstream &fin) {//считывание из файла одной строки типа string
		string tempData;//переменная для строки
		char* tempStr = nullptr;//временная строка для хранения данных
		int tempSize = 0;//временная переменная для считывания из файла размера считываемой строки
		fin.read((char*)&tempSize, sizeof(int)); //читаем размер строки
		tempStr = new char[tempSize / sizeof(char) + 1];//выделяем память под запись из файла строки, +1 для символка окончания строки '\0'
		fin.read((char*)&*tempStr, tempSize);//читаем строку
		tempStr[tempSize / sizeof(char)] = '\0';//записываем в строку символ окончания строки (для отсечения мусора)
		tempData = tempStr;//записываем строку в переменную для хранения
		if (tempStr != nullptr) {//высвобождаем память временной строки
			delete[] tempStr;
			tempStr = nullptr;
		}
		return tempData;
	}
	void WriteStringToFile(ofstream& fout, string str) {
		int tempSize = 0;//переменная для хранения размера записываемой строки
		tempSize = str.length() * sizeof(char);//расчет размера записываемой строки
		fout.write((char*)&tempSize, sizeof(int));//запись в файл размера строки
		fout.write((char*)str.c_str(), tempSize);//запись в файл самой строки (string конвертируем в Char перед записью)
	}
	void SaveFileUser(ofstream& fout, User *user) {
		if (!fout.is_open()) {
			cout << "Error save user " << user->GetLogin() << " to archive" << endl;
		}
		else {
			WriteStringToFile(fout, encryptor.Encryption(user->GetLogin()));
			WriteStringToFile(fout, encryptor.Encryption(user->GetPassword()));
			WriteStringToFile(fout, user->GetName());
			WriteStringToFile(fout, user->GetAddress());
			WriteStringToFile(fout, user->GetTelephone());
		}
	}
	void SaveUsers() {
		if (!users.size()) {
			return;
		}
		_mkdir("data");//создание подпапки для хранения файлов (если не создана)
		ofstream fout;
		fout.open(pathUsers, ios::out | ios::binary);
		if (!fout.is_open()) {
			cout << "Error save users to archive" << endl;
		}
		else {
			int temp = users.size();
			fout.write((char*)&temp, sizeof(int));//запись в файл колличества пользователей
			bool tempTypeAccount = users.at(0)->GetTypeAccount();
			fout.write((char*)&tempTypeAccount, sizeof(bool));//запись в файл типа первого пользователя в базе (админ)
			for (int i = 0; i < users.size(); i++) {
				SaveFileUser(fout, users.at(i));
			}
		}
		fout.close();
	}
	void SaveTests(string path) {
		if (!categorys.size()) {
			return;
		}
		ofstream fout;
		fout.open(path, ios::out | ios::binary | ios::trunc);
		if (!fout.is_open()) {
			cout << "Error save tests to archive" << endl;
		}
		else {
			int sizeCategorys = categorys.size();
			int sizeTests;
			fout.write((char*)&sizeCategorys, sizeof(int));//запись в файл колличества категорий
			for (int index_categ = 0; index_categ < sizeCategorys; index_categ++) {
				WriteStringToFile(fout, categorys.at(index_categ).GetNameCategory());
				sizeTests = categorys.at(index_categ).GetTests().size();
				fout.write((char*)&sizeTests, sizeof(int));//запись в файл колличества тестов
				for (int index_test = 0; index_test < sizeTests; index_test++) {
					WriteStringToFile(fout, categorys.at(index_categ).GetTests().at(index_test).GetNameTest());
					categorys.at(index_categ).GetTests().at(index_test).SaveFileQuestions(fout);
				}
			}
		}
		fout.close();
	}
	void SavePassedTests(string path) {
		if (resultsTest.empty()) {
			return;
		}
		ofstream fout;
		fout.open(path, ios::out | ios::binary | ios::trunc);
		if (!fout.is_open()) {
			cout << "Error save result of tests to archive" << endl;
		}
		else {
			int sizeTests = resultsTest.size();
			int sizeCorrectAnswers;
			float percCorrectAnsw;
			int grade;
			int currentQuestion;
			bool passedTest;
			vector<int> answers;
			int sizeAnswers;
			int elemAnswer;
			fout.write((char*)&sizeTests, sizeof(int));//запись в файл колличества пройденых тестов
			for (int index_test = 0; index_test < sizeTests; index_test++) {
				WriteStringToFile(fout, resultsTest.at(index_test).GetNameCategory());
				WriteStringToFile(fout, resultsTest.at(index_test).GetNameUser());
				sizeCorrectAnswers = resultsTest.at(index_test).GetSizeCorrectAnswers();
				fout.write((char*)&sizeCorrectAnswers, sizeof(int));//запись в файл количества правильно отвеченых вопросов
				percCorrectAnsw = resultsTest.at(index_test).GetPercCorrectAnsw();
				fout.write((char*)&percCorrectAnsw, sizeof(float));//запись в файл процента правильно отвеченых вопросов
				grade = resultsTest.at(index_test).GetGrade();
				fout.write((char*)&grade, sizeof(int));//запись в файл оценки
				currentQuestion = resultsTest.at(index_test).GetCurrentQuestion();
				fout.write((char*)&currentQuestion, sizeof(int));//запись в файл позиции текущего вопроса
				passedTest = resultsTest.at(index_test).GetPassedTest();
				fout.write((char*)&passedTest, sizeof(bool));//запись в файл отсетки о завершении теста
				WriteStringToFile(fout, resultsTest.at(index_test).GetNameTest());
				answers = resultsTest.at(index_test).GetAnswers();
				sizeAnswers = answers.size();
				fout.write((char*)&sizeAnswers, sizeof(int));//запись в файл колличество уже отвеченых вопросов
				for (int index_answ = 0; index_answ < sizeAnswers; index_answ++) {
					elemAnswer = answers.at(index_answ);
					fout.write((char*)&elemAnswer, sizeof(int));//запись в файл ответа
				}
				resultsTest.at(index_test).SaveFileQuestions(fout);
			}
		}
		fout.close();
	}
	void LoadUsers() {//загрузка базы пользователей при первоначальном запуске программы
		ifstream fin;
		fin.open(pathUsers, ios::in | ios::binary);
		if (fin.is_open()) {
			int tempSize = 0;//временная переменная для считывания из файла колличества пользователей
			bool typeAccount = 0;//переменная для хранения типа аккаунта
			string tempLogin;//логин учетной записи
			string tempPassword;//пароль учетной записи
			string tempName;//ФИО учетной записи
			string tempAddress;//Адрес
			string tempTelephone;//номер телефона
			fin.read((char*)&tempSize, sizeof(int));//считываем количество записанных в файле учетных записей
			if (!tempSize) {
				return;//выходим из метода если файл пустой
			}
			users.reserve(tempSize);
			fin.read((char*)&typeAccount, sizeof(bool));//считываем тип аккаунта первой записи
			for (int i = 0; i < tempSize; i++) {
				tempLogin = encryptor.UnEncryption(ReadStringFromFile(fin));
				tempPassword = encryptor.UnEncryption(ReadStringFromFile(fin));
				tempName = ReadStringFromFile(fin);
				tempAddress = ReadStringFromFile(fin);
				tempTelephone = ReadStringFromFile(fin);
				if (typeAccount) {
					Admin* tempAccount = new Admin(tempTelephone, tempAddress, tempName, tempPassword, tempLogin, pathUsers);
					users.push_back(tempAccount);
					typeAccount = 0;
				}
				else {
					TestUser* tempAccount = new TestUser(tempTelephone, tempAddress, tempName, tempPassword, tempLogin, pathUsers);
					users.push_back(tempAccount);
				}
			}
		}
		fin.close();
	}
	void LoadTests(string path) {
		ifstream fin;
		fin.open(path, ios::in | ios::binary);
		if (fin.is_open()) {
			categorys.clear();
			int sizeCategorys;//колличество категорий
			int sizeTests;//колличество тестов
			string nameCategory;
			string nameTest;
			fin.read((char*)&sizeCategorys, sizeof(int));//считываем количество записанных в файле категорий
			if (!sizeCategorys) {
				return;//выходим из метода если файл пустой
			}
			categorys.reserve(sizeCategorys);
			for (int index_categ = 0; index_categ < sizeCategorys; index_categ++) {
				nameCategory = ReadStringFromFile(fin);
				Category temp(nameCategory, pathCategorys);
				categorys.push_back(temp);
				fin.read((char*)&sizeTests, sizeof(int));//считываем количество записанных в файле тестов
				for (int index_test = 0; index_test < sizeTests; index_test++) {
					nameTest = ReadStringFromFile(fin);
					Test temp(nameTest, nameCategory, pathCategorys);
					categorys.at(index_categ).GetTests().push_back(temp);
					categorys.at(index_categ).GetTests().at(index_test).LoadFileQuestions(fin);
				}
			}
		}
		fin.close();
	}
	void LoadPassedTests(string path) {
		ifstream fin;
		fin.open(path, ios::in | ios::binary);
		if (fin.is_open()) {
			int sizeTests;
			int sizeCorrectAnswers;
			float percCorrectAnsw;
			int grade;
			int currentQuestion;
			bool passedTest;
			string nameCategory;
			string nameUser;
			string nameTest;
			vector<int> answers;
			int sizeAnswers;
			int elemAnswer;
			fin.read((char*)&sizeTests, sizeof(int));//считываем количество записанных в файле пройденых тестов
			if (!sizeTests) {
				return;//выходим из метода если файл пустой
			}
			resultsTest.reserve(sizeTests);
			for (int index_test = 0; index_test < sizeTests; index_test++) {
				nameCategory = ReadStringFromFile(fin);
				nameUser = ReadStringFromFile(fin);
				fin.read((char*)&sizeCorrectAnswers, sizeof(int));//считываем c файла правильных ответов
				fin.read((char*)&percCorrectAnsw, sizeof(float));//считываем с файла процент правильных ответов
				fin.read((char*)&grade, sizeof(int));//считываем с файла оценку
				fin.read((char*)&currentQuestion, sizeof(int));//считываем с файла позицию текущего вопроса
				fin.read((char*)&passedTest, sizeof(bool));//считываем с файла метку завершения теста
				nameTest = ReadStringFromFile(fin);
				fin.read((char*)&sizeAnswers, sizeof(int));//считываем с файла колличество уже отвеченых вопросов
				for (int index_answ = 0; index_answ < sizeAnswers; index_answ++) {
					fin.read((char*)&elemAnswer, sizeof(int));//считывание с файла ответа
					answers.push_back(elemAnswer);
				}
				Test tempTest (nameTest, nameCategory, path);
				PassedTest tempPassedTest(tempTest, nameUser, sizeCorrectAnswers, percCorrectAnsw, grade, currentQuestion, passedTest, answers);
				resultsTest.push_back(tempPassedTest);
				resultsTest.at(index_test).LoadFileQuestions(fin);
			}
		}
		fin.close();
	}
	void DelUsers() {
		for (auto element : users) {
			delete element;
		}
	}
	bool CheckLogin(string newLogin) {//проверка уникальности логина новой учетной записи, 1 - есть повтор, 0 - уникальный
		for (auto user : users) {
			if (newLogin == user->GetLogin()) {
				return 1;
			}
		}
		return 0;
	}
	string EnterLogin() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempLogin;//логин учетной записи
		string error = "";
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "Введите логин" << endl;
			cin >> tempLogin;
			if (CheckLogin(tempLogin)) {
				error = "Такой логин уже существует";
			}
		} while (CheckLogin(tempLogin));
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempLogin;
	}
	string EnterPassword() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempPassword;//пароль учетной записи
		string tempPassword2;//пароль учетной записи (повторный ввод)
		string error = "";
		do {
			system("cls");
			if (error != "") {
				cout << error << endl;
			}
			cout << "Введите пароль" << endl;
			cin >> tempPassword;
			cout << "Введите повторно пароль" << endl;
			cin >> tempPassword2;
			if (tempPassword != tempPassword2) {
				error = "Ошибка ввода: пароли не одиноковые";
			}
		} while (tempPassword != tempPassword2);
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempPassword;
	}
	string EnterName() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempName;//ФИО учетной записи
		system("cls");
		cout << "Введите фамилию, имя, отчество" << endl;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		getline(cin, tempName);
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempName;
	}
	string EnterAddress() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempAddress;//Адрес
		system("cls");
		cout << "Введите адресс" << endl;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		getline(cin, tempAddress);
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempAddress;
	}
	string EnterTelephone() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempTelephone;//номер телефона
		system("cls");
		cout << "Введите номер телефона" << endl;
		cin >> tempTelephone;
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempTelephone;
	}
	string EnterCategory() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempNameCategory;//название категории
		system("cls");
		cout << "Введите название категории" << endl;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		getline(cin, tempNameCategory);
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempNameCategory;
	}
	string EnterTest() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempNameTest;//название теста
		system("cls");
		cout << "Введите название теста" << endl;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		getline(cin, tempNameTest);
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempNameTest;
	}
	string EnterQuestion() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempNameQuestion;//текст вопроса
		system("cls");
		cout << "Введите текст вопроса" << endl;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		getline(cin, tempNameQuestion);
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempNameQuestion;
	}
	string EnterPath() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string tempPath;//адресс к файлу
		system("cls");
		cout << "Введите путь к файлу" << endl;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		getline(cin, tempPath);
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempPath;
	}
	vector <string> EnterUnswersQuestion(string &nameQuestion) {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		vector <string> tempUnswerQuestions;//ответы на вопросы
		tempUnswerQuestions.reserve(4);
		system("cls");
		cout << "Введите ответы на вопрос \"" << nameQuestion << "\"" << endl;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		string temp;
		for (int i = 0; i < 4; i++) {
			getline(cin, temp);
			tempUnswerQuestions.push_back(temp);
		}
		SetConsoleOutputCP(866);
		SetConsoleCP(866);
		return tempUnswerQuestions;
	}
	vector<bool> EnterCorrectUnswersQuestion(string& nameQuestion, vector <string> &unswersQuestion) {
		SetConsoleOutputCP(1251);
		vector <bool> tempUnswerQuestions;//правильные ответы на вопросы: 1 - верно, 0 - неверно
		tempUnswerQuestions.reserve(4);
		char temp;
		system("cls");
		cout << "Введите правильные ответы (1 - верно, 0 - неверно) на вопрос: \"" << nameQuestion << "\"" << endl;
		for (int i = 0; i < 4; i++) {
			cout << unswersQuestion[i] << " ";
			cin >> temp;
			if (cin.peek() == '\n') {
				cin.ignore();
			}
			tempUnswerQuestions.push_back((int)temp - 48);
		}
		SetConsoleOutputCP(866);
		return tempUnswerQuestions;
	}
	void CreateAccount() {//создание нового аккаунта
		if (users.size() == 0) {
			Admin* tempAccount = new Admin(EnterTelephone(), EnterAddress(), EnterName(), EnterPassword(), EnterLogin(), pathUsers);
			users.push_back(tempAccount);
		}
		else {
			TestUser* tempAccount = new TestUser(EnterTelephone(), EnterAddress(), EnterName(), EnterPassword(), EnterLogin(), pathUsers);
			users.push_back(tempAccount);
		}
	}
	int EnterAccount() {
		SetConsoleOutputCP(1251);
		string login;//логин учетной записи
		string password;//пароль учетной записи
		int index = 0;//номер кчетной записи
		string error = "";
		string error2 = "";
		bool re_enter = 0;//повторный ввод 1 - да, 0 - нет (выход)
		do {
			system("cls");
			error = "";
			cout << "Введите логин" << endl;
			cin >> login;
			cout << "Введите пароль" << endl;
			cin >> password;
			index = 0;
			for (auto user : users) {
				if (login == user->GetLogin()) {
					if (password == user->GetPassword()) {
						re_enter = 0;
					}
					else {
						error = "Неверный пароль";
					}
					break;
				}
				index++;
			}
			if (!CheckLogin(login)) {
				error = "Неверный логин";
			}
			if (error != "") {//если неверный ввод запрашиваем дальнейшие действия
				error2 = "";
				char key;
				do {
					system("cls");
					if (error2 != "") {
						cout << error2 << endl;
					}
					cout << error << endl;
					cout << "1 - Повторить ввод" << endl;
					cout << "2 - Выйти из программы" << endl;
					cin >> key;
					if (key < 49 || key > 50) {
						error2 = "Ошибка, повторите ввод";
					}
				} while (key < 49 || key > 50);
				if (key == '1') {
					re_enter = 1;
				}
				else {
					exit(0);
				}
			}
		} while (re_enter);
		SetConsoleOutputCP(866);
		return index;
	}
	void UserModification() {
		int targetUserPosition = ChooseUser(users);
		int targetUserProperty;
		do {
			targetUserProperty = ChooseUserProperty();
			switch (targetUserProperty)
			{
			case 1:
				users.at(currentAccount)->SetLogin(users.at(targetUserPosition), EnterLogin());
				break;
			case 2:
				users.at(currentAccount)->SetPassword(users.at(targetUserPosition), EnterPassword());
				break;
			case 3:
				users.at(currentAccount)->SetName(users.at(targetUserPosition), EnterName());
				break;
			case 4:
				users.at(currentAccount)->SetAddress(users.at(targetUserPosition), EnterAddress());
				break;
			case 5:
				users.at(currentAccount)->SetTelephone(users.at(targetUserPosition), EnterTelephone());
				break;
			}
		} while (targetUserProperty != 6);
	}
	void PassTest(vector <PassedTest> &resultsTest, Test test, string nameUser) {
		PassedTest newPassedTest(test, nameUser);
		newPassedTest.PassTest();
		resultsTest.push_back(newPassedTest);
	}
	

public:
	MainProgram() : pathUsers("data\\users.txt"), pathCategorys("data\\categorys.txt"), pathResultTest("data\\result_test.txt"), encryptor(5), currentAccount (-1) {
		LoadUsers();
		LoadTests(pathCategorys);
		LoadPassedTests(pathResultTest);
	}
	~MainProgram() {
		SaveUsers();
		SaveTests(pathCategorys);
		SavePassedTests(pathResultTest);
		DelUsers();
	}
	void AddUser() {
		CreateAccount();
	}
	void PrintUsers() {
		for (auto element : users) {
			element->Print();
		}
	}
	void GetCountUsers() {
		cout << User::GetCountAccounts() << endl;
	}
	void startProgram() {
		switch (InitialLogin(users.size()))
		{
		case 1://создание аккаунта
			CreateAccount();
			currentAccount = users.size() - 1;
			break;
		case 2://вход в аккаунт
			currentAccount = EnterAccount();
			break;
		case 3://выход
			this->~MainProgram();
			exit(0);
		}
		do {
			if (users.at(currentAccount)->GetTypeAccount()) {//если админ
				switch (AdminChoosAction())
				{
				case 1://Управление пользователями
					switch (UserManagement())
					{
					case 1://Создание пользователя
						CreateAccount();
						break;
					case 2://Удаление пользователя
						users.at(currentAccount)->DelUser(users, users.begin() + ChooseUser(users));
						break;
					case 3://Модификация пользователя
						UserModification();
						break;
					case 4://выход
						this->~MainProgram();
						exit(0);
						break;
					}
					break;
				case 2://Просмотр статистики
					switch (ChooseAdminActionView())
					{
					case 1://Просмотр результатов тестирования по категориям
						users.at(currentAccount)->ShowPassedTestCategory(resultsTest, categorys.at(ChooseCategory(categorys)).GetNameCategory());
						break;
					case 2://Просмотр результатов тестирования по конкретным тестам
						currentCategory = ChooseCategory(categorys);
						currentTest = ChooseTest(categorys.at(currentCategory).GetTests());
						users.at(currentAccount)->ShowPassedTestTest(resultsTest, categorys.at(currentCategory).GetTest(currentTest).GetNameTest());
						break;
					case 3://Просмотр результатов тестирования по по конкретным пользователям
						users.at(currentAccount)->ShowPassedTestName(resultsTest, users.at(ChooseUser(users))->GetName());
						break;
					}
					break;
				case 3://Управление тестированием
					switch (ChooseAdminActionCategory())
					{
					case 1://Выбор категории
						currentCategory = ChooseCategory(categorys);
						if (currentCategory == -1) {
							ListIsEmpty();
							return;
						}
						switch (ChooseAdminActionTest())
						{
						case 1://Выбор теста
							currentTest = ChooseTest(categorys.at(currentCategory).GetTests());
							if (currentTest == -1) {
								ListIsEmpty();
								return;
							}
							switch (ChooseAdminActionQuestion())
							{
							case 1://Выбор вопроса для определения правильных/неправильных ответов
								users.at(currentAccount)->ChooseQuestion_SetCorrectUnswers(categorys.at(currentCategory).GetTest(currentTest));
								break;
							case 2://Добавление вопроса
								string tempNameQuestion = EnterQuestion();//текст вопроса
								vector <string> unswers = EnterUnswersQuestion(tempNameQuestion);//варианты ответов
								vector <bool> correctUnswers = EnterCorrectUnswersQuestion(tempNameQuestion, unswers);//метки правильных ответов
								users.at(currentAccount)->AddQuestion(categorys.at(currentCategory).GetTest(currentTest), tempNameQuestion, unswers, correctUnswers);
								break;
							}
							break;
						case 2://Создание теста
							users.at(currentAccount)->AddTest(categorys.at(currentCategory).GetTests(), EnterTest(), categorys.at(currentCategory).GetNameCategory(), pathCategorys);
							break;
						}
						break;
					case 2://Создание категории
						users.at(currentAccount)->AddCategory(categorys, EnterCategory(), pathCategorys);
						break;
					case 3://Сохранение тестов в файл
						if (users.at(currentAccount)->GetTypeAccount()) {
							SaveTests(EnterPath());
						}
						break;
					case 4://Загрузка тестов из файла
						if (users.at(currentAccount)->GetTypeAccount()) {
							LoadTests(EnterPath());
						}
						break;
					}
					break;
				case 4://выход
					this->~MainProgram();
					exit(0);
					break;
				}
			}
			else {//если тестируемый пользователь
				switch (TestUserChoosAction())
				{
				case 1://Просмотреть свои предыдущие результаты тестирования
					users.at(currentAccount)->ShowPassedTestName(resultsTest, users.at(currentAccount)->GetName());
					break;
				case 2://Пройти новое тестирование
					currentCategory = ChooseCategory(categorys);
					currentTest = ChooseTest(categorys.at(currentCategory).GetTests());
					PassTest(resultsTest, categorys.at(currentCategory).GetTest(currentTest), users.at(currentAccount)->GetName());
					break;
				case 3://Продолжить старое тестирование
					currentTest = ChooseTestForFinish(resultsTest, users.at(currentAccount)->GetName());
					if (currentTest == -1) {
						ListIsEmpty();
					}
					else {
						resultsTest.at(currentTest).PassTest();
					}
					break;
				case 4://выход
					this->~MainProgram();
					exit(0);
					break;
				}
			}
		} while (1);
	}
};


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //для нескольких точек выхода
	MainProgram prg;
	prg.startProgram();
	//prg.PrintUsers();
}