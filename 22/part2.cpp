#include <array>
#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>

using namespace std;
namespace ranges = std::ranges;

class Game {
public:
	Game(deque<int> player_1, deque<int> player_2) : game_id(++s_game_id), players({player_1, player_2}) {}

	int play() {
		#ifdef DEBUG
			cout << "\n=== Game " << game_id << " ===\n";
		#endif

		int winner = -1;

		while (winner == -1 && !players[0].empty() && !players[1].empty()) {
			// check history
			auto repeat = ranges::find(history, players);
			if (repeat != end(history)) {
				winner = 0;
				break;
			}
			history.push_back(players);

			process_round();

		}

		if (winner < 0) {
			winner = (players[0].empty()) ? 1 : 0;
		}

		#ifdef DEBUG
			cout << "The winner of game " << game_id << " is player " << winner + 1 << "!\n";
			if (game_id == 1) {
				cout << "\n== Post-game results ==\n";
				print_deck(0);
				print_deck(1);
			}
		#endif

		return winner;
	}

	void process_round() {
		round += 1;

		#ifdef DEBUG
			cout << "\n-- Round " << round << " (Game " << game_id <<  ") --\n";
			print_deck(0);
			print_deck(1);
			cout << "Player 1 plays: " << players[0].front() << "\n";
			cout << "Player 2 plays: " << players[1].front() << "\n";
		#endif

		int card_0 = players[0].front();
		int card_1 = players[1].front();
		players[0].pop_front();
		players[1].pop_front();

		int winner = -1;

		if (players[0].size() >= card_0 && players[1].size() >= card_1) {
			#ifdef DEBUG
				cout << "Playing a sub-game to determine the winner...\n";
			#endif

			Game sub_game(deque(begin(players[0]), begin(players[0]) + card_0),
						  deque(begin(players[1]), begin(players[1]) + card_1));

			winner = sub_game.play();

		} else if (card_0 > card_1) {
			winner = 0;
		} else {
			winner = 1;
		}

		#ifdef DEBUG
			cout << "Player " << winner + 1 << " wins round " << round << " of game " << game_id << "!\n";
		#endif

		if (winner == 0) {
			players[0].push_back(card_0);
			players[0].push_back(card_1);
		} else {
			players[1].push_back(card_1);
			players[1].push_back(card_0);
		}
	}

	int score(int winner) const {
		return accumulate(rbegin(players[winner]), rend(players[winner]), 0,
						  [factor=1](int a, int b) mutable { return a + (b * factor++);});
	}

	void print_deck(int player) const {
		cout << "Player " << player + 1 << "'s deck: ";

		string delim = "";
		for (auto card : players[player]) {
			cout << delim << card;
			delim = ", ";
		}
		cout << "\n";
	}

private:
	using deck_t = array<deque<int>, 2>;

	static int s_game_id;

	int				game_id;
	int				round = 0;
	deck_t			players;
	vector<deck_t>	history;
};

int Game::s_game_id = 0;

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
	Game game(players[0], players[1]);

	int winner = game.play();

	cout << "Winning score = " << game.score(winner) << "\n";;

	return 0;
}
