#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

int decode_boardingpass(string_view pass) {

	int min_row = 0;
	int max_row = 127;

	int min_col = 0;
	int max_col = 7;

	for (auto c : pass) {
		if (c == 'F') {
			max_row = min_row + ((max_row - min_row) / 2);
		} else if (c == 'B') {
			min_row = min_row + ((max_row - min_row) / 2) + 1;
		} else if (c == 'L') {
			max_col = min_col + ((max_col - min_col) / 2);
		} else if (c == 'R') {
			min_col = min_col + ((max_col - min_col) / 2) + 1;
		}
	}

	int seat_id = (min_row * 8) + min_col;
	return seat_id;
}

int main() {
	string boarding_pass;

	vector<int> ids;

	while (getline(cin, boarding_pass)) {
		auto seat_id = decode_boardingpass(boarding_pass);
		ids.insert(upper_bound(begin(ids), end(ids), seat_id), seat_id);
	}

	auto boundary = adjacent_find(begin(ids), end(ids), [](auto a, auto b) {return (b - a) > 1;});
	if (boundary != end(ids)) {
		cout << *boundary + 1 << "\n";
	}

	return 0;
}
