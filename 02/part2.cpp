#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

struct Entry {
	int		first_pos;
	int		second_pos;
	char	letter;
	string  password;
};

Entry parse_entry(const std::string &line) {
	Entry result;

	stringstream ss(line);
	char dummy1, dummy2;

	ss >> result.first_pos >> dummy1 >> result.second_pos >> result.letter >> dummy2 >> result.password;
	return result;
}

bool is_entry_valid(const Entry &entry) {
	auto n = entry.password.size();
	if (entry.first_pos > n || entry.second_pos > n) {
		return false;
	}

	bool c1 = entry.password[entry.first_pos-1] == entry.letter;
	bool c2 = entry.password[entry.second_pos-1] == entry.letter;

	return c1 ^ c2;
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
