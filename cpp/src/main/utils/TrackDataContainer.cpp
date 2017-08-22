#include "TrackDataContainer.h"

void TrackDataContainer::process_line(std::string& line) {
  std::vector<std::string> splitted_line = split(line, ' ');
  int item = atoi(splitted_line[0].c_str());
  if (item >= (int)track_attributes_.size()) {
    track_attributes_.resize(item+1, nullptr);
  }
  track_attributes_[item] = new std::vector<std::vector<int>>{};
  for ( uint ii = 1; ii < splitted_line.size(); ++ii) {
    std::vector<std::string> ss = split(splitted_line[ii], ',');
    track_attributes_[item]->push_back(std::vector<int>());
    for ( uint jj = 0; jj < ss.size(); ++jj) {
      if ( ss[jj] == "" ) {
        continue;
      }
      (*(track_attributes_[item]))[ii-1].push_back(atoi(ss[jj].c_str()));
    }
  }
}

void TrackDataContainer::read_file(const char *file_name) {
  std::ifstream fi(file_name);
  std::string buffer;
  while (std::getline(fi, buffer)) {
    process_line(buffer);
  }
}
