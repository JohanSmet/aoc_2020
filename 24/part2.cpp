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

	Tile() = default;
	Tile(int16_t x, int16_t y) : x(x), y(y) {
	}
};

Tile &operator+=(Tile &lhs, const Tile &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Tile operator+(const Tile &lhs, const Tile &rhs) {
	return Tile(lhs.x + rhs.x, lhs.y + rhs.y);
}

bool operator<(const Tile &lhs, const Tile &rhs) {
	return lhs.full < rhs.full;
}

ostream &operator<<(ostream &os, const Tile &t) {
	os << "[" << t.x << ", " << t.y << "]";
	return os;
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

void grid_extents(const set<Tile> &black_tiles, Tile &out_min, Tile &out_max) {

	out_min = *begin(black_tiles);
	out_max = out_min;

	for (const auto &t : black_tiles) {
		out_min.x = min(out_min.x, t.x);
		out_min.y = min(out_min.y, t.y);
		out_max.x = max(out_max.x, t.x);
		out_max.y = max(out_max.y, t.y);
	}
}

int count_neighbour_black_tiles(const set<Tile> &black_tiles, const Tile &tile) {
	int result = 0;

	for (int dir = 0; dir < 6; ++dir) {
		result += black_tiles.contains(tile + TILE_DELTA[dir]);
	}

	return result;
}

void process_day(const set<Tile> &grid, set<Tile> &new_grid) {

	Tile grid_min, grid_max;
	grid_extents(grid, grid_min, grid_max);

	new_grid.clear();

	Tile t;

	for (t.y = grid_min.y - 2; t.y <= grid_max.y + 2; ++t.y) {
		for (t.x = grid_min.x - 2; t.x <= grid_max.x + 2; ++t.x) {
			int nc = count_neighbour_black_tiles(grid, t);
			bool is_black = grid.contains(t);

			if (is_black && (!(nc == 0 || nc > 2))) {
				new_grid.insert(t);
			}

			if (!is_black && nc == 2) {
				new_grid.insert(t);
			}
		}
	}
}


int main() {

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

	set<Tile> new_tiles = {};

	auto &a = black_tiles;
	auto &b = new_tiles;

	for (int i = 1; i <= 100; ++i) {
		process_day(a, b);
		cout << "Day " << i << ": " << b.size() << endl;
		swap(a,b);
	}

	return 0;
}
