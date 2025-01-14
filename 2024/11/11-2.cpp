#include <fstream>
#include <iostream>
#include <list>
#include <ostream>
#include <string>
#include <unordered_map>

typedef long long Stone;

// Custom add function to only allocate more memory if the stone is unique
void add_stone(std::unordered_map<Stone, long long>& stone_map, Stone& stone,
               long long number)
{
  if (stone_map.count(stone) != 0) {
    stone_map.at(stone) += number;
  }
  else {
    stone_map.insert({ stone, number });
  }
}

int main(/*int argc, char* argv[]*/)
{
  std::ios_base::sync_with_stdio(false);
  std::ifstream file_input("input");

  if (!file_input.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  static std::list<Stone> input;
  std::string line;
  while (file_input >> line) {
    Stone new_stone{ std::stoi(line) };
    input.push_back(new_stone);
  }

  file_input.close();

  for (auto it = input.begin(); it != input.end(); it++) {
    std::cerr << *it << " ";
  }
  std::cerr << "\n";

  // Create dictionary with stones and their counts to avoid creating individual
  // memory space for each stone.
  std::unordered_map<Stone, long long> dict;
  for (auto stone : input) {
    add_stone(dict, stone, 1);
  }

  for (int i = 0; i < 75; i++) {

    std::unordered_map<Stone, long long> new_dict;
    for (auto it = dict.begin(); it != dict.end(); it++) {
      // If the stone is engraved with the number 0, it is replaced by a stone
      // engraved with the number 1.
      if (it->first == 0) {
        Stone new_stone{ 1 };
        add_stone(new_dict, new_stone, it->second);
      }
      // If the stone is engraved with a number that has an even number of
      // digits, it is replaced by two stones. The left half of the digits are
      // engraved on the new left stone, and the right half of the digits are
      // engraved on the new right stone. (The new numbers don't keep extra
      // leading zeroes: 1000 would become stones 10 and 0.)
      else if (std::to_string(it->first).size() % 2 == 0) {
        std::string val_str = std::to_string(it->first);
        std::string left_half = val_str.substr(0, val_str.size() / 2);
        std::string right_half = val_str.substr(val_str.size() / 2);

        Stone left_stone{ std::stoi(left_half) };
        Stone right_stone{ std::stoi(right_half) };

        add_stone(new_dict, left_stone, it->second);
        add_stone(new_dict, right_stone, it->second);
      }
      // If none of the other rules apply, the stone is replaced by a new stone;
      // the old stone's number multiplied by 2024 is engraved on the new stone.
      else {
        Stone new_stone{ it->first * 2024 };
        add_stone(new_dict, new_stone, it->second);
      }
    }
    dict = new_dict;
  }

  long long sum_stones = 0;
  for (auto stone : dict) {
    sum_stones += stone.second;
  }

  std::cerr << "\nNumber of stones after 25 blinks: " << sum_stones << "\n\n";

  return 0;
}
