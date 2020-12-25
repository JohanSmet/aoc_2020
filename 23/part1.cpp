#include <array>
#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>

using namespace std;
namespace ranges = std::ranges;


class Board {
public:
	Board() = default;

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

		// pickup
		string pickup;
		array<int, 3> to_erase;

		for (int i = 1; i <= 3; ++i) {
			int idx = (current + i) % cups.size();
			pickup.append(1, cups[idx]);
			to_erase[i-1] = idx;
		}

		char current_char = cups[current];

		ranges::sort(to_erase);
		for (int i = 2; i >= 0; --i) {
			cups.erase(to_erase[i], 1);
		}
		current = cups.find(current_char);

		#ifdef DEBUG
			cout << "pick up: " << pickup[0] << ", " << pickup[1] << ", " << pickup[2] << "\n";
		#endif

		// destination
		string sorted = cups;
		ranges::sort(sorted);

		int destination = sorted.find(current_char);
		destination = (destination - 1 + sorted.size()) % sorted.size();
		char dest_char = sorted[destination];
		destination = cups.find(dest_char);

		#ifdef DEBUG
			cout << "destination: " << cups[destination] << "\n\n";
		#endif

		// place cups back
		cups.insert(destination + 1, pickup);

		// new current cup
		current = cups.find(current_char);
		current = (current + 1) % cups.size();
	}

	void print_cups() const {
		cout << "cups:";

		for (int i = 0; i < cups.size(); ++i) {
			if (current == i) {
				cout << " (" << cups[i] << ")";
			} else {
				cout << " " << cups[i];
			}
		}
		cout << "\n";
	}

	string labels_after_one() const {
		string result = "";

		int pos = cups.find('1');

		for (int i = 0; i < cups.size() - 1; ++i) {
			pos = (pos + 1) % cups.size();
			result.push_back(cups[pos]);
		}

		return result;
	}

	string	cups;
	int		current = 0;
	int		turn = 0;
};

int main() {

	Board	board;
	cin >> board.cups;

	board.play(100);
	cout << board.labels_after_one() << "\n";



	return 0;
}
