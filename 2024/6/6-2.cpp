#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum Direction { NORTH, SOUTH, WEST, EAST };

struct Position {
  int x;
  int y;
};

struct Visited_Position {
  int x;
  int y;
  Direction d;
};

static int PLAY_AREA_WIDTH = 0;
static int PLAY_AREA_HEIGHT = 0;

static std::vector<std::string> input_play_area;

bool is_outside_play_area(Position& p)
{
  if ((p.x >= 0 && p.x <= PLAY_AREA_WIDTH) &&
      (p.y >= 0 && p.y <= PLAY_AREA_HEIGHT)) {
    return false;
  }
  else {
    return true;
  }
}

Position get_next_position(Position& p, Direction& d)
{
  Position res;
  res.x = p.x;
  res.y = p.y;
  switch (d) {
    case NORTH:
      res.y -= 1;
      break;
    case SOUTH:
      res.y += 1;
      break;
    case WEST:
      res.x -= 1;
      break;
    case EAST:
      res.x += 1;
      break;
  }
  return res;
}

void turn(Direction& d)
{
  switch (d) {
    case NORTH:
      d = EAST;
      break;
    case SOUTH:
      d = WEST;
      break;
    case WEST:
      d = NORTH;
      break;
    case EAST:
      d = SOUTH;
      break;
  }
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
    input_play_area.push_back(line);
  }

  file.close();

  // Find starting position for guard and play area size
  Position guard_start;
  for (auto row = 0; row < input_play_area.size(); row++) {
    if (input_play_area.at(row).size() > PLAY_AREA_HEIGHT) {
      PLAY_AREA_HEIGHT = input_play_area.at(row).size() - 1;
    }
    for (auto col = 0; col < input_play_area.at(row).size(); col++) {
      if (col > PLAY_AREA_WIDTH) {
        PLAY_AREA_WIDTH = col;
      }
      if (input_play_area.at(row).at(col) == '^') {
        guard_start.x = col;
        guard_start.y = row;
      }
    }
  }

  Position guard_pos = guard_start;
  Direction direction = NORTH;

  // Find all walked positions to mark them as possible spots to place an
  // obstruction at.
  int steps = 0;
  std::vector<Position> possible_obstructions;
  while (!is_outside_play_area(guard_pos)) {
    Position next = get_next_position(guard_pos, direction);

    if (is_outside_play_area(next)) {
      break;
    }

    auto next_tile = input_play_area.at(next.y).at(next.x);
    if (next_tile == '#') {
      turn(direction);
    }
    else {
      guard_pos = next;
      ++steps;
      bool unique_obstruction = true;
      for (auto it = possible_obstructions.begin();
           it != possible_obstructions.end(); it++) {
        if ((it->x == guard_pos.x) && (it->y == guard_pos.y)) {
          unique_obstruction = false;
        }
      }
      if (unique_obstruction) {
        possible_obstructions.push_back(guard_pos);
      }
    }
  }

  // Try placing an obstruction and walking the guard.
  // Store the visited positions and check if the next step was
  // already walked on in the same direction.
  // If it was, the guard is stuck in a loop.
  int times_guard_stuck = 0;
  for (auto obstruction : possible_obstructions) {
    std::vector<Visited_Position> visited;
    guard_pos = guard_start;
    direction = NORTH;
    visited.clear();
    visited.push_back({ guard_pos.x, guard_pos.y, direction });

    while (true) {
      Position next = get_next_position(guard_pos, direction);
      if (is_outside_play_area(next)) {
        break;
      }

      bool next_in_visited = false;
      for (auto it = visited.begin(); it != visited.end(); it++) {
        if ((it->x == next.x) && (it->y == next.y) && (it->d == direction)) {
          next_in_visited = true;
          break;
        }
      }

      if (next_in_visited) {
        times_guard_stuck += 1;
        break;
      }

      auto next_tile = input_play_area.at(next.y).at(next.x);
      if (next_tile == '#' ||
          (next.x == obstruction.x && next.y == obstruction.y)) {
        turn(direction);
      }
      else {
        guard_pos = next;
        Visited_Position cur_visit = { guard_pos.x, guard_pos.y, direction };
        visited.push_back(cur_visit);
      }
    }
  }

  std::cout << std::endl
            << "Guard got stuck in a loop " << times_guard_stuck << " times"
            << std::endl
            << std::endl;
  return 0;
}
