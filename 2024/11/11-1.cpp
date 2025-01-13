#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

typedef long long Stone;

static std::vector<Stone> blink(std::vector<Stone>& stones_in)
{
  std::vector<Stone> new_stones;
  for (auto it = stones_in.begin(); it != stones_in.end(); it++) {
    // If the stone is engraved with the number 0, it is replaced by a stone
    // engraved with the number 1.
    if (*it == 0) {
      Stone new_stone{ 1 };
      new_stones.push_back(new_stone);
    }
    // If the stone is engraved with a number that has an even number of digits,
    // it is replaced by two stones. The left half of the digits are engraved on
    // the new left stone, and the right half of the digits are engraved on the
    // new right stone. (The new numbers don't keep extra leading zeroes: 1000
    // would become stones 10 and 0.)
    else if (std::to_string(*it).size() % 2 == 0) {
      std::string val_str = std::to_string(*it);
      std::string left_half = val_str.substr(0, val_str.size() / 2);
      std::string right_half = val_str.substr(val_str.size() / 2);

      Stone left_stone{ std::stoi(left_half) };
      Stone right_stone{ std::stoi(right_half) };

      new_stones.push_back(left_stone);
      new_stones.push_back(right_stone);
    }
    // If none of the other rules apply, the stone is replaced by a new stone;
    // the old stone's number multiplied by 2024 is engraved on the new stone.
    else {
      new_stones.push_back({ *it * 2024 });
    }
  }
  return new_stones;
}

int main(/*int argc, char* argv[]*/)
{
  std::ios_base::sync_with_stdio(false);
  std::ifstream file_input("input");

  if (!file_input.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  static std::vector<Stone> stones;
  std::string line;
  while (file_input >> line) {
    Stone new_stone{ std::stoi(line) };
    stones.push_back(new_stone);
  }

  file_input.close();

  for (auto it = stones.begin(); it != stones.end(); it++) {
    std::cerr << *it << " ";
  }
  std::cerr << "\n";

  std::vector<Stone> cur_stones = stones;
  for (int i = 0; i < 25; i++) {
    std::cerr << "Blink " << i + 1
              << ", cur_stones.size(): " << cur_stones.size() << "\n";
    cur_stones = blink(cur_stones);
  }

  std::cerr << "\nNumber of stones after 25 blinks: " << cur_stones.size()
            << "\n\n";

  return 0;
}