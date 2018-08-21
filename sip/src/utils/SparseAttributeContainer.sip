struct SparseAttributeContainerParameters{
  %TypeHeaderCode
  #include "utils/SparseAttributeContainer.h"
  %End
};
class SparseAttributeContainer {
  %TypeHeaderCode
  #include "utils/SparseAttributeContainer.h"
  %End
public:
  // unordered_map<int, double> get_attributes(int id);
  int get_max_attribute_index();
};
class FileSparseAttributeContainer : public SparseAttributeContainer{
  %TypeHeaderCode
  #include "utils/SparseAttributeContainer.h"
  %End
public:
  void load_from_file(std::string file_name);
};
