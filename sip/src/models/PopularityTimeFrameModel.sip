struct PopularityTimeFrameModelParameters{
  %TypeHeaderCode
  #include "models/PopularityTimeFrameModel.h"
  %End
  double tau;
};
class PopularityTimeFrameModel : public Model /NoDefaultCtors/{
  %TypeHeaderCode
  #include "models/PopularityModel.h"
  %End
  public:
    PopularityTimeFrameModel(PopularityTimeFrameModelParameters* params);
    ~PopularityTimeFrameModel();
    void add(RecDat * rec_dat);
    double prediction(RecDat * rec_dat);
};
