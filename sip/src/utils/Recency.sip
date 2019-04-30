
class Recency{
  %TypeHeaderCode
  #include "utils/Recency.h"
  %End
  public:
    Recency();
    ~Recency();
    virtual double get(int entity, double actual_time) = 0;
    virtual void update(int entity, double actual_time) = 0;
};

struct PowerLawRecencyParameters {
  %TypeHeaderCode
  #include "utils/Recency.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.PowerLawRecency`. See documentation there.
  %End
  double delta_t;
  double exponent;
};

class PowerLawRecency : public Recency{
  %TypeHeaderCode
  #include "utils/Recency.h"
  %End
  public:
    PowerLawRecency(PowerLawRecencyParameters * params);
    ~PowerLawRecency();
    double get(int entity, double actual_time);
    void update(int entity, double actual_time);
};

