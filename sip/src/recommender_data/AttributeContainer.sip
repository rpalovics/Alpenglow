class InlineAttributeReader{
  %TypeHeaderCode
  #include "recommender_data/AttributeContainer.h"
  %End
  public:
    virtual void read_attribute(int id, std::string line)=0;
    virtual ~InlineAttributeReader();
    bool self_test();
};