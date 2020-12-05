#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, function<bool(string)>> field_validation;

bool number_between(const std::string &value, int min, int max) {
	auto iv = stoi(value);
	return iv >= min && iv <= max;
}

void setup_validation_rules() {
	field_validation["byr"] = [](string value) -> bool { return number_between(value, 1920, 2002); };
	field_validation["iyr"] = [](string value) -> bool { return number_between(value, 2010, 2020); };
	field_validation["eyr"] = [](string value) -> bool { return number_between(value, 2020, 2030); };
	field_validation["hgt"] = [](string value) -> bool {
		if (value.ends_with("cm")) {
			return number_between(value, 150, 193);
		} else if (value.ends_with("in")) {
			return number_between(value, 59, 76);
		} else {
			return false;
		}
	};
	field_validation["hcl"] = [](string value) -> bool {
		if (value[0] != '#' || value.size() != 7) {
			return false;
		}

		return all_of(begin(value) + 1, end(value), [](auto c) -> bool {
				return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
		});

   	};
	field_validation["ecl"] = [](string value) -> bool {
		return value == "amb" || value == "blu" || value == "brn" ||
			   value == "gry" || value == "grn" || value == "hzl" ||
			   value == "oth";
	};
	field_validation["pid"] = [](string value) -> bool {
		return (value.size() == 9 &&
			    all_of(begin(value), end(value), [](auto c) -> bool {
					return c >= '0' && c <= '9';
			    }));
	};
}

int validate_passport_line(const std::string &line) {

	constexpr const char FS = ':';
	constexpr const char RS = ' ';

	int valid_fields = 0;

	size_t start = 0;
	auto pos_fs = line.find_first_of(FS, start);

	while (pos_fs != string::npos) {
		auto pos_rs = line.find_first_of(RS, pos_fs);

		auto field = line.substr(start, pos_fs - start);
		auto value = line.substr(pos_fs + 1, pos_rs - pos_fs - 1);

		// validation
		auto validator = field_validation.find(field);
		if (validator != end(field_validation)) {
			valid_fields += validator->second(value);
		}

		start = line.find_first_not_of(RS, pos_rs);
		pos_fs = line.find_first_of(FS, start);
	}

	return valid_fields;
}

int main() {

	setup_validation_rules();

	string line;

	int field_count = 0;
	int valid_passports = 0;

	while (getline(cin, line)) {
		if (line.empty()) {
			valid_passports += (field_count == field_validation.size());
			field_count = 0;
		} else {
			field_count += validate_passport_line(line);
		}
	}

	valid_passports += (field_count == field_validation.size());

	cout << valid_passports << "\n";
	return 0;
}
