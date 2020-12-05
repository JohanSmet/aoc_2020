#include <array>
#include <iostream>
#include <string>

using namespace std;

const array<string,7> REQUIRED_FIELDS = {
	"byr:",
	"iyr:",
	"eyr:",
	"hgt:",
	"hcl:",
	"ecl:",
	"pid:"
};

int count_fields(const string &line) {
	int count = 0;

	for (const auto &field : REQUIRED_FIELDS) {
		count += line.find(field) != string::npos;
	}

	return count;
}

int main() {
	string line;

	int field_count = 0;
	int valid_passports = 0;

	while (getline(cin, line)) {
		if (line.empty()) {
			valid_passports += (field_count == REQUIRED_FIELDS.size());
			field_count = 0;
		} else {
			field_count += count_fields(line);
		}
	}

	valid_passports += (field_count == REQUIRED_FIELDS.size());

	cout << valid_passports << "\n";
	return 0;
}
