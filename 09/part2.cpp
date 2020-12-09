#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

constexpr int PREAMBLE_LEN = 25;

template <typename InputIt>
static bool check_valid_number(InputIt first, InputIt last, int64_t number) {

	for (auto iter = first; iter != last; ++iter) {
		int64_t given = *iter;
		int64_t req = number - given;

		if (req == given) {
			continue;
		}

		if (find(first, last, req) != last) {
			return true;
		}
	}

	return false;
}

int main() {
	int64_t number;
	vector<int64_t> dataset;

	while (cin >> number) {
		dataset.push_back(number);
	}

	if (dataset.size() < PREAMBLE_LEN) {
		cerr << "Dataset invalid - insufficient data\n";
		return -1;
	}

	// find the first invalid number
	for (auto first = begin(dataset), last = begin(dataset) + PREAMBLE_LEN; last + 1 != end(dataset); ++first, ++last) {
		number = *last;
		if (!check_valid_number(first, last, number)) {
			break;
		}
	}

	int64_t invalid_number = number;

	// try to find a set of contiguous numbers that add up to the invalid number
	vector<int64_t>::iterator first;
	vector<int64_t>::iterator last;

	for (first = begin(dataset); first != end(dataset); ++first) {
		int64_t sum = 0;

		for (last = first; last != end(dataset) && sum < invalid_number; ++last) {
			sum += *last;
		}

		if (sum == invalid_number) {
			break;
		}
	}

	auto [lower, upper] = minmax_element(first, last);
	cout << *lower + *upper << endl;

	return 0;
}
