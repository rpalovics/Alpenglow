struct NearestNeighborModelParameters {
  %TypeHeaderCode
  #include "models/baseline/NearestNeighborModel.h"
  %End
  double gamma;
  std::string norm;
  std::string direction;
  double gamma_threshold;
  int num_of_neighbors;
  NearestNeighborModelParameters();
};

class NearestNeighborModel : public Model , public ModelFilter {
  %TypeHeaderCode
  #include "models/baseline/NearestNeighborModel.h"
  %End
  public:
    NearestNeighborModel(NearestNeighborModelParameters* params);
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    //vector<pair<int,double>>* get_personalized_items(int user) override;
    bool self_test();
};
