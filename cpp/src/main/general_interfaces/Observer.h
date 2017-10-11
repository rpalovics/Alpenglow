#ifndef OBSERVER
#define OBSERVER

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

#endif
