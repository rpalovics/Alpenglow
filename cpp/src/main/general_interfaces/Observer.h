#ifndef OBSERVER_H
#define OBSERVER_H

class Observer{
  public:
    Observer(){
      notified_ = 0;
    }
    void notify(){ notified_++; }
    bool notified(){ return notified_>0; }
    void delete_notification(){ notified_=0; }
  private:
    int notified_;
};

#endif /* OBSERVER_H */
