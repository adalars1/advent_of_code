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
                     Letter_Coordinate coord_2, Letter_Coordinate coord_3)
{
  return (coord_is_safe(coord_0) && coord_is_safe(coord_1) &&
          coord_is_safe(coord_2) && coord_is_safe(coord_3));
}

bool check_for_xmas(Letter_Coordinate lc0, Letter_Coordinate lc1,
                    Letter_Coordinate lc2, Letter_Coordinate lc3, char fill)
{
  bool xmas_found = false;

  if (coords_are_safe(lc0, lc1, lc2, lc3)) {
    auto c0 = word_grid.at(lc0.y).at(lc0.x);
    auto c1 = word_grid.at(lc1.y).at(lc1.x);
    auto c2 = word_grid.at(lc2.y).at(lc2.x);
    auto c3 = word_grid.at(lc3.y).at(lc3.x);

    if (c0 == 'X') {
      if (c1 == 'M') {
        if (c2 == 'A') {
          if (c3 == 'S') {
            count_xmas_words++;
            xmas_found = true;
            word_grid_kills.at(lc0.y).at(lc0.x) = fill;
            word_grid_kills.at(lc1.y).at(lc1.x) = fill;
            word_grid_kills.at(lc2.y).at(lc2.x) = fill;
            word_grid_kills.at(lc3.y).at(lc3.x) = fill;
          }
        }
      }
    }

    else if (c0 == 'S') {
      if (c1 == 'A') {
        if (c2 == 'M') {
          if (c3 == 'X') {
            count_xmas_words++;
            xmas_found = true;
            word_grid_kills.at(lc0.y).at(lc0.x) = fill;
            word_grid_kills.at(lc1.y).at(lc1.x) = fill;
            word_grid_kills.at(lc2.y).at(lc2.x) = fill;
            word_grid_kills.at(lc3.y).at(lc3.x) = fill;
          }
        }
      }
    }
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

  int vert_found = 0;
  int horiz_found = 0;
  int down_right_found = 0;
  int down_left_found = 0;

  for (auto row_cur_idx = 0; row_cur_idx < word_grid.size(); row_cur_idx++) {
    std::cout << word_grid.at(row_cur_idx) << std::endl;

    for (auto col_cur_idx = 0; col_cur_idx < word_grid.at(row_cur_idx).size();
         col_cur_idx++) {
      auto col_right_1 = col_cur_idx + 1;
      auto col_right_2 = col_cur_idx + 2;
      auto col_right_3 = col_cur_idx + 3;

      auto row_down_1 = row_cur_idx + 1;
      auto row_down_2 = row_cur_idx + 2;
      auto row_down_3 = row_cur_idx + 3;

      auto col_left_1 = col_cur_idx - 1;
      auto col_left_2 = col_cur_idx - 2;
      auto col_left_3 = col_cur_idx - 3;

      auto row_up_1 = row_cur_idx - 1;
      auto row_up_2 = row_cur_idx - 2;
      auto row_up_3 = row_cur_idx - 3;

      // horizontal search
      Letter_Coordinate horiz_0 = { col_cur_idx, row_cur_idx };
      Letter_Coordinate horiz_1 = { col_right_1, row_cur_idx };
      Letter_Coordinate horiz_2 = { col_right_2, row_cur_idx };
      Letter_Coordinate horiz_3 = { col_right_3, row_cur_idx };
      if (check_for_xmas(horiz_0, horiz_1, horiz_2, horiz_3, '>')) {
        horiz_found++;
      }

      // vertical search
      Letter_Coordinate vertical_0 = { col_cur_idx, row_cur_idx };
      Letter_Coordinate vertical_1 = { col_cur_idx, row_down_1 };
      Letter_Coordinate vertical_2 = { col_cur_idx, row_down_2 };
      Letter_Coordinate vertical_3 = { col_cur_idx, row_down_3 };
      if (check_for_xmas(vertical_0, vertical_1, vertical_2, vertical_3, 'v')) {
        vert_found++;
      }

      // diagonal down right search
      Letter_Coordinate down_right_0 = { col_cur_idx, row_cur_idx };
      Letter_Coordinate down_right_1 = { col_right_1, row_down_1 };
      Letter_Coordinate down_right_2 = { col_right_2, row_down_2 };
      Letter_Coordinate down_right_3 = { col_right_3, row_down_3 };
      if (check_for_xmas(down_right_0, down_right_1, down_right_2, down_right_3,
                         'D')) {
        down_right_found++;
      }

      // diagonal down left search
      Letter_Coordinate down_left_0 = { col_cur_idx, row_cur_idx };
      Letter_Coordinate down_left_1 = { col_left_1, row_down_1 };
      Letter_Coordinate down_left_2 = { col_left_2, row_down_2 };
      Letter_Coordinate down_left_3 = { col_left_3, row_down_3 };
      if (check_for_xmas(down_left_0, down_left_1, down_left_2, down_left_3,
                         'd')) {
        down_left_found++;
      }

    }  // for cols (x)

  }  // for lines (y)

  std::cout << "\n######## Line kills ########\n\n";
  for (auto line : word_grid_kills) {
    std::cout << line << std::endl;
  }

  std::cout << "Count of XMAS words: " << count_xmas_words << std::endl;

  std::cout << "vert_found: " << vert_found << std::endl
            << "horiz_found: " << horiz_found << std::endl
            << "down_right_found: " << down_right_found << std::endl
            << "down_left_found: " << down_left_found << std::endl;

  return 0;
}