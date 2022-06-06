#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>
#include<bitset>
#include "stime.h"

constexpr int N_TIMES = 30; // �ð� ���� 0 ~ 15.5 �� �� 30���� ���ڸ� ������.
constexpr int DOW = 5; // ���� ��
constexpr int INTERVAL =  10; // ���� �����ϴ� ����

using namespace std;

class Student {
public:
	int TimeTable[DOW][N_TIMES];
	Student();
	void makeTimeTable(int day, float start, float end, int Priority);
	void showTimeTable();
};

Student::Student() {
	for (int i = 0; i < DOW; i++) {
		for (int j = 0; j < N_TIMES; j++) TimeTable[i][j] = 0;
	}
}

void Student::makeTimeTable(int day, float start, float end, int Priority) {
	int s = static_cast<int>((start - 1) * 2);
	int e = static_cast<int>((end - 1)* 2);

	for (int i = s; i <= e; i++) TimeTable[day][i] = Priority;

}

void Student::showTimeTable() {

	cout << setw(7) << " ";
	
	for (int i = 0; i < DOW; i++) cout << i << " ";

	cout << endl;

	for (int j = 0; j < N_TIMES; j++) {
		cout << setw(4) << static_cast<float>(j) / 2.0 + 1.0 << " : ";
		for (int i = 0; i < DOW; i++) cout << TimeTable[i][j] << " ";
		cout << endl;
	}
}


class CheckCollision {
protected:
	vector<Student> S;
	int TimeTable[DOW][N_TIMES];
	int weightTable[DOW][N_TIMES - INTERVAL + 1] = {0};

public:
	CheckCollision();
	//void addStudent(string filename);
	void showStudent();
	void makeTimeTable(Student s);
	int makeWeightTable();
	friend Student;
public:
	virtual void addStudent(string filename); 
};

void CheckCollision::addStudent(string filename) {
	ifstream fin(filename);

	string temp;
	Student s;
	int day, Priority;
	float start, end;
	int a, b;
	while (!fin.eof()) {
		fin >> temp;
		day = stoi(temp);
		a = temp.find(",", 2);
		start = stof(temp.substr(2, a - 2));
		end = stof(temp.substr(a + 1, temp.length() + 1));
		b = temp.find(",", a + 1);
		Priority = stoi(temp.substr(b + 1, temp.length() + 1));
		s.makeTimeTable(day, start, end, Priority);
	}
	S.push_back(s);
	fin.close();
}

/*
����ġ�� ���� ������ ���̺��� �����.
���ī�� �˰���ó�� ���� ���� ���� ( mod ������ ���� )
*/
int CheckCollision::makeWeightTable() {
	int min = std::numeric_limits<int>::max(); // �ּҰ�

	constexpr int last = N_TIMES - INTERVAL;
	for (int d = 0; d < DOW; d++)
	{
		int weight = 0; // ���Ǵ� ����ġ

		for (int w = 0; w < INTERVAL; w++) // ����ġ ���� ���� ����
		{
			weight += TimeTable[d][w]; // ������ ���� ���̺�.
		}


		for (int i = 0; i < last + 1; i++)
		{	//������ζ�� INTERVAL ��ŭ ���� �ʿ�������, ���⼭�� 2�� ���길 ����.
			if (weight < min)
			{
				min = weight;
			}

			weightTable[d][i] = weight;
			if (i < last) // �ε����� ��� ���� ���� ���� �� 
			{
				weight -= TimeTable[d][i]; // �ּ� �ε��� �� ����
				weight += TimeTable[d][i + INTERVAL]; // �ִ� �ε��� �� ���ϱ�
			}
		}
	}

	return min;
}

CheckCollision::CheckCollision() {
	for (int i = 0; i < DOW; i++) {
		for (int j = 0; j < N_TIMES; j++) TimeTable[i][j] = 0;
	}
}

void CheckCollision::showStudent() {
	for (int i = 0; i < S.size(); i++) {
		cout << endl << i + 1 << "��° �л��� �ð�ǥ : \n";
		S[i].showTimeTable();
	}
}

void CheckCollision::makeTimeTable(Student s) {
	for (int i = 0; i < DOW; i++) {
		for (int j = 0; j < N_TIMES; j++) TimeTable[i][j] += s.TimeTable[i][j];
	}
}


class CheckCollisionWithBit : public CheckCollision {
private:
	std::bitset<N_TIMES> bits[DOW]; // ���� ó���� ���� ��Ʈ����
	std::bitset<N_TIMES> mask; // ���� �ð��� �˻��ϴ� ��Ʈ��.
	// INTERVAL ��ŭ 1�� ������.
	std::vector<STime> available_times; // ���õ� ���� �ð���


