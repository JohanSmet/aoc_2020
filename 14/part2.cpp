#include <unordered_map>
#include <numeric>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct Masks {
	uint64_t true_mask;
	uint64_t false_mask;
};

vector<Masks> address_masks;

void convert_mask(string_view mask) {

	address_masks.clear();
	address_masks.reserve(1 << count(begin(mask), end(mask), 'X'));
	address_masks.push_back(Masks{0ull, 0ull});

	int index = 35;

	for (auto c : mask) {
		if (c == '1') {
			for (auto &m : address_masks) {
				m.true_mask = m.true_mask | (1ull << index);
			}
		} else if (c == 'X') {
			auto first = begin(address_masks);
			auto last = end(address_masks);

			transform(first, last, back_inserter(address_masks),
					  [index](auto m) {
							m.true_mask = m.true_mask | (1ull << index);
							return m;
					  });

			transform(first, last, first,
					  [index](auto m) {
							m.false_mask = m.false_mask | (1ull << index);
							return m;
					  });
		}
		index -= 1;
	}
}

int main() {

	regex r_mask("mask = ([X01]+)");
	regex r_mem("mem\\[(\\d+)\\] = (\\d+)");

	unordered_map<uint64_t, uint64_t> memory;

	string line;

	while (getline(cin, line)) {
		smatch sm;

		if (regex_match(line, sm, r_mask)) {
			convert_mask(sm[1].str());
		} else if (regex_match(line, sm, r_mem)) {
			uint64_t loc = stoi(sm[1].str());
			uint64_t data = stoi(sm[2].str());

			for (const auto &m : address_masks) {
				uint64_t rloc = (loc | m.true_mask) & ~m.false_mask;
				memory[rloc] = data;
			}
		}
	}

	cout << accumulate(begin(memory), end(memory), 0ull, [](auto a, auto b) -> uint64_t {return a + b.second;}) << endl;

	return 0;
}
