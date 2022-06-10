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

unsigned int compare = 0;
unsigned int mathOp = 0;
unsigned int spaceSize = 0;

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
	void set_next_node();
	linked_node* get_end();
};

void TimeTable::set_next_node()
{
	end = end->next;
}

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
			{
				weightTable[i][j] += cur_list->priority;
				mathOp += 1;
			}
			cur_list = cur_list->next;
		}
	}
}

void TimeTable::set_end(int i)
{
	end = linked_vector[i].next;
}

linked_node* TimeTable::get_end()
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
	void getOptimalTimes();
	void set_students_end(int cur_DOW);
	void show_available_times();
};

void CheckCollision::show_available_times()
{
	for(int i = 0; i < available_times.size(); i++)
	{
		switch (available_times[i].dow)
		{
		case 0:
			cout << "월요일 ";
			break;
		case 1:
			cout << "화요일 ";
			break;
		case 2:
			cout << "수요일 ";
			break;
		case 3:
			cout << "목요일 ";
			break;
		case 4:
			cout << "금요일 ";
			break;
		default:
			break;
		}
		cout << available_times[i].start << " ~ " << available_times[i].stop << endl;
	}
}

//학생들의 end변수를 head를 제외한 해당 요일의 가장 앞 노드를 가르키게 한다.
void CheckCollision::set_students_end(int cur_DOW)
{
	for(int i = 0; i < students.size(); i++)
		students[i].set_end(cur_DOW);
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
	int i = 0;
	while (!fin.eof())
	{
		fin >> temp;
		day = stoi(temp);
		a = temp.find(",", 2);
		start = stod(temp.substr(2, a-2));
		end = stod(temp.substr(a+1, temp.length()+1));
		b = temp.find(",", a+1);
		Priority = stoi(temp.substr(b+1, temp.length() + 1));
		cout << i << "번째" << endl;
		cout << day << " " << start << " "<< end << " " << Priority << endl;
		ll.makeTimeTable(day, start, end, Priority);
		i++;
	}
	students.push_back(ll);
}

int CheckCollision::makeWeightTable()
{
	//요일마다 INTERVAL를 적용한 가중치 배열을 만든다.
	int res = MAX;
	for(int i = 0; i < DOW; i++)
	{
		set_students_end(i);//학생들의 end변수를 요일의 가장 앞 노드로 지정한다.
		for(int j = 0; j < N_TIMES - INTERVAL + 1; j++)//weightTable의 열 만큼 이동한다.
		{
			//현재 보고 있는 시간대에 해당하는 가중치를 더한다.
			for(int k = 0; k < students.size(); k++)
			{
				//현재 보고 있는 학생의 end가 null이라면 해당 요일의 시간표가 비어있거나 끝났다는 뜻이기 떄문에 넘어간다.
				//시간표의 끝시간이 시간대의 끝시간보다 크다는 것은 지금 시간대가 아니기 때문에 넘어간다.
				if(!students[k].get_end())
				{
					compare += 1;
					continue;
				}
				else if(students[k].get_end()->start > j + INTERVAL - 1)
				{
					compare += 2;
					continue;
				}
				//시간표의 시작 시간이 시간대의 시작 시간보다 작은 경우
				//뒤에 알맞은 시간표의 시간대가 있을 수 있기 떄문에 end를 다음 노드로 이동시키고 다시 비교
				//다시 비교했을 때 현재 노드가 null이 아니고 시간표의 시작시간이 시간대의 끝시간대보다 작거나 같은 경우 가중치를 더함
				else if(students[k].get_end()->end < j)
				{
					compare += 5;
					students[k].set_next_node();
					if(students[k].get_end() && students[k].get_end()->start <= j + INTERVAL - 1)
					{
						mathOp += 1;
						weightTable[i][j] += students[k].get_end()->priority;
					}
				}
				//그외의 경우 가중치를 더함
				else
				{
					compare += 5;
					weightTable[i][j] += students[k].get_end()->priority;
				}
			}
			//만약 다 더해진 가중치의 합이 res보다 작으면 더 작은 값으로 갱신한다.
			if(res > weightTable[i][j])
			{
				compare += 1;
				res = weightTable[i][j];
			}
		}
	}
	return res;
}

void CheckCollision::getOptimalTimes()
{
	//min에 최소 가중치가 저장된다.
	//available_times에 min의 가중치와 같은 요일과 시간이 저장된다.
	int min = makeWeightTable();
	for (int i = 0; i < DOW; i ++)
	{
		for (int j = 0; j < N_TIMES - INTERVAL + 1; j++)
		{
			compare += 1;
			if (weightTable[i][j] == min)
			{
				float start = static_cast<float>(0.5 * j + 1.0);
				float end = static_cast<float>(0.5 * (j + INTERVAL - 1) + 1.0);
				STime t(i, start, end);
				available_times.push_back(t);
			}
		}
	}
}

int main()
{
	CheckCollision c;
	std::string num;
	string fileway;
	string filename = "/Users/jeongchaeu/Downloads/data/data";
	for (int i = 1; i <= 100; i++) {
		num = std::to_string(i);
		c.addstudent(filename + num + ".txt");
	}
	cout << "hello" << endl;
	c.getOptimalTimes();
	c.show_available_times();
	cout << "비교연산 횟수 : " << compare << endl;
	cout << "사칙연산 횟수 : " << mathOp << endl;
	cout << "total : " << compare + mathOp << endl;
}