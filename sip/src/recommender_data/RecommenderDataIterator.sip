class RecommenderDataIterator : public Initializable {
  %TypeHeaderCode
  #include "recommender_data/RecommenderDataIterator.h"
  %End
  public:
    void set_recommender_data(RecommenderData* data);
    virtual bool has_next() const;
    virtual RecDat* next() = 0;
    virtual int get_counter() const;
    virtual RecDat* get(int index) const = 0;
    virtual RecDat* get_actual() = 0; 
    virtual RecDat* get_future(int index) const = 0;
    virtual int size() const;
    virtual double get_following_timestamp() const = 0;
    virtual ~RecommenderDataIterator();
  protected:
    bool autocalled_initialize();
};
