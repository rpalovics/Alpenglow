#ifndef RECENCY_H
#define RECENCY_H

//SIP_AUTOCONVERT

#include <map>
#include <cmath>
#include <cstdio>
#include <iostream>

class Recency{
  public:
    Recency(){};
    ~Recency(){};
    virtual double get(int entity, double actual_time) = 0;
    virtual void update(int entity, double actual_time) = 0;
  private:
};

struct PowerLawRecencyParameters {
  double delta_t;
  double exponent;
};

class PowerLawRecency : public Recency{
  public:
    PowerLawRecency(PowerLawRecencyParameters * params){
      delta_t_ = params->delta_t;
      exponent_ = params->exponent;
    }
    ~PowerLawRecency(){};
    double get(int entity, double actual_time);
    void update(int entity, double actual_time);
  private:
    std::map <int,double> prev_time_;
    double delta_t_;
    double exponent_;
};

#endif /* RECENCY_H */
