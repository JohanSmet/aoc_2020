#include <numeric>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

using rule_t = vector<pair<int,int>>;
using ticket_t = vector<int>;

static vector<rule_t> rules;
static ticket_t my_ticket;
static vector<ticket_t> tickets;

void parse_ticket(string line, ticket_t &ticket) {
	regex r_ticket("(\\d+),?");
	smatch sm;

	while (regex_search(line, sm, r_ticket)) {
		ticket.push_back(stoi(sm[1].str()));
		line = sm.suffix();
	}
}

bool read_input() {

	string line;

	regex r_rule("(\\d+)-(\\d+)");

	// rules
	while (getline(cin, line) && !line.empty()) {
		smatch sm;

		rule_t rule;

		while (regex_search(line, sm, r_rule)) {
			rule.push_back(make_pair(stoi(sm[1].str()), stoi(sm[2].str())));
			line = sm.suffix();
		}

		rules.emplace_back(move(rule));
	}

	if (!getline(cin, line) || line != "your ticket:") {
		cerr << "Bad input -- expected \"your ticket:\"\n";
		return false;
	}

	if (!getline(cin, line)) {
		cerr << "Error reading my ticket" << endl;
	}

	getline(cin, line);
	parse_ticket(line, my_ticket);

	if (!getline(cin, line) || line != "nearby tickets:") {
		cerr << "Bad input -- expected \"nearby tickets:\"\n";
		return false;
	}

	while (getline(cin, line)) {
		tickets.push_back({});
		parse_ticket(line, tickets.back());
	}

	return true;
}

bool validate_field(int value) {

	for (const auto &rule : rules) {
		for (auto [min, max] : rule) {
			if (value >= min && value <= max) {
				return true;
			}
		}
	}

	return false;
}


int main() {

	read_input();

	int64_t sum = 0;

	for (const auto &ticket : tickets) {
		for (auto field : ticket) {
			if (!validate_field(field)) {
				sum += field;
			}
		}
	}

	cout << sum << endl;

	return 0;
}
