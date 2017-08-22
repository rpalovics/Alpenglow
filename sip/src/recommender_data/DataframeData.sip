class DataframeData : public RecommenderData {
  %TypeHeaderCode
  #include "python/cpp_bridge/DataframeData.h"
  %End
  public:
    virtual bool init();
    void add_recdats(std::vector<RecDat*> rds);
    int size();
    RecDat* get(int idx);
};