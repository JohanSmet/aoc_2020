#include <array>
#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>

using namespace std;
namespace ranges = std::ranges;

class Board {
public:
	Board(const string &input, size_t size = 0) {
		size = max(size, input.size());

		cups.resize(size + 1);

		max_cup = 0;
		int prev = 0;

		for (size_t i = 0; i < input.size(); ++i) {
			cups[prev] = input[i] - '0';
			max_cup = max(max_cup, cups[prev]);
			prev = cups[prev];
		}

		for (size_t i = input.size(); i < size; ++i) {
			cups[prev] = ++max_cup;
			prev = max_cup;
		}

		cups[prev] = cups[0];
	}

	void play(int moves) {
		for (int i = 0; i < moves; ++i) {
			process_move();
		}

		#ifdef DEBUG
			cout << "-- final --\n";
			print_cups();
		#endif
	}

	void process_move() {
		turn += 1;

		#ifdef DEBUG
			cout << "-- move " << turn << " --\n";
			print_cups();
		#endif

		int current = cups[0];

		// choose cups to pick up
		array<int, 3> pickup;
		pickup[0] = cups[current];
		pickup[1] = cups[pickup[0]];
		pickup[2] = cups[pickup[1]];

		// remove cups
		cups[current] = cups[pickup[2]];

		#ifdef DEBUG
			cout << "pick up: " << pickup[0] << ", " << pickup[1] << ", " << pickup[2] << "\n";
		#endif

		// destination
		int destination = (current > 1) ? current - 1 : max_cup;

		while (ranges::find(pickup, destination) != end(pickup)) {
			destination = (destination > 1) ? destination - 1 : max_cup;
		}

		#ifdef DEBUG
			cout << "destination: " << destination << "\n\n";
		#endif

		// place cups back
		cups[pickup[2]] = cups[destination];
		cups[destination] = pickup[0];

		// new current cup
		cups[0] = cups[cups[0]];
	}

	void print_cups() const {
		cout << "cups: (" << cups[0] << ")";

		for (int cup = cups[cups[0]]; cup != cups[0]; cup = cups[cup]) {
			cout << " " << cup;
		}

		cout << endl;
	}

	vector<int>	cups;
	int			turn = 0;
	int			max_cup = 0;
};

int main() {
	string input;
	cin >> input;

	Board	board(input, 1000000);
	board.play(10000000);

	uint64_t cup_1 = board.cups[1];
	uint64_t cup_2 = board.cups[cup_1];
	cout << cup_1 << " * " << cup_2 << " = " << (cup_1 * cup_2) << endl;

	return 0;
}
