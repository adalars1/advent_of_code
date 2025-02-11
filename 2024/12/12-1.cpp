#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

struct Coordinate {
  size_t row;
  size_t col;
};

struct Plant_Region {
  char plant;
  long fences;
  long area;
  std::vector<Coordinate> coordinates;
};

bool region_exists(std::vector<Plant_Region>& regions, char plant, size_t row,
                   size_t col)
{
  std::cerr << "Checking if region exists for plant: " << plant
            << " at row: " << row << " col: " << col << "\n";
  for (auto region : regions) {
    if (region.plant == plant) {
      for (auto coord : region.coordinates) {
        if (coord.row == row && coord.col == col) {
          return true;
        }
      }
    }
  }
  return false;
}

Plant_Region& get_region(std::vector<Plant_Region>& regions, size_t row,
                         size_t col)
{
  std::cerr << "Getting region for row: " << row << " col: " << col << "\n";
  for (auto& region : regions) {  // Iterera med referens
    for (auto& coord : region.coordinates) {
      if (coord.row == row && coord.col == col) {
        std::cerr << "Returning region: " << region.plant << "\n";
        return region;  // Returnera referens till matchande region
      }
    }
  }
  std::cerr << "No region found for row: " << row << ", col: " << col
            << ", call region_exists first. Exiting.\n";
  exit(-1);
}

void add_to_region(Plant_Region& region, char plant, size_t row, size_t col)
{
  if (region.plant != plant) {
    std::cerr << "Error: Trying to add plant: " << plant
              << " to region: " << region.plant << ", exiting.\n";
    exit(-1);
  }
  region.area++;
  region.coordinates.push_back({ row, col });
  std::cerr << "Added plant: " << plant << " to region: " << region.plant
            << ", coordnates.size is now: " << region.coordinates.size()
            << "\n";
}

