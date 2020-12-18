#include <numeric>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

inline int half_bound(int b) {
	return (b - 1) / 2;
}

class Dimension {
public:
	Dimension(const vector<vector<bool>> &initial_state) {

		x_bounds = initial_state[0].size();
		y_bounds = initial_state.size();
		z_bounds = 1;
		w_bounds = 1;

		// allocate buffer that is large enough to hold final state
		x_size = x_bounds + 20;
		y_size = y_bounds + 20;
		z_size = z_bounds + 20;
		w_size = w_bounds + 20;
		data[0].resize(x_size * y_size * z_size * w_size);
		data[1].resize(x_size * y_size * z_size * w_size);

		copy_initial_state(initial_state);
	}

	uint8_t &at(int x, int y, int z, int w) {
		x += half_bound(x_size);
		y += half_bound(y_size);
		z += half_bound(z_size);
		w += half_bound(w_size);

		return data[curr][(w * (z_size * y_size * w_size)) + (z * (y_size * x_size)) + (y * x_size) + x];
	}

	uint8_t &set(int x, int y, int z, int w) {
		x += half_bound(x_size);
		y += half_bound(y_size);
		z += half_bound(z_size);
		w += half_bound(w_size);

		return data[next][(w * (z_size * y_size * w_size)) + (z * (y_size * x_size)) + (y * x_size) + x];
	}

	size_t count_active() {
		return accumulate(begin(data[curr]), end(data[curr]), 0);
	}

	size_t accumulate_neighbours(int x, int y, int z, int w) {
		size_t result = -at(x, y, z, w);

		for (int nw = w - 1; nw <= w + 1; ++nw) {
			for (int nz = z - 1; nz <= z + 1; ++nz) {
				for (int ny = y - 1; ny <= y + 1; ++ny) {
					for (int nx = x - 1; nx <= x + 1; ++nx) {
						result += at(nx, ny, nz, nw);
					}
				}
			}
		}

		return result;
	}

	void process() {

		x_bounds += 2;
		y_bounds += 2;
		z_bounds += 2;
		w_bounds += 2;

		for (int w = - half_bound(w_bounds); w <= half_bound(w_bounds); ++w) {
			for (int z = - half_bound(z_bounds); z <= half_bound(z_bounds); ++z) {
				for (int y = - half_bound(y_bounds); y <= half_bound(y_bounds); ++y) {
					for (int x = - half_bound(x_bounds); x <= half_bound(x_bounds); ++x) {
						int state = at(x, y, z, w);
						int neighbours = accumulate_neighbours(x, y, z, w);

						if (state) {
							set(x, y, z, w) = (neighbours == 2 || neighbours == 3);
						} else {
							set(x, y, z, w) = neighbours == 3;
						}
					}
				}
			}
		}

		swap(curr, next);
	}

private:
	void copy_initial_state(const vector<vector<bool>> &initial_state) {
		int hw = half_bound(initial_state[0].size());
		int hh = half_bound(initial_state.size());

		for (int y = 0; y < initial_state.size(); ++y) {
			for (int x = 0; x < initial_state[y].size(); ++x) {
				at(x - hw, y - hh, 0, 0) = initial_state[y][x];
			}
		}
	}

private:
	int x_size;
	int y_size;
	int z_size;
	int w_size;

	int x_bounds;
	int y_bounds;
	int z_bounds;
	int w_bounds;

	vector<uint8_t> data[2];

	int curr = 0;
	int next = 1;
};

vector<vector<bool>> read_input() {
	string line;

	vector<vector<bool>> result;

	while (getline(cin, line)) {
		result.push_back({});
		transform(begin(line), end(line), back_inserter(result.back()), [](auto c) { return c == '#'; });
	}

	return move(result);
}

int main() {

	// read input
	auto initial_state = read_input();

	Dimension pocket_dimension(initial_state);

	for (int i = 0; i < 6; ++i) {
		pocket_dimension.process();
		cout << pocket_dimension.count_active() << endl;
	}

	return 0;
}
