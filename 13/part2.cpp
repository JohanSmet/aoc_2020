#include <algorithm>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>

// wrote a quick brute force solution to run while researching better solutions
//	but it completed in about 15 minutes, better solution will have to wait until AoC is done

using namespace std;
namespace ranges = std::ranges;

struct Element {
	int id;
	int offset;
};

static void read_input(vector<Element> &dataset) {
	string departures;
	getline(cin, departures);
	getline(cin, departures);

	regex id_regex("(\\w+),*", regex_constants::ECMAScript);
	smatch sm;

	int index = 0;

	while (regex_search(departures, sm, id_regex)) {
		if (sm[1].str() != "x") {
			dataset.emplace_back(stoi(sm[1].str()), index);
		}

		index += 1;
		departures = sm.suffix();
	}
}

int main() {
	vector<Element> dataset;

	// load data
	read_input(dataset);

	// find largest id and retarget offsets
	auto max_el = ranges::max_element(dataset, [](auto a, auto b) {return a.id < b.id;});
	int max_offset = max_el->offset;

	transform(begin(dataset), end(dataset), begin(dataset),
			  [=] (auto el) {
				return Element{el.id, el.offset - max_offset};
			  });

	uint64_t product = (100000000000000ll / max_el->id) * max_el->id;
	bool ok = false;
	uint64_t iter = 0;

	while (!ok) {
		ok = true;
		product += max_el->id;

		iter += 1;
		if (iter % 1000000000 == 0) {
			cout << product << endl;
		}

		for (auto el: dataset) {
			if (((product + el.offset) % el.id) != 0) {
				ok = false;
				break;
			}
		}

	}

	cout << "Result: " <<  product - max_offset << endl;

	return 0;
}
