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

	for (auto first = begin(dataset), last = begin(dataset) + PREAMBLE_LEN; last + 1 != end(dataset); ++first, ++last) {
		number = *last;
		if (!check_valid_number(first, last, number)) {
			break;
		}
	}

	cout << number << "\n";
	return 0;
}
