#include <array>
#include <iostream>

using namespace std;

using Point2 = array<int64_t, 2>;

constexpr int64_t sin_lut[4] = {
	0,		// 0 degrees
	1,		// 90 degrees
	0,		// 180 degrees
	-1		// 270 degress
};

constexpr int64_t cos_lut[4] = {
	1,		// 0 degrees
	0,		// 90 degrees
	-1,		// 180 degrees
	0,		// 270 degress
};

Point2 rotate_right(Point2 p, int angle) {
	int i = ((angle / 90) + 4) % 4;
	return Point2 {
		p[0] * cos_lut[i] - p[1] * sin_lut[i],
		p[0] * sin_lut[i] + p[1] * cos_lut[i]
	};
}

int main() {
	Point2 position = {0,0};
	Point2 waypoint = {10, -1};

	char cmd;
	int64_t value;

	while (cin >> cmd >> value) {
		switch (cmd) {
			case 'N':
				waypoint[1] -= value;
				break;
			case 'S':
				waypoint[1] += value;
				break;
			case 'E':
				waypoint[0] += value;
				break;
			case 'W':
				waypoint[0] -= value;
				break;
			case 'L':
				waypoint = rotate_right(waypoint, 360 - value);
				break;
			case 'R':
				waypoint = rotate_right(waypoint, value);
				break;
			case 'F':
				position[0] += waypoint[0] * value;
				position[1] += waypoint[1] * value;
				break;
			default:
				cerr << "Invalid command " << cmd << value << endl;
				return -1;
		}
	}

	cout << abs(position[0]) + abs(position[1]) << endl;

	return 0;
}
