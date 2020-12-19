#include <deque>
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>

using namespace std;

class Rule {
public:
	Rule() = default;

	void set_match(char m) {
		leaf = true;
		match_char = m;
	}

	void new_subrule() {
		sub_rules.push_back({});
	}

	void add_subrule(int rule_id) {
		sub_rules.back().push_back(rule_id);
	}

	bool match_leaf(char c) const {return c == match_char;}

public:
	bool leaf = false;
	char match_char = '\0';
	vector<vector<int>> sub_rules;
};


bool match(const vector<Rule> &rules, string_view target, deque<int> rule_set = {0}, int pos = 0) {

	// no more rules to expand - done if we matched entire string
	if (rule_set.empty()) {
	   return target.size() == pos;
	}

	// don't try matching beyond the length of the target string
	if (pos >= target.size()) {
		return false;
	}

	// expand the first rule and recurse
	int rule = rule_set.front();
	rule_set.pop_front();

	if (rules[rule].leaf) {
		if (!rules[rule].match_leaf(target[pos])) {
			return false;
		}

		if (match(rules, target, rule_set, pos + 1)) {
			return true;
		}
	} else {
		for (const auto &sr : rules[rule].sub_rules) {
			auto new_rule_set = rule_set;

			for (auto iter = rbegin(sr); iter != rend(sr); ++iter) {
				new_rule_set.push_front(*iter);
			}

			if (match(rules, target, new_rule_set, pos)) {
				return true;
			}
		}
	}

	return false;
}

vector<Rule> read_rules() {
	vector<Rule> rules;
	string line;

	auto grow_rules = [&rules](size_t n) {
		if (rules.size() <= n) {
			rules.resize(n + 1);
		}
	};

	while (getline(cin, line) && !line.empty()) {
		stringstream ss(line);
		string part;

		int rule_id;
		ss >> rule_id;
		grow_rules(rule_id);

		ss >> part;			// colon

		ss >> part;
		if (part[0] == '"') {
			rules[rule_id].set_match(part[1]);
		} else {
			rules[rule_id].new_subrule();
			do {
				if (part == "|") {
					rules[rule_id].new_subrule();
				} else {
					int sub_rule = stoi(part);
					grow_rules(sub_rule);
					rules[rule_id].add_subrule(sub_rule);
				}
			} while (ss >> part);
		}
	}

	return move(rules);
}

int main() {

	auto rules = read_rules();

	int matches = 0;
	string line;
	while (getline(cin, line)) {
		matches += match(rules,line);
	}

	cout << matches << endl;

	return 0;
}
