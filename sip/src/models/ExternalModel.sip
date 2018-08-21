struct ExternalModelParameters{
  %TypeHeaderCode
  #include "models/ExternalModel.h"
  %End
  std::string mode;
};

class ExternalModel : Model{
  %TypeHeaderCode
  #include "models/ExternalModel.h"
  %End
  public:
    void add(RecDat* rec_dat) ;
    double prediction(RecDat* rec_dat) ;
    void clear();
    ExternalModel(ExternalModelParameters* parameters);
    ~ExternalModel();
    bool self_test();
    void read_predictions(std::string file_name);
};
