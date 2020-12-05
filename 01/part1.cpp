#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
	constexpr const int target = 2020;
	int expense;
	vector<int> cache;

	while (cin >> expense) {
		int required = target - expense;

		if (binary_search(begin(cache), end(cache), required)) {
			cout << expense << " * " << required << " = " << (expense * required) << "\n";
			break;
		}

		auto ip = upper_bound(begin(cache), end(cache), expense);
		cache.insert(ip, expense);
	}

	return 0;
}
