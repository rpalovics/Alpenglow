
struct ToplistCreatorParameters {
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.ToplistCreator`. See documentation there.
  %End
  int top_k;
  int exclude_known;
};
class ToplistCreator : public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
 public:
   ToplistCreator(ToplistCreatorParameters* params);
   virtual ~ToplistCreator();
   virtual std::vector<RecDat>* run(RecDat* rec_dat)=0; 
   void set_model(Model* model);
   void set_filter(ModelFilter* filter);
   void set_train_matrix(SpMatrix *train_matrix);
   bool self_test();
  protected:
   bool autocalled_initialize();
};

struct ToplistCreatorGlobalParameters : public ToplistCreatorParameters {
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.ToplistCreatorGlobalParameters : public ToplistCreator`. See documentation there.
  %End
  int initial_threshold; 
};

class ToplistCreatorGlobal: public ToplistCreator{
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  public:
    ToplistCreatorGlobal(ToplistCreatorGlobalParameters* params);
    virtual ~ToplistCreatorGlobal();
    std::vector<RecDat>* run(RecDat* rec_dat);
    bool self_test();

};

struct ToplistCreatorPersonalizedParameters : public ToplistCreatorParameters {
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.ToplistCreatorPersonalizedParameters : public ToplistCreator`. See documentation there.
  %End
};
class ToplistCreatorPersonalized: public ToplistCreator{
  %TypeHeaderCode
  #include "utils/ToplistCreator.h"
  %End
  public:
    ToplistCreatorPersonalized(ToplistCreatorParameters* params);
    std::vector<RecDat>* run(RecDat* rec_dat);
    bool self_test();
  protected:
    bool autocalled_initialize();
};
