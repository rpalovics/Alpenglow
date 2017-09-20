#ifndef INITIALIZABLE_H
#define INITIALIZABLE_H

class Initializable {
public:
  bool is_initialized(){ return is_initialized_; }
  bool initialize(){
    if(!is_initialized()){
      is_initialized_ = autocalled_initialize();
      return is_initialized_;
    } else {
      return true;
    }
  }
protected:
  virtual bool autocalled_initialize()=0;
private:
  bool is_initialized_=false;
};

#endif
