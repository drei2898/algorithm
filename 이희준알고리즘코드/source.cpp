#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>
#include<bitset>
#include "stime.h"

constexpr int N_TIMES = 30; // 시간 개수 0 ~ 15.5 면 총 30개의 숫자를 가진다.
constexpr int DOW = 5; // 요일 수
constexpr int INTERVAL =  10; // 점수 측정하는 간격

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
가중치를 더해 구성한 테이블을 만든다.
라빈카프 알고리즘처럼 덧셈 연산 수행 ( mod 연산은 안함 )
*/
int CheckCollision::makeWeightTable() {
	int min = std::numeric_limits<int>::max(); // 최소값

	constexpr int last = N_TIMES - INTERVAL;
	for (int d = 0; d < DOW; d++)
	{
		int weight = 0; // 계산되는 가중치

		for (int w = 0; w < INTERVAL; w++) // 가중치 덧셈 연산 수행
		{
			weight += TimeTable[d][w]; // 가중합 계산된 테이블.
		}


		for (int i = 0; i < last + 1; i++)
		{	//원래대로라면 INTERVAL 만큼 덧셈 필요하지만, 여기서는 2번 연산만 수행.
			if (weight < min)
			{
				min = weight;
			}

			weightTable[d][i] = weight;
			if (i < last) // 인덱스가 계산 범위 내에 있을 때 
			{
				weight -= TimeTable[d][i]; // 최소 인덱스 값 빼기
				weight += TimeTable[d][i + INTERVAL]; // 최대 인덱스 값 더하기
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
		cout << endl << i + 1 << "번째 학생의 시간표 : \n";
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
	std::bitset<N_TIMES> bits[DOW]; // 숫자 처리를 위한 비트열들
	std::bitset<N_TIMES> mask; // 최적 시간을 검사하는 비트열.
	// INTERVAL 만큼 1을 가진다.
	std::vector<STime> available_times; // 선택된 최적 시간들


	void setBit(int dow, float start, float end) {
		int s_int = static_cast<int>((start - 1) * 2);
		int s_end = static_cast<int>((end - 1) * 2);

		for (int v = s_int; v <= s_end; v++)
		{
			bits[dow].set(v,true); // v번째 비트를 true로 설정한다.
		}
	}
public:
	CheckCollisionWithBit()
		: CheckCollision()
	{
		for (int i = 0; i < DOW; i++)
		{
			bits[i].reset(); // 전체 비트를 0으로 초기화.
		}

		for (int i = 0; i < INTERVAL; i++)
		{
			mask.set(i); // INTERVAL 만큼의 비트를 1로 초기화.
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
			setBit(day, start, end); // 비트 설정 부분이 다름
		}
		S.push_back(s);
		fin.close();
	}
	/*
	* 비트 마스크와 각 비트열을 & 연산하여 현재 자유 시간이 존재하는지 검사한다.
	* 존재하면 true, 존재하지 않으면 false을 반환한다.
	*/
	bool checkBit() {
		// 비트를 검사하여 최적의 값이 존재하는지 알아본다.
		for( int d = 0; d < DOW; d++) // 요일 별로 검사
			{
			for (int i = 0; i < N_TIMES - INTERVAL + 1; i++) // 각 시간대 검사
			{
				auto result = bits[d] & (mask << i); // 비트 검사. 
				// 현재 검사하는 INTERVAL 위치를 & 연산한 결과가 0 이 된다는 것은
				// 해당 시간대에 업무가 있는 사람이 없다는 의미가 된다. 
				// 이 경우 해당 시간대는 최적 시간이 되므로 더 이상 연산할 필요가 없다.
				// 아닌 경우, 0으로 처리된 부분에 대해서는 연산하지 않도록 처리한다.

				if (result.none()) // 모두 0이라면 == 해당 시간대가 모든 사람에게 유효하다면
				{ // 최적 시간대를 available_times 에 삽입한다.
					float start = static_cast<float>(0.5 * i + 1.0);
					float end = static_cast<float>(0.5 * (i + INTERVAL - 1) + 1.0);
					STime t(d, start, end);
					available_times.push_back(t);
				}
			}
		}
		return !available_times.empty(); // 하나라도 존재하면 이미 최적 시간을 찾은 경우.
	}

	void addStudents(vector<string> filenames) {
		for (const auto& filename : filenames) {
			addStudent(filename);
		}
	}

	/*
	checkBit을 통해 최적시간을 발견한 경우, 바로 반환한다.

	*/
	std::vector<STime> getOptimalTimes() {
		//for(const auto & )
		bool condition = checkBit(); // 최적 시간이 존재하는지 먼저 검사.
		if (condition) { // 최적 시간(interval 만큼 비는 시간) 존재하면
			cout << "최적값 찾음!" << endl;
			return available_times; // 가능 시간 반환
		}

		for (int d = 0; d < DOW; d++) // 요일 별
		{
			for (int i = 0; i < N_TIMES; i++) // 각 시간대별로 더하기
			{
				if (bits[d][i]) // 해당 시간대에 연산이 필요한 경우만 더한다.
				{
					for (const auto& student : S)
					{
						TimeTable[d][i] += student.TimeTable[d][i];
					}
				}
			}
		}
		int min  = makeWeightTable(); // 가중치 테이블 생성

		//for (const auto& t : weightTable)
		//{
		//	cout << 
		//}

		for (int d = 0; d < DOW; d++)
		{
			for (int i = 0; i < N_TIMES - INTERVAL + 1; i++)
			{
				if (weightTable[d][i] == min) // 선택된 최소 시간대인 경우
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