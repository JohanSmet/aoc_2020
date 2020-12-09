#include <algorithm>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

// you can tell I'm good at regular expressions
static regex color_parser("(?: contain \\d* *)?(?:, \\d* *)?(.*?) bags?", regex_constants::ECMAScript);

static unordered_map<string, size_t> color_id_map;
static vector<string> known_colors;
static vector<vector<int>> bag_children;

static vector<string> parse_bag_colors(std::string input) {
	smatch sm;
	vector<string> result;

	while (regex_search(input, sm, color_parser)) {
		result.push_back(sm[1].str());
		input = sm.suffix();
	}

	return result;
}

inline size_t color_id(const std::string &color) {
	auto found = color_id_map.find(color);
	if (found != end(color_id_map)) {
		return found->second;
	}

	size_t id = color_id_map.size();
	color_id_map[color] = id;
	known_colors.push_back(color);
	return id;
}

bool find_target(int current, int target, set<int> visited = {}) {

	if (ranges::any_of(bag_children[current], [=](auto x) {return x == target;})) {
		return true;
	}

	visited.insert(current);

	for (auto child: bag_children[current]) {
		if (!visited.contains(child) && find_target(child, target, visited)) {
			return true;
		}
	}

	return false;
}

int main() {
	string line;

	while (getline(cin, line)) {
		auto colors = parse_bag_colors(line);

		int key = color_id(colors[0]);
		if (bag_children.size() <= key) {
			bag_children.resize(key + 1);
		}

		for (int child = 1; child < colors.size(); ++child) {
			if (colors[child] != "no other") {
				bag_children[key].push_back(color_id(colors[child]));
			}
		}
	}

	int target_id = color_id("shiny gold");
	int sum = 0;

	for (int container = 0; container < bag_children.size(); ++container) {
		sum += find_target(container, target_id);
	}

	cout << sum << "\n";

	return 0;
}
