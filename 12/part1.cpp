#include <array>
#include <iostream>

using namespace std;

using Point2 = array<int64_t, 2>;

constexpr Point2 headings[] = {
	{1, 0},
	{0, 1},
	{-1, 0},
	{0, -1}
};

int main() {
	Point2 position = {0,0};
	int heading = 0;

	char cmd;
	int64_t value;

	while (cin >> cmd >> value) {
		switch (cmd) {
			case 'N':
				position[1] -= value;
				break;
			case 'S':
				position[1] += value;
				break;
			case 'E':
				position[0] += value;
				break;
			case 'W':
				position[0] -= value;
				break;
			case 'L': {
				int delta = (value / 90) % 4;
				heading = (heading + 4 - delta) % 4;
				break;
			}
			case 'R': {
				int delta = (value / 90) % 4;
				heading = (heading + delta) % 4;
				break;
			}
			case 'F':
				position[0] += headings[heading][0] * value;
				position[1] += headings[heading][1] * value;
				break;
			default:
				cerr << "Invalid command " << cmd << value << endl;
				return -1;
		}
	}

	cout << abs(position[0]) + abs(position[1]) << endl;

	return 0;
}