	void setBit(int dow, float start, float end) {
		int s_int = static_cast<int>((start - 1) * 2);
		int s_end = static_cast<int>((end - 1) * 2);

		for (int v = s_int; v <= s_end; v++)
		{
			bits[dow].set(v,true); // v��° ��Ʈ�� true�� �����Ѵ�.
		}
	}
public:
	CheckCollisionWithBit()
		: CheckCollision()
	{
		for (int i = 0; i < DOW; i++)
		{
			bits[i].reset(); // ��ü ��Ʈ�� 0���� �ʱ�ȭ.
		}

		for (int i = 0; i < INTERVAL; i++)
		{
			mask.set(i); // INTERVAL ��ŭ�� ��Ʈ�� 1�� �ʱ�ȭ.
		}
	}

	void addStudent(string filename) override {
		ifstream fin(filename);

		string temp;
		Student s;
		int day, Priority;
		float start, end;
		int a, b;
		while (!fin.eof()) {
			fin >> temp;
			day = stoi(temp);
			a = temp.find(",", 2);
			start = stof(temp.substr(2, a - 2));
			end = stof(temp.substr(a + 1, temp.length() + 1));
			b = temp.find(",", a + 1);
			Priority = stoi(temp.substr(b + 1, temp.length() + 1));
			s.makeTimeTable(day, start, end, Priority);
			setBit(day, start, end); // ��Ʈ ���� �κ��� �ٸ�
		}
		S.push_back(s);
		fin.close();
	}
	/*
	* ��Ʈ ����ũ�� �� ��Ʈ���� & �����Ͽ� ���� ���� �ð��� �����ϴ��� �˻��Ѵ�.
	* �����ϸ� true, �������� ������ false�� ��ȯ�Ѵ�.
	*/
	bool checkBit() {
		// ��Ʈ�� �˻��Ͽ� ������ ���� �����ϴ��� �˾ƺ���.
		for( int d = 0; d < DOW; d++) // ���� ���� �˻�
			{
			for (int i = 0; i < N_TIMES - INTERVAL + 1; i++) // �� �ð��� �˻�
			{
				auto result = bits[d] & (mask << i); // ��Ʈ �˻�. 
				// ���� �˻��ϴ� INTERVAL ��ġ�� & ������ ����� 0 �� �ȴٴ� ����
				// �ش� �ð��뿡 ������ �ִ� ����� ���ٴ� �ǹ̰� �ȴ�. 
				// �� ��� �ش� �ð���� ���� �ð��� �ǹǷ� �� �̻� ������ �ʿ䰡 ����.
				// �ƴ� ���, 0���� ó���� �κп� ���ؼ��� �������� �ʵ��� ó���Ѵ�.

				if (result.none()) // ��� 0�̶�� == �ش� �ð��밡 ��� ������� ��ȿ�ϴٸ�
				{ // ���� �ð��븦 available_times �� �����Ѵ�.
					float start = static_cast<float>(0.5 * i + 1.0);
					float end = static_cast<float>(0.5 * (i + INTERVAL - 1) + 1.0);
					STime t(d, start, end);
					available_times.push_back(t);
				}
			}
		}
		return !available_times.empty(); // �ϳ��� �����ϸ� �̹� ���� �ð��� ã�� ���.
	}

	void addStudents(vector<string> filenames) {
		for (const auto& filename : filenames) {
			addStudent(filename);
		}
	}

	/*
	checkBit�� ���� �����ð��� �߰��� ���, �ٷ� ��ȯ�Ѵ�.

	*/
	std::vector<STime> getOptimalTimes() {
		//for(const auto & )
		bool condition = checkBit(); // ���� �ð��� �����ϴ��� ���� �˻�.
		if (condition) { // ���� �ð�(interval ��ŭ ��� �ð�) �����ϸ�
			cout << "������ ã��!" << endl;
			return available_times; // ���� �ð� ��ȯ
		}

		for (int d = 0; d < DOW; d++) // ���� ��
		{
			for (int i = 0; i < N_TIMES; i++) // �� �ð��뺰�� ���ϱ�
			{
				if (bits[d][i]) // �ش� �ð��뿡 ������ �ʿ��� ��츸 ���Ѵ�.
				{
					for (const auto& student : S)
					{
						TimeTable[d][i] += student.TimeTable[d][i];
					}
				}
			}
		}
		int min  = makeWeightTable(); // ����ġ ���̺� ����

		//for (const auto& t : weightTable)
		//{
		//	cout << 
		//}

		for (int d = 0; d < DOW; d++)
		{
			for (int i = 0; i < N_TIMES - INTERVAL + 1; i++)
			{
				if (weightTable[d][i] == min) // ���õ� �ּ� �ð����� ���
				{
					float start = static_cast<float>(0.5 * i + 1.0);
					float end = static_cast<float>(0.5 * (i + INTERVAL - 1) + 1.0);
					STime s(d, start, end);
					available_times.push_back(s);
				}
			}
		}
		return available_times;
	}
};


int main() {
	CheckCollisionWithBit c;

	std::string num;
	string filename = "data";
	for (int i = 1; i <= 11; i++) {
		num = std::to_string(i);
		c.addStudent(filename + num + ".txt");
	}

	c.showStudent();
	auto times = c.getOptimalTimes();
	cout << times.size() << endl;
	for (const auto& t : times)
	{
		cout << t.dow << " " << t.start << " " << t.stop << endl;
	}
}