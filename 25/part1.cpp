#include <cassert>
#include <iostream>
#include <string>

using namespace std;

int64_t encryption_key(int64_t subject, int loop) {

	int64_t value = 1;
	for (int i = 0; i < loop; ++i) {
		value *= subject;
		value %= 20201227;
	}

	return value;
}

int determine_loop_size(int64_t public_key) {

	int64_t subject = 7;
	int64_t value = 1;
	int loop = 0;

	while (value != public_key) {
		value *= subject;
		value %= 20201227;
		loop += 1;
	}

	assert(value == public_key);
	return loop;
}

int main() {
	int64_t pub_keys[2];
	cin >> pub_keys[0] >> pub_keys[1];

	// cout << pub_keys[0] << " -- " << pub_keys[1] << endl;
	int loop[2] = {
		determine_loop_size(pub_keys[0]),
		determine_loop_size(pub_keys[1])
	};

	cout << pub_keys[0] << " loop = " << loop[0] << endl;
	cout << pub_keys[1] << " loop = " << loop[1] << endl;

	cout << "Key: " << encryption_key(pub_keys[0], loop[1]) << endl;
	cout << "Key: " << encryption_key(pub_keys[1], loop[0]) << endl;


	return 0;
}
