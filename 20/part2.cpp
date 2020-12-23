#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

constexpr const char *FILTER[] = {
	"                  # ",
	"#    ##    ##    ###",
	" #  #  #  #  #  #   "
};

enum Sides {
	TOP = 0,
	RIGHT = 1,
	BOTTOM = 2,
	LEFT = 3
};

class EdgeMap {
public:
	EdgeMap() = default;

	void add_edge(int tile, const std::string &edge) {
		auto found = dataset.find(edge);
		if (found == end(dataset)) {
			dataset[edge] = make_pair(tile, -1);
		} else {
			assert(found->second.second == -1);
			found->second.second = tile;
		}
	}

	int count_edge(const std::string &edge) const {
		return (dataset.at(edge).second == -1) ? 1 : 2;
	}

	int neighbour_tile(int tile, const std::string &edge) const {
		auto &e = dataset.at(edge);
		return (e.first == tile) ? e.second : e.first;
	}

private:
	unordered_map<string, pair<int, int>> dataset;
};

class Tile {
public:
	Tile() = default;

	bool read_tile();

	void rotate_left();
	void flip();
	bool rotate_until_matches(int side_1, const std::string &edge_1, int side_2, int count_2, const EdgeMap &edge_map);

	void print() const {
		cout << "Tile " << id << "(angle=" << angle << "), flipped=" << flipped << ")" << endl;
		for (const auto &d: data) {
			cout << d << endl;
		}
	}

public:
	int					id = 0;
	vector<string>		data;
	array<string, 4>	edges;
	int					angle = 0;
	bool				flipped = false;
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
	data.resize(10);
	for (int i = 0; i < data.size(); ++i) {
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
		edges[LEFT] += data[9-i][0];
	}

	return true;
}

void Tile::rotate_left() {
	// edges
	rotate(begin(edges), next(begin(edges)), end(edges));

	// full dataset
	auto original = data;

	data.clear();

	for (int y = 0; y < original[0].size(); ++y) {
		data.push_back(string(original.size(), ' '));

		for (int x = 0; x < original.size(); ++x) {
			data[y][x] = original[x][original[0].size()-y-1];
		}
	}

	angle = (angle + 1) % 4;
}

void Tile::flip() {
	// edges
	swap(edges[LEFT], edges[RIGHT]);
	for (auto &e : edges) {
		reverse(begin(e), end(e));
	}

	// full dataset
	for (auto &d : data) {
		reverse(begin(d), end(d));
	}

	flipped = true;
}

bool Tile::rotate_until_matches(int side_1, const std::string &edge_1, int side_2, int count_2, const EdgeMap &edge_map) {

	string target = string(rbegin(edge_1), rend(edge_1));

	for (int i = 0; i < 2; ++i) {
		for (int s = 0; s < 4; ++s) {
			if (edges[side_1] == target &&
				edge_map.count_edge(edges[side_2]) == count_2) {
				return true;
			}
			rotate_left();
		}

		flip();
	}

	assert(false && "no match found");
	return false;
}

vector<Tile> load_tiles() {
	vector<Tile> result;

	Tile tile;

	while (tile.read_tile()) {
		result.push_back(tile);
	}

	return result;
}

int apply_filter(const vector<char> &image, int image_w, int image_h, const Tile &filter) {

	const int filter_h = filter.data.size();
	const int filter_w = filter.data[0].size();
	int result = 0;

	for (int sy = 0; sy < image_h - filter_h; ++sy) {
		for (int sx = 0; sx < image_w - filter_w; ++sx) {
			bool filter_match = true;
			for (int y = 0; filter_match && y < filter_h; ++y) {
				int s = ((sy + y) * image_w) + sx;
				for (int x = 0; filter_match && x < filter_w; ++x) {
					if (filter.data[y][x] != '#') {
						continue;
					}

					filter_match = image[s+x] == '#';
				}
			}

			result += filter_match;
		}
	}

	return result;
}

