#include <array>
#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>

using namespace std;
namespace ranges = std::ranges;

static inline void print_deck(int player, const deque<int> &deck) {
	cout << "Player " << player << "'s deck: ";

	string delim = "";
	for (auto card : deck) {
		cout << delim << card;
		delim = ", ";
	}
	cout << "\n";
}

int main() {

	array<deque<int>, 2> players;

	// load data
	auto read_player = [&](int p) {
		string line;
		getline(cin, line);
		while (getline(cin, line) && !line.empty()) {
			players[p].push_back(stoi(line));
		}
	};

	read_player(0);
	read_player(1);

	// process
	int round = 0;

	while (!players[0].empty() && !players[1].empty()) {
		round += 1;

		#ifdef DEBUG
			cout << "\n-- Round " << round << " --\n";
			print_deck(1, players[0]);
			print_deck(2, players[1]);
			cout << "Player 1 plays: " << players[0].front() << "\n";
			cout << "Player 2 plays: " << players[1].front() << "\n";
		#endif

		if (players[0].front() > players[1].front()) {
			#ifdef DEBUG
				cout << "Player 1 wins the round!\n";
			#endif

			players[0].push_back(players[0].front());
			players[0].push_back(players[1].front());
		} else {
			#ifdef DEBUG
				cout << "Player 2 wins the round!\n";
			#endif

			players[1].push_back(players[1].front());
			players[1].push_back(players[0].front());
		}

		players[0].pop_front();
		players[1].pop_front();
	}

	#ifdef DEBUG
		cout << "\n== Post-game results ==\n";
		print_deck(1, players[0]);
		print_deck(2, players[1]);
	#endif

	int winner = (players[0].empty()) ? 1 : 0;

	cout << "Winning score = " << accumulate(rbegin(players[winner]), rend(players[winner]), 0,
											 [factor=1](int a, int b) mutable { return a + (b * factor++);})
		 << "\n";

	return 0;
}
