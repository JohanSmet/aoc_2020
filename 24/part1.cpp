#include <array>
#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;
namespace ranges = std::ranges;

union Tile {
	struct {
		int16_t x;
		int16_t y;
	};
	int32_t full;
};

Tile &operator+=(Tile &lhs, const Tile &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

bool operator<(const Tile &lhs, const Tile &rhs) {
	return lhs.full < rhs.full;
}

enum DIRECTIONS {
	EAST = 0,
	SOUTH_EAST,
	SOUTH_WEST,
	WEST,
	NORTH_WEST,
	NORTH_EAST
};

Tile TILE_DELTA[] = {
	{2, 0},
	{1, 1},
	{-1, 1},
	{-2, 0},
	{-1, -1},
	{1, -1}
};

int main() {

	// determine starting grid
	set<Tile> black_tiles = {};

	string line;
	while (getline(cin, line)) {
		Tile current = {0,0};

		int idx = 0;
		while (idx < line.size()) {

			if (line[idx] == 'e') {
				current += TILE_DELTA[EAST];
				idx += 1;
			} else if (line[idx] == 'w') {
				current += TILE_DELTA[WEST];
				idx += 1;
			} else if (line[idx] == 's') {
				if (line[idx+1] == 'e') {
					current += TILE_DELTA[SOUTH_EAST];
				} else if (line[idx+1] == 'w') {
					current += TILE_DELTA[SOUTH_WEST];
				} else {
					cerr << "Error at position " << idx << " in command " << line << endl;
					return -1;
				}
				idx += 2;
			} else if (line[idx] == 'n') {
				if (line[idx+1] == 'e') {
					current += TILE_DELTA[NORTH_EAST];
				} else if (line[idx+1] == 'w') {
					current += TILE_DELTA[NORTH_WEST];
				} else {
					cerr << "Error at position " << idx << " in command " << line << endl;
					return -1;
				}
				idx += 2;
			} else {
				cerr << "Error at position " << idx << " in command " << line << endl;
				return -1;
			}
		}

		auto black = black_tiles.find(current);

		if (black == end(black_tiles)) {
			black_tiles.insert(current);
		} else {
			black_tiles.erase(black);
		}
	}

	cout << "Black tiles = " << black_tiles.size() << endl;

	return 0;
}
