class DataframeIterator : public RecommenderDataIterator {
  %TypeHeaderCode
  #include "cpp/DataframeIterator.h"
  %End
  public:
    DataframeIterator();
    void add_recdats(std::vector<RecDat*>);
    bool has_next() const;
    RecDat* next();
    int get_counter() const;
    RecDat* get(int index);
    RecDat* get_future(int index);
    int size() const;
    double get_following_timestamp();
};