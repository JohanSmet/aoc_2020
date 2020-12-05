#include <iostream>
#include <string>
#include <string_view>

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

	int max_id = 0;

	while (getline(cin, boarding_pass)) {
		max_id = max(max_id, decode_boardingpass(boarding_pass));
	}

	cout << max_id << "\n";

	return 0;
}
