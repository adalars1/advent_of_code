#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

static int count_xmas_words = 0;

std::vector<std::string> word_grid;
std::vector<std::string> word_grid_kills;

struct Letter_Coordinate {
  int x;
  int y;
};

bool coord_is_safe(Letter_Coordinate coord)
{
  if (coord.y < 0) {
    return false;
  }
  if (coord.y >= word_grid.size()) {
    return false;
  }
  if (coord.x < 0) {
    return false;
  }
  if (coord.x >= 140) {
    return false;
  }

  return true;
}

bool coords_are_safe(Letter_Coordinate coord_0, Letter_Coordinate coord_1,
                     Letter_Coordinate coord_2, Letter_Coordinate coord_3,
                     Letter_Coordinate coord_4)
{
  return (coord_is_safe(coord_0) && coord_is_safe(coord_1) &&
          coord_is_safe(coord_2) && coord_is_safe(coord_3) &&
          coord_is_safe(coord_4));
}

bool check_for_xmas(Letter_Coordinate middle, Letter_Coordinate up_left,
                    Letter_Coordinate down_left, Letter_Coordinate up_right,
                    Letter_Coordinate down_right, char fill)
{
  bool xmas_found = false;

  if (coords_are_safe(middle, up_left, down_left, up_right, down_right)) {
    auto middle_char = word_grid.at(middle.y).at(middle.x);
    auto up_left_char = word_grid.at(up_left.y).at(up_left.x);
    auto down_left_char = word_grid.at(down_left.y).at(down_left.x);
    auto up_right_char = word_grid.at(up_right.y).at(up_right.x);
    auto down_right_char = word_grid.at(down_right.y).at(down_right.x);

    if (middle_char == 'A') {
      if (up_left_char == 'M' && up_right_char == 'M') {
        if (down_left_char == 'S' && down_right_char == 'S') {
          count_xmas_words++;
          xmas_found = true;
        }
      }
      else if (up_left_char == 'S' && up_right_char == 'S') {
        if (down_left_char == 'M' && down_right_char == 'M') {
          count_xmas_words++;
          xmas_found = true;
        }
      }
      else if (up_left_char == 'M' && up_right_char == 'S') {
        if (down_left_char == 'M' && down_right_char == 'S') {
          count_xmas_words++;
          xmas_found = true;
        }
      }
      else if (up_left_char == 'S' && up_right_char == 'M') {
        if (down_left_char == 'S' && down_right_char == 'M') {
          count_xmas_words++;
          xmas_found = true;
        }
      }
    }
  }

  if (xmas_found) {
    word_grid_kills.at(middle.y).at(middle.x) = fill;
    word_grid_kills.at(up_left.y).at(up_left.x) = fill;
    word_grid_kills.at(down_left.y).at(down_left.x) = fill;
    word_grid_kills.at(up_right.y).at(up_right.x) = fill;
    word_grid_kills.at(down_right.y).at(down_right.x) = fill;
  }
  return xmas_found;
}

int main(int argc, char* argv[])
{
  std::ifstream file("input");
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  while (getline(file, line, '\n')) {
    word_grid.push_back(line);
    word_grid_kills.push_back(line);
  }

  file.close();

  for (auto row_cur_idx = 0; row_cur_idx < word_grid.size(); row_cur_idx++) {
    std::cout << word_grid.at(row_cur_idx) << std::endl;

    for (auto col_cur_idx = 0; col_cur_idx < word_grid.at(row_cur_idx).size();
         col_cur_idx++) {
      auto col_left = col_cur_idx - 1;
      auto col_right = col_cur_idx + 1;
      auto row_down = row_cur_idx + 1;
      auto row_up = row_cur_idx - 1;

      // diagonal down left search
      Letter_Coordinate center = { col_cur_idx, row_cur_idx };
      Letter_Coordinate up_left = { col_left, row_up };
      Letter_Coordinate down_left = { col_left, row_down };
      Letter_Coordinate up_right = { col_right, row_up };
      Letter_Coordinate down_right = { col_right, row_down };
      if (check_for_xmas(center, up_left, down_left, up_right, down_right,
                         ' ')) {
      }

    }  // for cols (x)

  }  // for lines (y)

  std::cout << "\n######## Line kills ########\n\n";
  for (auto line : word_grid_kills) {
    std::cout << line << std::endl;
  }

  std::cout << "Count of X-MAS words: " << count_xmas_words << std::endl;
  return 0;
}