#include <algorithm>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

// you can tell I'm good at regular expressions
static regex color_parser("(?: contain )?(?:, )?(\\d* *.*?) bags?", regex_constants::ECMAScript);

static unordered_map<string, size_t> color_id_map;
static vector<string> known_colors;

static vector<vector<int>> bag_children;
static vector<vector<int>> bag_count;

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

size_t count_bags(int current, int factor) {

	size_t sum = 0;

	for (int i = 0; i < bag_children[current].size(); ++i) {
		sum += (bag_count[current][i] * factor);
		sum += count_bags(bag_children[current][i], (bag_count[current][i] * factor));
	}

	return sum;
}

int main() {
	string line;

	while (getline(cin, line)) {
		auto colors = parse_bag_colors(line);

		int key = color_id(colors[0]);
		if (bag_children.size() <= key) {
			bag_children.resize(key + 1);
			bag_count.resize(key + 1);
		}

		for (int child = 1; child < colors.size(); ++child) {
			if (colors[child] != "no other") {
				int split = colors[child].find_first_of(' ');
				bag_count[key].push_back(stoi(colors[child].substr(0, split)));
				bag_children[key].push_back(color_id(colors[child].substr(split+1)));
			}
		}
	}

	int start = color_id("shiny gold");

	cout << count_bags(start, 1) << "\n";

	return 0;
}
