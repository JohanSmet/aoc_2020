#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
	vector<string> terrain;

	// load terrain
	string line;
	while (getline(cin, line)) {
		terrain.push_back(line);
	}

	// traverse terrain
	const auto terrain_width = terrain[0].size();
	constexpr const int x_delta = 3;
	constexpr const int y_delta = 1;
	constexpr const char tree = '#';

	int x = 0;
	int trees = 0;

	for (int y = 0; y < terrain.size(); y += y_delta) {
		trees += (terrain[y][x] == tree);
		x = (x + x_delta) % terrain_width;
	}

	cout << trees << "\n";

	return 0;
}
