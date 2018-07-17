
class Bias{
  %TypeHeaderCode
  #include "utils/Bias.h"
  %End
  public:
    Bias();
    ~Bias();
    double get(int entity);
    void update(int entity, double val);
    void init(int entity);
    void clear();
};

