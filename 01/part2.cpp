#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int product_of_target_sum(const vector<int> &dataset, int target) {
	for (auto i = dataset.begin(); i != dataset.end(); ++i) {
		int required = target - *i;

		if (binary_search(i, end(dataset), required)) {
			return *i * required;
		}
	}

	return 0;
}

int main() {
	constexpr const int target = 2020;
	int expense;
	vector<int> dataset;

	while (cin >> expense) {
		auto ip = upper_bound(begin(dataset), end(dataset), expense);
		dataset.insert(ip, expense);
	}

	for (auto n1 : dataset) {
		auto result = product_of_target_sum(dataset, target - n1);
		if (result != 0) {
			cout << result * n1 << "\n";
			break;
		}
	}

	return 0;
}
