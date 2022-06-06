#include <array>
#include <iostream>

using namespace std;

/**
* 만들어진 결과 값을 받는다. 
* params arr interval에 대해 얻은 전체 데이터의 수치
* params n_num 사람의 수
* params n_num 간격
* description 평균을 내는 간격이 4, 사람의 수가 5라면  하나의 값에는 최소 5, 최대 20의 범위를 가지므로, 5 * ( 4 - 1 ) + 1 = 16개의 데이터를 가진다.
* 해당 범위의 숫자에 대해 counting sort을 수행한 결과를 반환한다. 
*/
template <int n, int n_num, int interval = 4, int n_count = n_num * (interval - 1) + 1>
std::array<int, n> counting_sort(std::array<int, n> arr)
{
	//const int n_count = n_num * (interval - 1) + 1; // 위에서 나온 값
	const int base = n_num; // 베이스가 되는 값으로, 사람의 수와 대응. 인덱스는 0부터 시작하니까 일괄적으로 5씩 더함.

	std::array<int, n_count> c_table = {0}; // 카운트 테이블 생성
	std::array<int, n> result = {0};

	for (const auto& val : arr) // arr 내에 있는 값들에 대해 계수 구하기.
	{
		cout << val - base << endl;
		c_table[val - base] += 1;
	}

	for (int i = 1; i < n_count; i++) // 계수를 누적합으로 나타내기
	{
		c_table[i] += c_table[i - 1]; // 이전 인덱스의 값 더한다
	}
	
	//for (const auto& val : c_table) // arr 내에 있는 값들에 대해 계수 구하기.
	//{
	//	cout << val << endl;
	//}

	for (int i = n-1; i >= 0; i--) // 뒤에서부터 값을 정렬한다.
	{
		int idx = c_table[arr[i] - base] - 1; // 현재 값의 인덱스 위치 구하기.
		//cout << idx << " ";
		c_table[arr[i] - base] -= 1; // 해당 인덱스 썼으니까 값 감소.
		result[idx] = arr[i]; // 실제 값 넣기
	}

	return result;
}

int main() {
	// 5명, interval 4로 가정.
	std::array<int, 15> arr = { 15, 5 ,10, 6, 8, 6, 9, 20, 18, 16, 7, 15, 10, 9, 18 };

	auto arr2 = counting_sort<15, 5, 4>(arr);

	for (const auto& v : arr2)
	{
		std::cout << v << " ";
	}
}