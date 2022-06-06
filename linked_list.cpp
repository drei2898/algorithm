#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>
#include "stime.h"

constexpr int N_TIMES = 30; // 시간 개수 0 ~ 15.5 면 총 30개의 숫자를 가진다.
constexpr int DOW = 5; // 요일 수
constexpr int INTERVAL =  10; // 점수 측정하는 간격
constexpr int MAX = 9999999;

using namespace std;

//링크리스트의 노드입니다.
class linked_node
{
    public:
    int start, end, priority; //시작 시간, 끝 시간, 우선순위
    linked_node* next = nullptr; // 다음 노드의 주소가 저장될 linked_node 포인터 변수

	// start, end에 저장될 때 그대로 저장 되는 것이 아니라 정수 형태로 가공한다.
    linked_node(float st, float en, int pri)
    {
        start = (st - 1) / 0.5;
        end = (en - 1) / 0.5;
        priority = pri;
    }

	// 생성자 노드이다.
	// 각 학생이 가지는 시간표를 링크 리스트를 만들 때 각 요일마다 리스트를 하나씩 가진다.
	// 각 요일의 처음 head에는 의미 없는 값이 들어가게 된다.
	linked_node()
	{
		start = -1;
		end = -1;
		priority = -1;
	}
};

//링크리스트입니다.
class TimeTable
{
private:
	//요일의 링크 리스트에 노드를 넣기 위한 변수
	linked_node* end;
	//요일마다 링크 리스트르 만들었습니다
	linked_node linked_vector[DOW];
	//이전의 링크리스트에 넣고 있던 요일과 다른지 비교하기 위한 변수
	int previous_day = -1;
public:
	void makeTimeTable(int day, float st, float en, int pri);
	void addWeight(unsigned int (*weightTable)[N_TIMES]);
	void set_end(int i);
	linked_node* TimeTable::get_end(int i);
};

void TimeTable::makeTimeTable(int day, float st, float en, int pri)
{
	//요일, 시작 시간, 끝 시간, 우선순위를 입력받는다.
	//받은 정보로 노드를 만들어서 링크 리스트의 끝에 넣는다.
	linked_node *cur = new linked_node(st, en, pri);
	if(previous_day != day)
	{
		end = &linked_vector[day];
		previous_day = day;
	}
	end -> next = cur;
	end = end -> next;
}

void TimeTable::addWeight(unsigned int (*weightTable)[N_TIMES])
{
	//해당 시간표의 우선도를 weightTable이차원 배열에 더한다.
	for(int i = 0; i < DOW; i++)
	{
		linked_node *cur_list = linked_vector[i].next;
		while (cur_list)
		{
			for (int j = cur_list->start; j <= cur_list->end; j++)
				weightTable[i][j] += cur_list->priority;
			cur_list = cur_list->next;
		}
	}
}

void TimeTable::set_end(int i)
{
	end = linked_vector[i].next;
}

linked_node* TimeTable::get_end(int i)
{
	return end;
}

class CheckCollision
{
private:
	vector<TimeTable> students;
	vector<STime> available_times;
	vector<linked_node*> students_head;
	unsigned int weightTable[DOW][N_TIMES - INTERVAL + 1] = {0};
public:
	void addstudent(string filename);
	int makeWeightTable();
	vector<STime> getOptimalTimes();
	void set_students_end(int cur_DOW);
};

void CheckCollision::set_students_end(int cur_DOW)
{
	for(int i = 0; i < students.size(); i++)
		students[i].set_end(cur_DOW)
}

void CheckCollision::addstudent(string filename)
{
	//학생마다 시간표의 정보가 있는 링크리스트를 만든다.
	ifstream fin(filename);

	TimeTable ll;
	string temp;
	int day, Priority;
	float start, end;
	int a,b;
	while (!fin.eof())
	{
		fin >> temp;
		day = stoi(temp);
		a = temp.find(",", 2);
		start = stod(temp.substr(2, a-2));
		end = stod(temp.substr(a+1, temp.length()+1));
		b = temp.find(",", a+1);
		Priority = stoi(temp.substr(b+1, temp.length() + 1));
		cout << day << " " << start << " "<< end << " " << Priority << endl;
		ll.makeTimeTable(day, start, end, Priority);
	}
	students.push_back(ll);
}

int CheckCollision::makeWeightTable()
{
	//먼저 단순히 모든 요일의 시간대마다 가중치를 더한 표를 만든다.
	//그리고 INTERVAL값 만큼의 범위 가중치를 새로운 배열에 넣는다.
	//개로운 배열에 넣을 때 첫 원소값을 빼고 다음에 올 원소 값을 더한다.
	//res에는 최소 가중치를 넣는다.
	int res = MAX;
	for(int i = 0; i < DOW; i++)
	{
		set_students_end(i);
	}
	return res;
}

vector<STime> CheckCollision::getOptimalTimes()
{
	//min에 최소 가중치가 저장된다.
	//available_times에 min의 가중치와 같은 요일과 시간이 저장된다.
	int min = makeWeightTable();
	for (int i = 0; i < DOW; i ++)
	{
		for (int j = 0; j < N_TIMES - INTERVAL + 1; j++)
		{
			if (weightTable[i][j] == min)
			{
				float start = static_cast<float>(0.5 * j + 1.0);
				float end = static_cast<float>(0.5 * (j + INTERVAL - 1) + 1.0);
				STime t(i, start, end);
				available_times.push_back(t);
			}
		}
	}
	return 	available_times;
}

int main()
{
	CheckCollision c;
	std::string num;
	string filename = "data";
	for (int i = 1; i <= 11; i++) {
		num = std::to_string(i);
		c.addstudent(filename + num + ".txt");
	}
}