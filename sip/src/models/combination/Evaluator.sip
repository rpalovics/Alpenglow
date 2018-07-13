
class Evaluator {
  %TypeHeaderCode
  #include "models/combination/Evaluator.h"
  %End
public:
  virtual double get_score(RecDat* rec_dat);
  virtual double get_loss(RecDat* rec_dat);
  virtual ~Evaluator();
  bool self_test();
};

