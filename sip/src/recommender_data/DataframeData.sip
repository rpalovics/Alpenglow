class DataframeData : public RecommenderData {
  %TypeHeaderCode
  #include "cpp/DataframeData.h"
  %End
  public:
    virtual bool init();
    void add_recdats(std::vector<RecDat*> rds);
    int size();
    RecDat* get(int idx);
};