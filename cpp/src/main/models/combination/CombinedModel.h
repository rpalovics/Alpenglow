#ifndef COMBINED_MODEL
#define COMBINED_MODEL

//SIP_AUTOCONVERT

#include <vector>
#include "../Model.h"

using namespace std;

struct CombinedModelParameters{
  string log_file_name;
  int log_frequency;
  bool use_user_weights;
};
class CombinedModel : public Model {
  public:
    CombinedModel(CombinedModelParameters* params){
      log_file_stream = new ofstream(params->log_file_name.c_str());
      log_frequency = params->log_frequency;
      log_counter = 0;
      use_user_weights=params->use_user_weights;
    };
    ~CombinedModel(){ delete log_file_stream; };
    void add_model(Model* model){
      models.push_back(model);
      int model_num=models.size();
      global_weights.clear();
      global_weights.resize(model_num,1/(double)model_num);
    }
    double prediction(RecDat* rec_dat);
    void add(RecDat* rec_dat);
  private:
    vector <Model*>  models;
    vector <vector<double>*> user_weights;
    vector <double> global_weights;
    ofstream* log_file_stream;
    int log_frequency;
    int log_counter;
    bool use_user_weights;
    friend class CombinedDoubleLayerModelGradientUpdater; 
    friend class CombinedSingleLayerModelGradientUpdater;
};

#endif
