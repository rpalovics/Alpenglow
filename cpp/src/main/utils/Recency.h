#ifndef RECENCY_H
#define RECENCY_H

//SIP_AUTOCONVERT

#include <map>
#include <cmath>
#include <cstdio>
#include <iostream>

class Recency{
  public:
    virtual ~Recency(){}
    virtual double get(int entity, double actual_time) = 0;
    virtual void update(int entity, double actual_time) = 0;
  private:
};

struct PowerLawRecencyParameters {
  double delta_t = -1;
  double exponent = -1;
};

class PowerLawRecency : public Recency{
  public:
    PowerLawRecency(PowerLawRecencyParameters* params){
      delta_t_ = params->delta_t;
      exponent_ = params->exponent;
    }
    double get(int entity, double actual_time) override;
    void update(int entity, double actual_time) override;
  private:
    std::map <int,double> prev_time_;
    double delta_t_;
    double exponent_;
};

#endif /* RECENCY_H */