int main(int argc, char* argv[])
{
  std::ios_base::sync_with_stdio(false);
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input file>\n";
    return -1;
  }
  std::ifstream file_input(argv[1]);

  if (!file_input.is_open()) {
    std::cerr << "Unable to open file '" << argv[1] << "'" << std::endl;
    return -1;
  }

  std::vector<std::string> input;
  std::string line;
  while (file_input >> line) {
    input.push_back(line);
  }

  file_input.close();

  std::vector<Plant_Region> regions;

  // Find all plant regions in input file
  for (size_t row = 0; row < input.size(); row++) {
    for (size_t col = 0; col < input[row].size(); col++) {
      std::cerr << "\nInput: " << input[row][col] << " at row: " << row
                << " col: " << col << "\n";
      size_t up = row - 1;
      size_t left = col - 1;

      char cur_plant = input[row][col];

      if (left < input[row].size()) {  // coords are unsigned, always >= 0
        if (cur_plant == input[row][left]) {
          std::cerr << " Left: ";
          // we are to the right of an existing region of the same plant
          if (region_exists(regions, cur_plant, row, left)) {
            Plant_Region& existing_region = get_region(regions, row, left);
            add_to_region(existing_region, cur_plant, row, col);
            continue;
          }
        }
      }
      
      if (up < input.size()) {  // coords are unsigned, always >= 0
        if (cur_plant == input[up][col]) {
          // we are below an existing region of the same plant
          std::cerr << " Up: ";
          if (region_exists(regions, cur_plant, up, col)) {
            Plant_Region& existing_region = get_region(regions, up, col);
            add_to_region(existing_region, cur_plant, row, col);
            continue;
          }
        }
      }



      // Walk right to search for existing region above in mirrored L pattern

      if (up < input.size()) {  // coords are unsigned, always >= 0
        size_t end_cur_plant_col = col;
        while (end_cur_plant_col < input[row].size() &&
               input[row][end_cur_plant_col] == cur_plant) {
          end_cur_plant_col++;
        }
        end_cur_plant_col--;
        std::cerr << "####### end_cur_plant_col: " << end_cur_plant_col << "\n";

        for (size_t i = end_cur_plant_col; i > col; i--) {
          if (input[up][i] == cur_plant) {
            std::cerr << "Found same plant: " << cur_plant << " at row: " << row
                      << " col: " << i << "\n";
            end_cur_plant_col = i;
            break;
          }
        }

        // size_t found_col = col;
        // if (up < input.size()) {  // coords are unsigned, always >= 0
        //   while (found_col < input[row].size()) {
        //     std::cerr << "found_col step: " << found_col << "\n";
        //     if (input[up][found_col] == cur_plant) {
        //       break;
        //     }

        //     else {
        //       found_col++;
        //     }
        //   }

        //   std::cerr << "found_col after loop: " << found_col << "\n";

        std::cerr << " Up Right Reverse L: ";
        if (region_exists(regions, cur_plant, up, end_cur_plant_col)) {
          Plant_Region& existing_region =
              get_region(regions, up, end_cur_plant_col);
          add_to_region(existing_region, cur_plant, row, col);
          continue;
        }
      }

      if (!region_exists(regions, cur_plant, row, col)) {
        std::cerr << "Creating new area for plant: " << cur_plant
                  << " at row: " << row << " col: " << col << "\n";
        // cur_plant does not touch an existing region of same plant type,
        // create new region
        std::vector<Coordinate> new_region_coords;
        new_region_coords.push_back({ row, col });

        Plant_Region new_region;
        new_region.plant = cur_plant;
        new_region.fences = 0;
        new_region.area = 1;
        new_region.coordinates = new_region_coords;
        regions.push_back(new_region);
      }
    }
  }

  // Calculate amount of fences needed per region
  /*
  for (auto& region : regions) {
    for (auto& coord : region.coordinates) {
      size_t row = coord.row;
      size_t col = coord.col;

      size_t up = row - 1;
      size_t down = row + 1;
      size_t left = col - 1;
      size_t right = col + 1;

      if (up < input.size()) {
        if (input[row][col] != input[up][col]) {
          region.fences++;
        }
      }
      else {
        region.fences++;  // count fence around the whole map
      }

      if (down < input.size()) {
        if (input[row][col] != input[down][col]) {
          region.fences++;
        }
      }
      else {
        region.fences++;  // count fence around the whole map
      }

      if (left < input[row].size()) {
        if (input[row][col] != input[row][left]) {
          region.fences++;
        }
      }
      else {
        region.fences++;  // count fence around the whole map
      }

      if (right < input[row].size()) {
        if (input[row][col] != input[row][right]) {
          region.fences++;
        }
      }
      else {
        region.fences++;  // count fence around the whole map
      }
    }
    // std::cerr << "\n";
  }
  // std::cerr << "\n";
*/

  for (auto& region : regions) {
    for (auto& coord : region.coordinates) {
      size_t row = coord.row;
      size_t col = coord.col;

      if (row == 0 || input[row - 1][col] != region.plant) region.fences++;
      if (row == input.size() - 1 || input[row + 1][col] != region.plant)
        region.fences++;
      if (col == 0 || input[row][col - 1] != region.plant) region.fences++;
      if (col == input[row].size() - 1 || input[row][col + 1] != region.plant)
        region.fences++;
    }
  }

  // Calculate total fencing cost for all regions
  long long sum_fencing_cost = 0;
  for (auto region : regions) {
    std::cerr << "A region of " << region.plant << " plants with price "
              << region.area << " * " << region.fences << " = "
              << region.area * region.fences << "\n";

    sum_fencing_cost = sum_fencing_cost + (region.area * region.fences);
  }

  // std::cerr << "\n\n";

  // Step 1 correct answer: 647 regions, cost : 1374934
  // Step 2 correct answer: 647 regions, cost : 841078
  // Cur answer:            905 regions, cost : 1052318
  std::cerr << "\nNumber of plant regions: " << regions.size() << "\n";
  std::cerr << "\nTotal cost of fencing for all plant areas: "
            << sum_fencing_cost << "\n\n";

  return 0;
}
