#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

bool get_report_safety(std::vector<int> report)
{
  bool report_is_safe = true;
  bool has_positive_deltas = false;
  bool has_negative_deltas = false;
  for (auto it = report.begin(); it != report.end() - 1; it++) {
    auto next_it = it + 1;
    int cur = *it;
    int next = *next_it;

    if (cur < next) {
      has_positive_deltas = true;
    }
    if (cur > next) {
      has_negative_deltas = true;
    }

    if (has_negative_deltas && has_positive_deltas) {
      report_is_safe = false;
    }

    unsigned int delta = abs(cur - next);

    if (delta > 3) {
      report_is_safe = false;
    }
    if (delta < 1) {
      report_is_safe = false;
    }
  }

  return report_is_safe;
}

int main(int argc, char* argv[])
{
  std::ifstream file("input");
  std::string line;
  std::vector<std::vector<int>> input_reports;
  std::vector<std::vector<int>> unsafe_reports;

  if (!file.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return -1;
  }

  while (getline(file, line, '\n')) {
    std::vector<int> report;

    std::istringstream ss(line);
    std::string report_digit;
    while (ss >> report_digit) {
      report.push_back(std::stoi(report_digit));
    }

    input_reports.push_back(report);
  }

  file.close();

  int count_safe_reports = 0;

  for (auto cur_report : input_reports) {
    bool is_safe = get_report_safety(cur_report);
    if (is_safe) {
      count_safe_reports++;
    }
    else {
      unsafe_reports.push_back(cur_report);
    }
  }

  for (auto unsafe_report : unsafe_reports) {
    bool safe_variant_found = false;

    for (int index = 0; index < unsafe_report.size(); index++) {
      std::vector<int> tmp_variant(unsafe_report);  //
      tmp_variant.at(index) = 1337;  // value not present in input

      auto new_end = std::remove_if(tmp_variant.begin(), tmp_variant.end(),
                                    [](const int& cur) { return cur == 1337; });
      tmp_variant.erase(new_end, tmp_variant.end());

      if (get_report_safety(tmp_variant)) {
        safe_variant_found = true;
      }
    }

    if (safe_variant_found) {
      count_safe_reports++;
    }
  }

  std::cout << "Count of safe reports: " << count_safe_reports << std::endl;
  return 0;
}