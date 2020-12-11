#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

enum class Cell {
	Floor = 0,
	Empty = 1,
	Taken = 2
};

using Map = vector<vector<Cell>>;

void read_map(Map &map) {
	string line;

	while (getline(cin, line)) {
		vector<Cell>  row;

		ranges::transform(line, back_inserter(row),
						  [](auto c) -> Cell {
							return (c == 'L') ? Cell::Empty : Cell::Floor;
						  });
		map.emplace_back(row);
	}
}

inline bool is_valid_pos(const Map &map, int row, int col) {
	return (row >= 0 && row < map.size() && col >= 0 && col < map[row].size());
}

inline bool trace_occupied(const Map& map, int r, int c, int dr, int dc) {

	r += dr;
	c += dc;

	while (is_valid_pos(map, r, c)) {
		if (map[r][c] != Cell::Floor) {
			return map[r][c] == Cell::Taken;
		}

		r += dr;
		c += dc;
	}

	return false;
}

int count_adjacent_occupied(const Map &map, int row, int col) {

	int result = trace_occupied(map, row, col, 0, 1);
	result += trace_occupied(map, row, col, 1, 1);
	result += trace_occupied(map, row, col, 1, 0);
	result += trace_occupied(map, row, col, 1, -1);
	result += trace_occupied(map, row, col, 0, -1);
	result += trace_occupied(map, row, col, -1, -1);
	result += trace_occupied(map, row, col, -1, 0);
	result += trace_occupied(map, row, col, -1, 1);

	return result;
}

int occupied_count(const Map &map) {
	int result = 0;

	for (auto &row: map) {
		result += ranges::count(row, Cell::Taken);
	}

	return result;
}

void process_map(const Map &input, Map &output) {
	output.resize(input.size());

	for (int row = 0; row < input.size(); ++row) {
		output[row].resize(input[row].size());
		for (int col = 0; col < input[row].size(); ++col) {
			output[row][col] = input[row][col];

			int adjacent_count = count_adjacent_occupied(input, row, col);

			if (input[row][col] == Cell::Empty && adjacent_count == 0) {
				output[row][col] = Cell::Taken;
			} else if (input[row][col] == Cell::Taken && adjacent_count >= 5) {
				output[row][col] = Cell::Empty;
			}
		}
	}
}

void print_map(const Map &map) {

	static constexpr char SYMBOL[] = {'.', 'L', '#'};

	for (auto &row: map) {
		for (auto c : row) {
			cout << SYMBOL[static_cast<int> (c)];
		}
		cout << endl;
	}

	cout << endl;

}

int main() {
	Map map[2];;
	read_map(map[0]);

	int a = 0, b = 1;
	int prev_count = -1;
	int count = 0;

	while (prev_count != count) {
		process_map(map[a], map[b]);
		swap(a, b);

		prev_count = count;
		count = occupied_count(map[a]);
	}

	cout << count << endl;

	return 0;
}
