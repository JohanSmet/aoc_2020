#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <string>

using namespace std;
namespace ranges = std::ranges;

int main() {
	string line;

	array<int, 26>	answer_count = {0};
	int				person_count = 0;
	int				sum = 0;

	while (getline(cin, line)) {
		if (!line.empty()) {
			person_count += 1;
			for (char c : line) {
				answer_count[c - 'a'] += 1;
			}
		} else {
			sum += ranges::count(answer_count, person_count);
			person_count = 0;
			ranges::fill(answer_count, 0);
		}
	}

	sum += ranges::count(answer_count, person_count);

	cout << sum << "\n";

	return 0;
}
