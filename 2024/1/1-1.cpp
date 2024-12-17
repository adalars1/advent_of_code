#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{

  std::ifstream file("input");
  std::string line;
  std::string num1, num2;
  std::vector<int> nums_1;
  std::vector<int> nums_2;
  std::vector<int> distances;

  if (!file.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  while (file >> num1 >> num2) {
    nums_1.push_back(std::stoi(num1));
    nums_2.push_back(std::stoi(num2));
  }

  file.close();

  if (nums_1.size() != nums_2.size()) {
    std::cerr << "nums_1 size differs from nums_2, exiting.\n";
    return -1;
  }

  std::sort(nums_1.begin(), nums_1.end());
  std::sort(nums_2.begin(), nums_2.end());

  for (int i = 0; i < nums_1.size(); i++) {
    int distance = abs((nums_1.at(i) - nums_2.at(i)));
    distances.push_back(distance);
  }

  int sum_distances = 0;
  for (int i = 0; i < distances.size(); i++) {
    sum_distances += distances.at(i);
  }

  std::cout << "Sum of all distances: " << sum_distances << std::endl;
  return 0;
}