#include <numeric>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

class Rule {
public:
	Rule() = default;
	Rule(const Rule &) = delete;
	Rule(Rule &&other) = default;

	void push_range(int min, int max) {
		ranges.push_back(make_pair(min, max));
	}

	bool validate_field(int field) const {
		for (auto [min, max] : ranges) {
			if (field >= min && field <= max) {
				return true;
			}
		}
		return false;
	}

	bool is_assigned() const {return field != -1;}
	void assign_to_field(int f) {field = f;}
	int assigned_field() const {return field;}

private:
	vector<pair<int, int>> ranges;
	int field = -1;
};

using ticket_t = vector<int>;

static vector<Rule> rules;
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

		Rule rule;

		while (regex_search(line, sm, r_rule)) {
			rule.push_range(stoi(sm[1].str()), stoi(sm[2].str()));
			line = sm.suffix();
		}

		rules.push_back(move(rule));
	}

	if (!getline(cin, line) || line != "your ticket:") {
		cerr << "Bad input -- expected \"your ticket:\"\n";
		return false;
	}

	if (!getline(cin, line)) {
		cerr << "Error reading my ticket" << endl;
	}

	parse_ticket(line, my_ticket);
	getline(cin, line);

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
		if (rule.validate_field(value)) {
			return true;
		}
	}

	return false;
}

bool is_invalid_ticket(const ticket_t &ticket) {

	for (auto field : ticket) {
		if (!validate_field(field)) {
			return true;
		}
	}

	return false;
}

bool find_one_field_rule(vector<int> &available_fields) {

	for (int field : available_fields) {

		vector<int> possible_rules;
		for (int i = 0; i < rules.size(); ++i) {
			if (!rules[i].is_assigned()) {
				possible_rules.push_back(i);
			}
		}

		// check this field in all tickets
		for (const auto &ticket : tickets) {
			auto iter = begin(possible_rules);

			while (iter != end(possible_rules)) {
				const Rule &rule = rules[*iter];

				if (!rule.validate_field(ticket[field])) {
					iter = possible_rules.erase(iter);
				} else {
					++iter;
				}
			}

			if (possible_rules.empty()) {
				break;
			}
		}

		if (possible_rules.size() == 1) {
			rules[possible_rules.front()].assign_to_field(field);
			available_fields.erase(remove(begin(available_fields), end(available_fields), field), end(available_fields));
			return true;
		}
	}

	return false;
}

int main() {

	read_input();

	cout << "Input tickets: " << tickets.size() << endl;
	tickets.erase(remove_if(begin(tickets), end(tickets), is_invalid_ticket), end(tickets));
	cout << "Valid tickets: " << tickets.size() << endl;


	vector<int> available_fields(rules.size());
	iota(begin(available_fields), end(available_fields), 0);

	while (!available_fields.empty()) {
		if (!find_one_field_rule(available_fields)) {
			cerr << "Error assigning all rules\n";
			return -1;
		}
	}

	int64_t result = 1;

	for (int i = 0; i < 6; ++i) {
		result *= my_ticket[rules[i].assigned_field()];
	}

	cout << result << endl;

	return 0;
}
