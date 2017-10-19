struct FactorRepr{
  %TypeHeaderCode
  #include "python/cpp_bridge/FactorModelReader.h"
  %End
  int entity;
  std::vector<double> factors;
};
struct UserItemFactors{
  %TypeHeaderCode
  #include "python/cpp_bridge/FactorModelReader.h"
  %End
  std::vector<FactorRepr> user_factors;
  std::vector<FactorRepr> item_factors;
};
class FactorModelReader{
  %TypeHeaderCode
  #include "python/cpp_bridge/FactorModelReader.h"
  %End
public:
  UserItemFactors read(std::string file_name, int dimension);
};

class EigenFactorModelReader{
  %TypeHeaderCode
  #include "python/cpp_bridge/FactorModelReader.h"
  %End
public:
  UserItemFactors read(std::string file_name);
};
