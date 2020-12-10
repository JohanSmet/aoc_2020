#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

int main() {
	int64_t number;
	vector<int64_t> dataset;

	while (cin >> number) {
		dataset.insert(upper_bound(begin(dataset), end(dataset), number), number);
	}

	adjacent_difference(begin(dataset), end(dataset), begin(dataset));
	dataset.push_back(3);

	cout << ranges::count(dataset, 1ll) << " difference(s) of 1 jolt, and "
		 << ranges::count(dataset, 3ll) << " difference(s) of 3 jolts.\n";


	return 0;
}