int main() {

	// load tiles
	auto tiles = load_tiles();

	// connect edges
	EdgeMap edge_map;

	for (int tile = 0; tile < tiles.size(); ++tile) {
		for (string e: tiles[tile].edges) {
			edge_map.add_edge(tile, e);
			edge_map.add_edge(tile, string(rbegin(e), rend(e)));
		}
	}

	// find a corner tile
	int corner = -1;

	for (int tile = 0; tile < tiles.size(); ++tile) {
		int count[] = {0, 0, 0};

		for (string e: tiles[tile].edges) {
			count[edge_map.count_edge(e)] += 1;
		}

		if (count[1] == 2 && count[2] == 2) {
			corner = tile;
			break;
		}
	}

	assert(corner != -1);

	// rotate corner tile until it can be used in the top-left spot
	int l_count = edge_map.count_edge(tiles[corner].edges[LEFT]);
	int t_count = edge_map.count_edge(tiles[corner].edges[TOP]);

	while (l_count != 1 || t_count != 1) {
		tiles[corner].rotate_left();
		l_count = edge_map.count_edge(tiles[corner].edges[LEFT]);
		t_count = edge_map.count_edge(tiles[corner].edges[TOP]);
	}

	// build the tile map
	vector<vector<int>> tile_map;

	for (int row = 0; true; ++row) {

		// find the first tile of the row
		int tile = -1;

		if (row == 0) {
			tile = corner;
		} else {
			int prev_tile = tile_map[row-1][0];
			tile = edge_map.neighbour_tile(prev_tile, tiles[prev_tile].edges[BOTTOM]);

			if (tile == -1) {
				// we're at the bottom of the map
				break;
			}

			tiles[tile].rotate_until_matches(TOP, tiles[prev_tile].edges[BOTTOM], LEFT, 1, edge_map);
		}

		tile_map.push_back({tile});

		// finish the row
		int next = edge_map.neighbour_tile(tile, tiles[tile].edges[RIGHT]);

		while (next != -1) {
			tiles[next].rotate_until_matches(LEFT, tiles[tile].edges[RIGHT], LEFT, 2, edge_map);

			tile_map[row].push_back(next);
			tile = next;

			next = edge_map.neighbour_tile(tile, tiles[tile].edges[RIGHT]);
		}
	}

#if DEBUG
	for (const auto &r : tile_map) {
		for (int t : r) {
			cout << tiles[t].id << " ";
		}
		cout << endl;
	}
#endif

	// construct the image
	int image_w = tile_map[0].size() * 8;
	int image_h = tile_map.size() * 8;

	vector<char> image(image_w * image_h);

	for (int ty = 0; ty < tile_map.size(); ++ty) {
		for (int tx = 0; tx < tile_map[0].size(); ++tx) {
			const auto &tile = tiles[tile_map[ty][tx]];
			for (int y = 0; y < 8; ++y) {
				int s = (((ty * 8) + y) * image_w) + (tx * 8);
				for (int x = 0; x < 8; ++x) {
					image[s+x] = tile.data[y+1][x+1];
				}
			}
		}
	}

#if DEBUG
	for (int y = 0; y < image_h; ++y) {
		for (int x = 0; x < image_w; ++x) {
			cout << image[y*image_w+x];
		}
		cout << endl;
	}
#endif

	// create a filter-tile
	Tile filter;
	filter.data = {FILTER[0], FILTER[1], FILTER[2]};

	int filter_elements = ranges::count(filter.data[0], '#') + ranges::count(filter.data[1], '#') + ranges::count(filter.data[2], '#');
	int matches = 0;

	for (int flip_pass = 0; matches == 0 && flip_pass < 2; ++flip_pass) {
		for (int rotate_pass = 0; matches == 0 &&  rotate_pass < 4; ++rotate_pass) {
			matches = apply_filter(image, image_w, image_h, filter);
			filter.rotate_left();
		}

		filter.flip();
	}

	cout << matches << " sea monsters found\n";

	// determine roughness
	int image_count = ranges::count(image,'#');

	cout << filter_elements << endl;
	cout << image_count << endl;

	cout << "Roughness = " << image_count - (filter_elements * matches) << endl;

	return 0;
}
