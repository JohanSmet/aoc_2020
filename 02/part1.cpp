#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

struct Entry {
	int		min_count;
	int		max_count;
	char	letter;
	string  password;
};

Entry parse_entry(const std::string &line) {
	Entry result;

	stringstream ss(line);
	char dummy1, dummy2;

	ss >> result.min_count >> dummy1 >> result.max_count >> result.letter >> dummy2 >> result.password;
	return result;
}

bool is_entry_valid(const Entry &entry) {
	int num = std::count(begin(entry.password), end(entry.password), entry.letter);
	return num >= entry.min_count && num <= entry.max_count;
}

int main() {
	string line;
	int total_ok = 0;

	while (getline(cin, line)) {
		auto entry = parse_entry(line);
		total_ok += is_entry_valid(entry);
	}

	cout << total_ok << "\n";

	return 0;
}
