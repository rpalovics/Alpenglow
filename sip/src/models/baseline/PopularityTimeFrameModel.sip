struct PopularityTimeFrameModelParameters{
  %TypeHeaderCode
  #include "models/baseline/PopularityTimeFrameModel.h"
  %End
  double tau;
};
class PopularityTimeFrameModel : public Model /NoDefaultCtors/{
  %TypeHeaderCode
  #include "models/baseline/PopularityModel.h"
  %End
  public:
    PopularityTimeFrameModel(PopularityTimeFrameModelParameters* params);
    ~PopularityTimeFrameModel();
    void add(RecDat * rec_dat);
    double prediction(RecDat * rec_dat);
};
