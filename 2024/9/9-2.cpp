#include <stdlib.h>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum Chunk_Type { File, Space };

struct Chunk {
  std::string text;
  Chunk_Type type;
  int file_id;
  unsigned int space;

  friend std::ostream& operator<<(std::ostream& os, const Chunk& c)
  {
    os << c.file_id << "::" << c.type << "::" << c.text << " ["
       << c.text.length() << "]" << " (" << c.space << ")";
    return os;
  }
};

void print_chunks(std::vector<Chunk>& vc)
{
  for (auto chunk_it = vc.begin(); chunk_it != vc.end(); chunk_it++) {
    std::cout << chunk_it->text << " ";
  }
  std::cout << "\n";
}
int main(/*int argc, char* argv[]*/)
{
  std::ios_base::sync_with_stdio(false);
  std::ifstream file_input("input");
  std::string line;

  if (!file_input.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  std::vector<char> input;
  char c;
  while (file_input.get(c)) {
    if (std::isdigit(c)) {
      input.push_back(c);
    }
  }

  file_input.close();

  bool is_file = true;
  unsigned int file_id_counter = 0;
  std::vector<Chunk> chunks;

  for (size_t idx = 0; idx < input.size(); idx++) {
    int length = input[idx] - '0';
    Chunk new_chunk;
    new_chunk.space = length;
    if (is_file) {
      new_chunk.file_id = file_id_counter;
      new_chunk.type = File;
      for (int i = 0; i < length; i++) {
        new_chunk.text.append(std::to_string(file_id_counter));
      }
      file_id_counter++;
    }
    else {
      new_chunk.file_id = -1;
      new_chunk.type = Space;
      for (int i = 0; i < length; i++) {
        new_chunk.text.append(".");
      }
    }
    chunks.push_back(new_chunk);
    is_file = !is_file;
  }

  print_chunks(chunks);
  std::cout << "\n";

  // Gå igenom filerna efter fallande file_id
  for (int cur_file_id = file_id_counter - 1; cur_file_id > 0; --cur_file_id) {
    // Hitta nuvarande fil baserat på file_id
    auto file_it = std::find_if(
        chunks.begin(), chunks.end(), [cur_file_id](const Chunk& rhs) {
          return (rhs.file_id == cur_file_id) && (rhs.type == File);
        });
    auto file_idx = std::distance(chunks.begin(), file_it);

    // Hitta första Space chunk från vänster som har ett space som är större än
    // filens space
    auto larger_space_it =
        std::find_if(chunks.begin(), chunks.end(), [file_it](const Chunk& rhs) {
          return (rhs.type == Space) && (rhs.space > file_it->space);
        });
    auto larger_space_idx = std::distance(chunks.begin(), larger_space_it);

    // Hitta första Space chunk från vänster som har ett space som är lika
    // stort som filens space
    auto equal_space_it =
        std::find_if(chunks.begin(), chunks.end(), [file_it](const Chunk& rhs) {
          return (rhs.type == Space) && (rhs.space == file_it->space);
        });
    auto equal_space_idx = std::distance(chunks.begin(), equal_space_it);

    if ((larger_space_it == chunks.end() && equal_space_it == chunks.end()) ||
        ((larger_space_idx >= file_idx) && (equal_space_idx >= file_idx))) {
      continue;  // File does not fit anywhere to the left, skip to next file_id
    }

    if (larger_space_idx < equal_space_idx) {
      // Räkna ut hur mycket space som blir över efter flytten
      unsigned int leftover_space = larger_space_it->space - file_it->space;
      // Byt plats på datan manuellt
      larger_space_it->file_id = file_it->file_id;
      larger_space_it->type = file_it->type;
      larger_space_it->space = file_it->space;
      larger_space_it->text = file_it->text;

      file_it->file_id = -1;
      file_it->type = Space;
      file_it->space = larger_space_it->space;
      file_it->text = std::string(larger_space_it->space, '.');

      // Skapa ny chunk för resterande space
      Chunk new_space_chunk;
      new_space_chunk.file_id = -1;
      new_space_chunk.type = Space;
      new_space_chunk.space = leftover_space;
      new_space_chunk.text = std::string(leftover_space, '.');

      // std::cout << "inserting: " << new_space_chunk
      //           << " with space=" << new_space_chunk.space
      //           << ", text=" << new_space_chunk.text << "\n";
      // Lägg in nya spacet ett steg efter spacet som nu har file data
      chunks.insert(larger_space_it + 1, new_space_chunk);
    }

    else if (equal_space_idx < larger_space_idx) {
      // std::cout << "swapping: " << *file_it << " with " << *equal_space_it
      //           << "\n";
      // iter_swap gör automatiskt flytt av djupa datan
      std::iter_swap(file_it, equal_space_it);
    }
  }

  for (auto chunk_it = chunks.begin(); chunk_it != chunks.end(); chunk_it++) {
    if (chunk_it->type == Space) {
      if (chunk_it->space != chunk_it->text.size()) {
        std::cout << "ERROR: Space size mismatch\n";
        return -1;
      }
      if (chunk_it->file_id != -1) {
        std::cout << "ERROR: Space file_id is " << chunk_it->file_id
                  << ", shall be -1\n";
        return -1;
      }
    }
    else if (chunk_it->type == File) {
      if (chunk_it->space !=
          (chunk_it->text.size() / std::to_string(chunk_it->file_id).size())) {
        std::cout << "ERROR: File size mismatch for:\n";
        std::cout << *chunk_it << "\n";
        return -1;
      }
    }
  }

  print_chunks(chunks);

  unsigned long long checksum = 0LL;
  unsigned int block_counter = 0;

  for (const auto& chunk : chunks) {
    if (chunk.type == File) {
      for (unsigned int i = 0; i < chunk.space; i++) {
        checksum += block_counter * chunk.file_id;
        block_counter++;
      }
    }
    else {
      // Öka block_counter även för Space
      block_counter += chunk.space;
    }
  }

  std::cout << "\n\nChecksum of defragged files: " << checksum << "\n\n";

  return 0;
}