#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <sstream>
#include <map>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

struct FoodItem {
	vector<string>	ingredients;
	vector<string>	allergens;
};

vector<FoodItem> load_dataset() {

	vector<FoodItem> result;

	string str_data;
	bool reading_allergens = false;

	FoodItem food;

	while (cin >> str_data) {
		if (!reading_allergens) {
			if (str_data == "(contains") {
				reading_allergens = true;
			} else {
				food.ingredients.push_back(str_data);
			}
		} else {
			bool complete = str_data.ends_with(')');

			if (complete || str_data.ends_with(',')) {
				food.allergens.push_back(str_data.substr(0, str_data.size() - 1));
			} else {
				food.allergens.push_back(str_data);
			}

			if (complete) {
				ranges::sort(food.ingredients);
				result.push_back(move(food));
				food = {};
				reading_allergens = false;
			}
		}
	}

	return result;
}


int main() {

	auto dataset = load_dataset();

	// construct a map with the intersection of all ingredients for an allergen
	map<string, vector<string>> allergen_map;

	for (const auto &food : dataset) {
		for (const auto &a : food.allergens) {
			if (!allergen_map.contains(a)) {
				allergen_map[a] = food.ingredients;
			} else {
				auto current = allergen_map[a];
				allergen_map[a].clear();

				set_intersection(begin(current), end(current), begin(food.ingredients), end(food.ingredients),
								 back_inserter(allergen_map[a]));
			}
		}
	}


	// reduce the ingredient list per allergen until only 1 is left
	vector<string> incomplete_allergens;
	ranges::transform(allergen_map, back_inserter(incomplete_allergens), [](const auto &x) {return x.first;});

	while (!incomplete_allergens.empty()) {
		auto iter = begin(incomplete_allergens);

		while (iter != end(incomplete_allergens)) {

			if (allergen_map[*iter].size() == 1) {
				// remove ingredient from all other allergens
				string ingred = allergen_map[*iter].front();

				for (auto &am : allergen_map) {
					if (am.first != *iter) {
						am.second.erase(remove(begin(am.second), end(am.second), ingred), end(am.second));
					}
				}

				// remove from incomplete_allergens
				iter = incomplete_allergens.erase(iter);
			} else {
				++iter;
			}
		}
	}

	// build a list of dangerous ingredients
	string separator = "";

	for (const auto &am : allergen_map) {
		cout << separator << am.second.front();
		separator = ",";
	}
	cout << endl;


	return 0;
}
