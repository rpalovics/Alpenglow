struct EgienFactorsParamteres{
  %TypeHeaderCode
  #include "utils/EigenFactors.h"
  %End
  double begin_max;
  double begin_min;
  double dimension;
  int seed;
};

class EigenFactors{
  %TypeHeaderCode
  #include "utils/EigenFactors.h"
  %End
  public:
    EigenFactors();
    EigenFactors(EgienFactorsParamteres parameters);
    void set_parameters(EgienFactorsParamteres parameters);
};
