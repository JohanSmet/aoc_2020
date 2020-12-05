#include <iostream>
#include <string>
#include <vector>

using namespace std;

int64_t count_trees_on_slope(const vector<string> &terrain, int x_delta, int y_delta, char tree) {
	const auto terrain_width = terrain[0].size();

	int x = 0;
	int64_t trees = 0;

	for (int y = 0; y < terrain.size(); y += y_delta) {
		trees += (terrain[y][x] == tree);
		x = (x + x_delta) % terrain_width;
	}

	return trees;
}

int main() {
	vector<string> terrain;

	// load terrain
	string line;
	while (getline(cin, line)) {
		terrain.push_back(line);
	}

	// traverse terrain
	constexpr const char tree = '#';

	auto result = count_trees_on_slope(terrain, 1, 1, tree);
	result *= count_trees_on_slope(terrain, 3, 1, tree);
	result *= count_trees_on_slope(terrain, 5, 1, tree);
	result *= count_trees_on_slope(terrain, 7, 1, tree);
	result *= count_trees_on_slope(terrain, 1, 2, tree);

	cout << result << "\n";

	return 0;
}
