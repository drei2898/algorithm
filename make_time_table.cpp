#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>

using namespace std;

class Student {
public:
	int TimeTable[5][30];
	Student();
	void makeTimeTable(int day, float start, float end, int Priority);
	void showTimeTable();
};

Student::Student() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 30; j++) TimeTable[i][j] = 0;
	}
}

void Student::makeTimeTable(int day, float start, float end, int Priority) {
	int s = (start - 1) / 0.5;
	int e = (end - 1) / 0.5;

	for (int i = s; i <= e; i++) TimeTable[day][i] = Priority;

}

void Student::showTimeTable() {

	cout << setw(7) << " ";
	
	for (int i = 0; i < 5; i++) cout << i << " ";

	cout << endl;

	for (int j = 0; j < 30; j++) {
		cout << setw(4) << j * 0.5 + 1 << " : ";
		for (int i = 0; i < 5; i++) cout << TimeTable[i][j] << " ";
		cout << endl;
	}
}


class CheckCollision {
private:
	vector<Student> S;
	int TimeTable[5][30];
public:
	CheckCollision();
	void addStudent();
	void makeTimeTable(Student s);
	friend Student;
};

CheckCollision::CheckCollision() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 30; j++) TimeTable[i][j] = 0;
	}
}

void CheckCollision::makeTimeTable(Student s) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 42; j++) TimeTable[i][j] &= s.TimeTable[i][j];
	}
}

int main() {
	ifstream fin;
	fin.open("data/data1.txt");
	
	string temp;
	Student s;
	int day, Priority;
	float start, end;
	int a,b;
	while (!fin.eof()) {
		fin >> temp;
		day = stoi(temp);
		a = temp.find(",", 2);
		start = stod(temp.substr(2, a-2));
		end = stod(temp.substr(a+1, temp.length()+1));
		b = temp.find(",", a+1);
		Priority = stoi(temp.substr(b+1, temp.length() + 1));
		s.makeTimeTable(day, start, end, Priority);
	}

	s.showTimeTable();

	fin.close();
	return 0;
}