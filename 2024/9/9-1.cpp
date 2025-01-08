#include <stdlib.h>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum Block_Type { File, Space };

struct Block {
  unsigned int file_id;
  std::string text;
  Block_Type type;
};

bool is_compacted(std::vector<Block>& v)
{
  for (size_t i = 0; i < v.size(); i++) {
    // if (0 < i == i < v.size()) {
    //   if ((v.at(i - 1).type == File) && (v.at(i).type == Space) &&
    //       (v.at(i + 1).type == File)) {
    //     return false;
    //   }
    // }
    if (0 < i) {
      if ((v.at(i - 1).type == Space) && (v.at(i).type == File)) {
        return false;
      }
    }
  }
  return true;
}

int main(/*int argc, char* argv[]*/)
{
  std::ios_base::sync_with_stdio(false);
  std::ifstream file("input");
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  std::vector<int> input;
  char c;
  while (file.get(c)) {
    input.push_back(c);
  }

  file.close();

  bool is_file_block = true;
  unsigned int file_id = 0;
  std::vector<Block> blocks;

  for (size_t idx = 0; idx < input.size(); idx++) {
    int c_count = input.at(idx) - '0';
    if (is_file_block) {
      for (int i = 0; i < c_count; i++) {
        Block new_file;
        new_file.type = File;
        new_file.file_id = file_id;
        new_file.text.append(std::to_string(file_id));
        // blocks.push_back(std::to_string(pos));
        blocks.push_back(new_file);
      }
      file_id++;
    }
    else {
      for (int i = 0; i < c_count; i++) {
        Block new_space;
        new_space.type = Space;
        new_space.text.append(".");
        // blocks.push_back(".");
        blocks.push_back(new_space);
      }
    }
    is_file_block = !is_file_block;
  }

  while (!is_compacted(blocks)) {
    auto first_space_it =
        std::find_if(blocks.begin(), blocks.end(),
                     [](const Block& rhs) { return rhs.type == Space; });
    auto last_file_it =
        std::find_if(blocks.rbegin(), blocks.rend(),
                     [](const Block& rhs) { return rhs.type == File; });
    //    std::cout << "last_file_it->text: " << last_file_it->text <<
    //    std::endl;

    std::iter_swap(last_file_it, first_space_it);
  }

  for (auto it = blocks.begin(); it != blocks.end(); it++) {
    std::cout << it->text;
  }

  unsigned long long checksum = 0LL;

  for (size_t i = 0; i < blocks.size(); i++) {
    if (blocks.at(i).type == Space) {
      break;
    }
    checksum += i * blocks.at(i).file_id;
  }

  std::cout << "\n\nChecksum of compacted disk: " << checksum << "\n\n";

  return 0;
}