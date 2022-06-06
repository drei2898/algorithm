#include <array>
#include <iostream>

using namespace std;

/**
* ������� ��� ���� �޴´�. 
* params arr interval�� ���� ���� ��ü �������� ��ġ
* params n_num ����� ��
* params n_num ����
* description ����� ���� ������ 4, ����� ���� 5���  �ϳ��� ������ �ּ� 5, �ִ� 20�� ������ �����Ƿ�, 5 * ( 4 - 1 ) + 1 = 16���� �����͸� ������.
* �ش� ������ ���ڿ� ���� counting sort�� ������ ����� ��ȯ�Ѵ�. 
*/
template <int n, int n_num, int interval = 4, int n_count = n_num * (interval - 1) + 1>
std::array<int, n> counting_sort(std::array<int, n> arr)
{
	//const int n_count = n_num * (interval - 1) + 1; // ������ ���� ��
	const int base = n_num; // ���̽��� �Ǵ� ������, ����� ���� ����. �ε����� 0���� �����ϴϱ� �ϰ������� 5�� ����.

	std::array<int, n_count> c_table = {0}; // ī��Ʈ ���̺� ����
	std::array<int, n> result = {0};

	for (const auto& val : arr) // arr ���� �ִ� ���鿡 ���� ��� ���ϱ�.
	{
		cout << val - base << endl;
		c_table[val - base] += 1;
	}

	for (int i = 1; i < n_count; i++) // ����� ���������� ��Ÿ����
	{
		c_table[i] += c_table[i - 1]; // ���� �ε����� �� ���Ѵ�
	}
	
	//for (const auto& val : c_table) // arr ���� �ִ� ���鿡 ���� ��� ���ϱ�.
	//{
	//	cout << val << endl;
	//}

	for (int i = n-1; i >= 0; i--) // �ڿ������� ���� �����Ѵ�.
	{
		int idx = c_table[arr[i] - base] - 1; // ���� ���� �ε��� ��ġ ���ϱ�.
		//cout << idx << " ";
		c_table[arr[i] - base] -= 1; // �ش� �ε��� �����ϱ� �� ����.
		result[idx] = arr[i]; // ���� �� �ֱ�
	}

	return result;
}

int main() {
	// 5��, interval 4�� ����.
	std::array<int, 15> arr = { 15, 5 ,10, 6, 8, 6, 9, 20, 18, 16, 7, 15, 10, 9, 18 };

	auto arr2 = counting_sort<15, 5, 4>(arr);

	for (const auto& v : arr2)
	{
		std::cout << v << " ";
	}
}