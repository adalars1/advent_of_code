#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
  // std::cout << "argc: " << argc << std::endl;

  for (int i = 0; i < argc; i++) {
    // std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
  }

  std::ifstream file("input");
  std::string line;
  std::string num1, num2;
  std::vector<int> nums_1;
  std::vector<int> nums_2;
  std::vector<int> num_scores;

  if (!file.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  while (file >> num1 >> num2) {
    // std::cout << "num1: " << num1 << " num2: " << num2 << std::endl;
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
    int cur_number_to_find = nums_1.at(i);
    int cur_number_occurences = 0;

    for (int j = 0; j < nums_2.size(); j++) {
      if (nums_2.at(j) == cur_number_to_find) {
        cur_number_occurences++;
      }
    }

    int cur_number_score = cur_number_to_find * cur_number_occurences;
    num_scores.push_back(cur_number_score);
  }

  int similarity_score = 0;
  for (int i = 0; i < num_scores.size(); i++) {
    similarity_score += num_scores.at(i);
  }
  std::cout << "Similarity score: " << similarity_score << std::endl;
  return 0;
}