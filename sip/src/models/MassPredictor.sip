class MassPredictor{
%TypeHeaderCode
#include "python/cpp_bridge/MassPredictor.h"
%End
public:
  void set_model(Model *model);
  std::vector<double> predict(std::vector<int> users, std::vector<int> items);
};