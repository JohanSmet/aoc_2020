#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main() {
	int target;
	string departures;

	cin >> target;
	getline(cin, departures);
	getline(cin, departures);

	regex id_regex("(\\d+),*", regex_constants::ECMAScript);
	smatch sm;

	int min_diff = target;
	int best_id = 0;

	while (regex_search(departures, sm, id_regex)) {
		int id = stoi(sm[1].str());

		int diff = (((target / id) + 1) * id) - target;

		if (diff > 0 && diff < min_diff) {
			min_diff = diff;
			best_id = id;
		}

		departures = sm.suffix();
	}

	cout << best_id * min_diff << endl;



	return 0;
}
