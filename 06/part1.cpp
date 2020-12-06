#include <iostream>
#include <set>
#include <string>

using namespace std;

int main() {
	string line;
	set<char> answers;

	int sum = 0;

	while (getline(cin, line)) {
		if (!line.empty()) {
			answers.insert(begin(line), end(line));
		} else {
			sum += answers.size();
			answers.clear();
		}
	}

	sum += answers.size();

	cout << sum << "\n";

	return 0;
}
