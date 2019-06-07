#ifndef EXTERNAL_MODEL_H
#define EXTERNAL_MODEL_H

#include "Model.h"
#include "TopListRecommender.h"
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

struct ExternalModelParameters {
  string mode;
};

class ExternalModel : public Model, public TopListRecommender{
  public:
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    void clear() override {}
    ExternalModel(ExternalModelParameters *parameters) : mode_(parameters->mode){}
    ~ExternalModel() override {}
    bool self_test(){
      return true;
    }
    void read_predictions(string file_name);
    void write_requested_users(string file_name);
    vector<pair<int,double>> get_top_list(int user, int k, const map<int,double> *exclude) override;

  protected:
    const string mode_;
    unordered_set<int> requested_users_;
    unordered_map<int, vector<int>> toplists_;
};

#endif /* EXTERNAL_MODEL_H */
