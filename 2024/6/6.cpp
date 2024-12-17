#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum Direction { NORTH, SOUTH, WEST, EAST };

static int visited_indicies = 0;
static int PLAY_AREA_WIDTH = 0;
static int PLAY_AREA_HEIGHT = 0;

static std::vector<std::string> play_area;

class Guard {
 public:
  Guard() : pos_x(0), pos_y(0), direction(NORTH) {}

  bool is_outside_play_area()
  {
    if ((pos_x >= 0 && pos_x <= PLAY_AREA_WIDTH) &&
        (pos_y >= 0 && pos_y <= PLAY_AREA_HEIGHT)) {
      return false;
    }
    else {
      return true;
    }
  }

  void step()
  {
    int next_pos_x;
    int next_pos_y;

    switch (direction) {
      case NORTH:
        next_pos_x = pos_x;
        next_pos_y = pos_y - 1;
        break;
      case SOUTH:
        next_pos_x = pos_x;
        next_pos_y = pos_y + 1;
        break;
      case WEST:
        next_pos_x = pos_x - 1;
        next_pos_y = pos_y;
        break;
      case EAST:
        next_pos_x = pos_x + 1;
        next_pos_y = pos_y;
        break;
    }
    play_area.at(pos_y).at(pos_x) = 'X'; // mark current position as visited

    if ((next_pos_x >= 0 && next_pos_x <= PLAY_AREA_WIDTH) &&
        (next_pos_y >= 0 && next_pos_y <= PLAY_AREA_HEIGHT)) {
      if (play_area.at(next_pos_y).at(next_pos_x) == '#') {
        // Change direction 90 degrees clockwise
        switch (direction) {
          case NORTH:
            direction = EAST;
            break;
          case SOUTH:
            direction = WEST;
            break;
          case WEST:
            direction = NORTH;
            break;
          case EAST:
            direction = SOUTH;
            break;
        }
      }
      else {
        pos_x = next_pos_x;
        pos_y = next_pos_y;
      }
    }
    else {
      pos_x = -1;
      pos_y = -1;
    }
  }

  int pos_x;
  int pos_y;
  Direction direction;
};

int main(int argc, char* argv[])
{
  std::ifstream file("input");
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  while (getline(file, line, '\n')) {
    play_area.push_back(line);
  }

  file.close();

  Guard guard;

  // Find starting position for guard and play area size
  for (auto row = 0; row < play_area.size(); row++) {
    if (play_area.at(row).size() > PLAY_AREA_HEIGHT) {
      PLAY_AREA_HEIGHT = play_area.at(row).size() - 1;
    }
    for (auto col = 0; col < play_area.at(row).size(); col++) {
      if (col > PLAY_AREA_WIDTH) {
        PLAY_AREA_WIDTH = col;
      }
      if (play_area.at(row).at(col) == '^') {
        guard.pos_x = col;
        guard.pos_y = row;
      }
    }  // for cols (x)

  }  // for lines (y)

  std::cout << "Play area width: " << PLAY_AREA_WIDTH << std::endl
            << "Play area height: " << PLAY_AREA_HEIGHT << std::endl
            << "Guard outside play area: " << guard.is_outside_play_area()
            << std::endl;

  while (!guard.is_outside_play_area()) {
    guard.step();
  }

  for (auto line : play_area) {
    std::cout << line << std::endl;
  }

  int placed_xes = 0;
  for (auto row = 0; row < play_area.size(); row++) {
    for (auto col = 0; col < play_area.at(row).size(); col++) {
      if (play_area.at(row).at(col) == 'X') {
        placed_xes++; // count visited positions
      }
    }
  }

  std::cout << "Visited positions: " << placed_xes << std::endl;
  return 0;
}