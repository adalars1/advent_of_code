#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ostream>
#include <queue>
#include <string>
#include <vector>

struct Coordinate {
  int x;
  int y;
  int height;
  std::string height_str;

  friend std::ostream& operator<<(std::ostream& os, const Coordinate& c)
  {
    os << ".x = " << c.x << " .y = " << c.y << " height: " << c.height_str;
    return os;
  }

  friend bool operator==(Coordinate lhs, const Coordinate& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.height == rhs.height;
  }
};

int find_trails(std::vector<std::vector<Coordinate>>& vvc, Coordinate& start)
{
  int rating = 0;

  std::queue<Coordinate> queue;
  queue.push(start);

  while (!queue.empty()) {
    Coordinate cur_coord = queue.front();
    queue.pop();

    if (cur_coord.height == 9) {
      ++rating;
    }

    else {
      std::vector<Coordinate> next_coords;
      int x_left = cur_coord.x - 1;
      int x_right = cur_coord.x + 1;
      int y_up = cur_coord.y - 1;
      int y_down = cur_coord.y + 1;

      if (y_up >= 0) {
        next_coords.push_back(vvc[y_up][cur_coord.x]);
      }
      if (y_down < static_cast<int>(vvc.size())) {
        next_coords.push_back(vvc[y_down][cur_coord.x]);
      }
      if (x_right < static_cast<int>(vvc.size())) {
        next_coords.push_back(vvc[cur_coord.y][x_right]);
      }
      if (x_left >= 0) {
        next_coords.push_back(vvc[cur_coord.y][x_left]);
      }

      for (auto next_coord : next_coords) {
        if (next_coord.height == (cur_coord.height + 1)) {
          queue.push(next_coord);
        }
      }
    }
  }

  return rating;
}

int main(/*int argc, char* argv[]*/)
{
  std::ios_base::sync_with_stdio(false);
  std::ifstream file_input("input");

  if (!file_input.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  std::vector<std::vector<Coordinate>> coords;

  std::string line;
  int cur_x = 0, cur_y = 0;
  while (getline(file_input, line, '\n')) {
    std::vector<Coordinate> cur_coord_line;
    for (auto c : line) {
      int height;
      if (std::isdigit(c)) {
        height = c - '0';
      }
      else {
        height = 99;
      }
      cur_coord_line.push_back({ cur_x, cur_y, height, std::string(1, c) });
      ++cur_x;
    }
    coords.push_back(cur_coord_line);
    cur_x = 0;
    ++cur_y;
  }

  file_input.close();

  unsigned long long sum_trails = 0;
  for (auto row_it = coords.begin(); row_it != coords.end(); row_it++) {
    for (auto col_it = row_it->begin(); col_it != row_it->end(); col_it++) {
      if (col_it->height == 0) {
        sum_trails += find_trails(coords, *col_it);
      }
    }
  }

  std::cout << "\nSum of trailhead ratings: " << sum_trails << "\n\n";

  return 0;
}