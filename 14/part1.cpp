#include <numeric>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

void convert_mask(string_view mask, uint64_t &true_mask, uint64_t &false_mask) {
	true_mask = 0;
	false_mask = 0xffffffffffffffff;

	int index = 35;

	for (auto c : mask) {
		if (c == '1') {
			true_mask = true_mask | (1ull << index);
		} else if (c == '0') {
			false_mask = false_mask & ~(1ull << index);
		}
		index -= 1;
	}
}

int main() {

	regex r_mask("mask = ([X01]+)");
	regex r_mem("mem\\[(\\d+)\\] = (\\d+)");

	uint64_t true_mask, false_mask;
	vector<uint64_t> memory;

	string line;

	while (getline(cin, line)) {
		smatch sm;

		if (regex_match(line, sm, r_mask)) {
			convert_mask(sm[1].str(), true_mask, false_mask);
		} else if (regex_match(line, sm, r_mem)) {
			uint64_t loc = stoi(sm[1].str());
			uint64_t data = stoi(sm[2].str());

			if (memory.size() <= loc) {
				memory.resize(loc + 1);
			}
			memory[loc] = (data | true_mask) & false_mask;
			// cout << "memory[" << loc << "] = " << memory[loc] << " (t=" << true_mask << ",f=" << false_mask << ")\n";
		}
	}

	cout << accumulate(begin(memory), end(memory), 0ull) << endl;


	return 0;
}
