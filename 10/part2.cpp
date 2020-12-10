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

	/* examining the input data shows us that are a maximum of four 1's in a row */
	size_t combo_lut[] = {
		1,
		1,
		2,
		4,
		7
	};
	
	int64_t total = 1;

	auto first = find(begin(dataset), end(dataset), 1);

	while (first != end(dataset)) {
		auto last = find(next(first), end(dataset), 3);
		
		auto len = last - first;
		total *= combo_lut[len];

		first = find(next(last), end(dataset), 1);
	}

	cout << total << endl;

	return 0;
}
