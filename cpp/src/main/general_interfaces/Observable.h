#ifndef OBSERVABLE
#define OBSERVABLE

#include "Observer.h"
#include <vector>

using namespace std;

class Observable{
  public:
    void attach(Observer* observer){
      observers_.push_back(observer);
    }
    void notify_observers(){
      for(auto observer: observers_){
        observer->notify();
      }
    }
  private:
    vector<Observer*> observers_;
};

#endif
