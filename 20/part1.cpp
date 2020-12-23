#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

enum Sides {
	TOP = 0,
	RIGHT = 1,
	BOTTOM = 2,
	LEFT = 3
};

class Tile {
public:
	Tile() = default;

	bool read_tile();

public:
	int id;
	array<string, 10>	data;
	array<string, 4>	edges;
};

bool Tile::read_tile() {
	string str_data;

	// tile-id
	if (!(cin >> str_data)) {
		return false;
	}

	cin >> id;
	getline(cin, str_data);

	// tile-data
	for (int i = 0; i < 10; ++i) {
		getline(cin, data[i]);
	}

	// empty line
	getline(cin, str_data);

	// cache edges
	edges[TOP] = data[0];
	edges[BOTTOM] = string(rbegin(data[9]), rend(data[9]));
	edges[RIGHT] = "";
	edges[LEFT] = "";

	for (int i = 0; i < 10; ++i) {
		edges[RIGHT] += data[i][9];
		edges[LEFT] += data[i][0];
	}

	return true;
}

vector<Tile> load_tiles() {
	vector<Tile> result;

	Tile tile;

	while (tile.read_tile()) {
		result.push_back(tile);
	}

	return result;
}

int main() {

	// load tiles
	auto tiles = load_tiles();

	// count occurences of edges
	unordered_map<string, int> edge_map;
	for (const auto &tile : tiles) {
		for (string e: tile.edges) {
			edge_map[e] += 1;
			edge_map[string(rbegin(e), rend(e))] += 1;
		}
	}

	// classify tiles - assuming that a specific edge configuration is unique
	int64_t product = 1;

	for (const auto &tile : tiles) {
		int count[] = {0, 0, 0};

		for (string e: tile.edges) {
			count[edge_map[e]] += 1;
		}

		// cout << tile.id << " [1] == " << count[1] << " [2] == " << count[2] << endl;
		if (count[1] == 2 && count[2] == 2) {
			cout << tile.id << endl;
			product *= tile.id;
		}
	}

	cout << product << endl;

	return 0;
}
