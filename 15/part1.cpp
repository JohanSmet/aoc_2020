#include <numeric>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

int main() {

	regex r_input("(\\d+)[,]*");
	smatch sm;

	string line;
	getline(cin, line);

	vector<int64_t> dataset;

	while (regex_search(line, sm, r_input)) {
		dataset.push_back(stoi(sm[1].str()));
		//cout << dataset.back() << "\n";
		line = sm.suffix();
	}

	for (int turn = dataset.size(); turn < 2020; ++turn) {
		auto found = find(next(rbegin(dataset)), rend(dataset), dataset.back());

		if (found == rend(dataset)) {
			dataset.push_back(0);
		} else {
			dataset.push_back(found - rbegin(dataset));
		}
	}

	cout << dataset.back() << endl;

	return 0;
}
