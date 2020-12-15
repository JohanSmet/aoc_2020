#include <numeric>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {

	regex r_input("(\\d+)[,]*");
	smatch sm;

	string line;
	getline(cin, line);

	unordered_map<int, int> dataset;

	regex_search(line, sm, r_input);

	int last = stoi(sm[1].str());
	int next;
	line = sm.suffix();

	while (regex_search(line, sm, r_input)) {
		int turn = dataset.size() + 1;
		dataset[last] = turn;

		last = stoi(sm[1].str());
		line = sm.suffix();
	}

	for (int turn = dataset.size() + 1; turn < 30000000; ++turn) {
		auto found = dataset.find(last);

		if (found == end(dataset)) {
			next = 0;
		} else {
			next = turn - found->second;
		}

		dataset[last] = turn;
		last = next;

	}

	cout << last << endl;

	return 0;
}
