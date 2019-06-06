#include <vector>
#include <gtest/gtest.h>
#include "../../main/online_experiment/ExperimentEnvironment.h"
#include "../../main/recommender_data/SimpleIterator.h"


namespace {

class TestExperimentEnvironment : public ::testing::Test  {
public:
  RecommenderData recommender_data;
  vector<RecDat> rec_dats;
  SimpleIterator rdi;
  OnlineExperimentParameters params;
  TestExperimentEnvironment() {
  }
  virtual ~TestExperimentEnvironment() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    rec_dats.resize(10);
    for(uint i=0;i<rec_dats.size();i++){
      rec_dats[i].time = i;
      rec_dats[i].user = i/4;
      rec_dats[i].item = i/3;
      rec_dats[i].score = 1;
      rec_dats[i].id = i;
      rec_dats[i].eval = 1;
    }
    recommender_data.set_rec_data(rec_dats);
    rdi.set_recommender_data(&recommender_data);
    params.top_k = 123;
    params.evaluation_start_time = 0;
    params.experiment_termination_time = 0;
    params.exclude_known = true;
    params.initialize_all = false;
    params.max_user = -1;
    params.max_item = -1;
    params.random_seed = 54321;
  }
  virtual void TearDown(){
  }
};

}
TEST_F(TestExperimentEnvironment, self_test_filters_missing_call) {
  ExperimentEnvironment expenv;
  EXPECT_FALSE(expenv.self_test());
  expenv.set_parameters(&params);
  EXPECT_FALSE(expenv.self_test()); //rdi==NULL
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());
}

TEST_F(TestExperimentEnvironment, self_test_filters_missing_call2) {
  ExperimentEnvironment expenv;
  EXPECT_FALSE(expenv.self_test());
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_FALSE(expenv.self_test()); //params are not set
  expenv.set_parameters(&params);
  EXPECT_TRUE(expenv.self_test());
}

TEST_F(TestExperimentEnvironment, self_test_filters_bad_topk) {
  params.top_k = -3;
  ExperimentEnvironment expenv;
  expenv.set_recommender_data_iterator(&rdi);
  expenv.set_parameters(&params);
  EXPECT_FALSE(expenv.self_test());
}

TEST_F(TestExperimentEnvironment, self_test_filters_bad_initall) {
  params.initialize_all = true;
  ExperimentEnvironment expenv;
  expenv.set_recommender_data_iterator(&rdi);
  expenv.set_parameters(&params);
  EXPECT_FALSE(expenv.self_test()); //max_user, max_item not set
  params.max_user = 123;
  params.max_item = 432;
  expenv.set_parameters(&params);
  EXPECT_TRUE(expenv.self_test());
}

TEST_F(TestExperimentEnvironment, self_test_filters_bad_time) {
  params.evaluation_start_time = 1234;
  params.experiment_termination_time = 324;
  ExperimentEnvironment expenv;
  expenv.set_recommender_data_iterator(&rdi);
  expenv.set_parameters(&params);
  EXPECT_FALSE(expenv.self_test()); //max_user, max_item not set
  params.evaluation_start_time = 1234;
  params.experiment_termination_time = 2324;
  expenv.set_parameters(&params);
  EXPECT_TRUE(expenv.self_test());
}

TEST_F(TestExperimentEnvironment, queries1) {
  params.top_k = 123;
  params.evaluation_start_time = 134;
  params.experiment_termination_time = 234;
  params.exclude_known = true;
  params.initialize_all = false;
  params.max_user = 1234;
  params.max_item = 1111;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());
  EXPECT_EQ(123,expenv.get_top_k());
  EXPECT_EQ(134,expenv.get_evaluation_start_time());
  EXPECT_EQ(234,expenv.get_experiment_termination_time());
  EXPECT_EQ(true,expenv.get_exclude_known());
  EXPECT_EQ(false,expenv.get_initialize_all());
  EXPECT_EQ(1234,expenv.get_max_user_id());
  EXPECT_EQ(1111,expenv.get_max_item_id());
}
TEST_F(TestExperimentEnvironment, queries2) {
  params.top_k = 321;
  params.evaluation_start_time = 431;
  params.experiment_termination_time = 432;
  params.exclude_known = false;
  params.initialize_all = true;
  params.max_user = 4321;
  params.max_item = 2222;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());
  EXPECT_EQ(321,expenv.get_top_k());
  EXPECT_EQ(431,expenv.get_evaluation_start_time());
  EXPECT_EQ(432,expenv.get_experiment_termination_time());
  EXPECT_EQ(false,expenv.get_exclude_known());
  EXPECT_EQ(true,expenv.get_initialize_all());
  EXPECT_EQ(4321,expenv.get_max_user_id());
  EXPECT_EQ(2222,expenv.get_max_item_id());
}

TEST_F(TestExperimentEnvironment, new_user_item) {
  //data:
  //id=0 t=0 u=0 i=0 s=1
  //id=1 t=1 u=0 i=0 s=1
  //id=2 t=2 u=0 i=0 s=1
  //id=3 t=3 u=0 i=1 s=1
  //id=4 t=4 u=1 i=1 s=1
  //id=5 t=5 u=1 i=1 s=1
  //id=6 t=6 u=1 i=2 s=1
  //id=7 t=7 u=1 i=2 s=1
  //id=8 t=8 u=2 i=2 s=1
  //id=9 t=9 u=2 i=3 s=1

  params.initialize_all = false;
  params.max_user = -1;
  params.max_item = -1;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());

  for (int i=0;i<10;i++) EXPECT_FALSE(expenv.is_item_existent(i));
  for (int i=0;i<10;i++) EXPECT_FALSE(expenv.is_user_existing(i));
  expenv.update(rdi.next()); //id=0 t=0 u=0 i=0 s=1
  EXPECT_TRUE(expenv.is_new_item());
  EXPECT_TRUE(expenv.is_first_occurrence_of_item());
  EXPECT_TRUE(expenv.is_new_user());
  EXPECT_TRUE(expenv.is_first_occurrence_of_user());
  EXPECT_TRUE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=1 t=1 u=0 i=0 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_FALSE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_FALSE(expenv.is_first_occurrence_of_user());
  EXPECT_FALSE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=2 t=2 u=0 i=0 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_FALSE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_FALSE(expenv.is_first_occurrence_of_user());
  EXPECT_FALSE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=3 t=3 u=0 i=1 s=1
  EXPECT_TRUE(expenv.is_new_item());
  EXPECT_TRUE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_FALSE(expenv.is_first_occurrence_of_user());
  EXPECT_TRUE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=4 t=4 u=1 i=1 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_FALSE(expenv.is_first_occurrence_of_item());
  EXPECT_TRUE(expenv.is_new_user());
  EXPECT_TRUE(expenv.is_first_occurrence_of_user());
  EXPECT_TRUE(expenv.is_item_new_for_user());
  for (int i=0;i<2;i++) EXPECT_TRUE(expenv.is_item_existent(i));
  for (int i=2;i<10;i++) EXPECT_FALSE(expenv.is_item_existent(i));
  for (int i=0;i<2;i++) EXPECT_TRUE(expenv.is_user_existing(i));
  for (int i=2;i<10;i++) EXPECT_FALSE(expenv.is_user_existing(i));

  expenv.update(rdi.next()); //id=5 t=5 u=1 i=1 s=1
  expenv.update(rdi.next()); //id=6 t=6 u=1 i=2 s=1
  expenv.update(rdi.next()); //id=7 t=7 u=1 i=2 s=1
  expenv.update(rdi.next()); //id=8 t=8 u=2 i=2 s=1
  expenv.update(rdi.next()); //id=9 t=9 u=2 i=3 s=1
  for (int i=0;i<4;i++) EXPECT_TRUE(expenv.is_item_existent(i));
  for (int i=4;i<10;i++) EXPECT_FALSE(expenv.is_item_existent(i));
  for (int i=0;i<3;i++) EXPECT_TRUE(expenv.is_user_existing(i));
  for (int i=3;i<10;i++) EXPECT_FALSE(expenv.is_user_existing(i));
}

TEST_F(TestExperimentEnvironment, new_user_item_initall) {
  //data:
  //id=0 t=0 u=0 i=0 s=1
  //id=1 t=1 u=0 i=0 s=1
  //id=2 t=2 u=0 i=0 s=1
  //id=3 t=3 u=0 i=1 s=1
  //id=4 t=4 u=1 i=1 s=1
  //id=5 t=5 u=1 i=1 s=1
  //id=6 t=6 u=1 i=2 s=1
  //id=7 t=7 u=1 i=2 s=1
  //id=8 t=8 u=2 i=2 s=1
  //id=9 t=9 u=2 i=3 s=1

  params.initialize_all = true;
  params.max_user = 2;
  params.max_item = 3;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());

  for (int i=0;i<4;i++) EXPECT_TRUE(expenv.is_item_existent(i));
  for (int i=4;i<10;i++) EXPECT_FALSE(expenv.is_item_existent(i));
  for (int i=0;i<3;i++) EXPECT_TRUE(expenv.is_user_existing(i));
  for (int i=3;i<10;i++) EXPECT_FALSE(expenv.is_user_existing(i));

  expenv.update(rdi.next()); //id=0 t=0 u=0 i=0 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_TRUE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_TRUE(expenv.is_first_occurrence_of_user());
  EXPECT_TRUE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=1 t=1 u=0 i=0 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_FALSE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_FALSE(expenv.is_first_occurrence_of_user());
  EXPECT_FALSE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=2 t=2 u=0 i=0 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_FALSE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_FALSE(expenv.is_first_occurrence_of_user());
  EXPECT_FALSE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=3 t=3 u=0 i=1 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_TRUE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_FALSE(expenv.is_first_occurrence_of_user());
  EXPECT_TRUE(expenv.is_item_new_for_user());

  expenv.update(rdi.next()); //id=4 t=4 u=1 i=1 s=1
  EXPECT_FALSE(expenv.is_new_item());
  EXPECT_FALSE(expenv.is_first_occurrence_of_item());
  EXPECT_FALSE(expenv.is_new_user());
  EXPECT_TRUE(expenv.is_first_occurrence_of_user());
  EXPECT_TRUE(expenv.is_item_new_for_user());

  for (int i=0;i<4;i++) EXPECT_TRUE(expenv.is_item_existent(i));
  for (int i=4;i<10;i++) EXPECT_FALSE(expenv.is_item_existent(i));
  for (int i=0;i<3;i++) EXPECT_TRUE(expenv.is_user_existing(i));
  for (int i=3;i<10;i++) EXPECT_FALSE(expenv.is_user_existing(i));
}
TEST_F(TestExperimentEnvironment, users_items_noinitall) {
  //data:
  //id=0 t=0 u=0 i=0 s=1
  //id=1 t=1 u=0 i=0 s=1
  //id=2 t=2 u=0 i=0 s=1
  //id=3 t=3 u=0 i=1 s=1
  //id=4 t=4 u=1 i=1 s=1
  //id=5 t=5 u=1 i=1 s=1
  //id=6 t=6 u=1 i=2 s=1
  //id=7 t=7 u=1 i=2 s=1
  //id=8 t=8 u=2 i=2 s=1
  //id=9 t=9 u=2 i=3 s=1

  params.initialize_all = false;
  params.max_user = 2;
  params.max_item = 3;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());
  const vector<int>* items = expenv.get_items();
  const vector<int>* users = expenv.get_users();

  EXPECT_EQ(0,items->size());
  EXPECT_EQ(0,users->size());
  
  expenv.update(rdi.next()); //id=0 t=0 u=0 i=0 s=1
  EXPECT_EQ(1,items->size());
  EXPECT_EQ(1,users->size());

  expenv.update(rdi.next()); //id=1 t=1 u=0 i=0 s=1
  expenv.update(rdi.next()); //id=2 t=2 u=0 i=0 s=1
  expenv.update(rdi.next()); //id=3 t=3 u=0 i=1 s=1
  EXPECT_EQ(2,items->size());
  EXPECT_EQ(1,users->size());

  expenv.update(rdi.next()); //id=4 t=4 u=1 i=1 s=1
  EXPECT_EQ(2,items->size());
  EXPECT_EQ(2,users->size());

  expenv.update(rdi.next()); //id=5 t=5 u=1 i=1 s=1
  expenv.update(rdi.next()); //id=6 t=6 u=1 i=2 s=1
  EXPECT_EQ(3,items->size());
  EXPECT_EQ(2,users->size());

  expenv.update(rdi.next()); //id=7 t=7 u=1 i=2 s=1
  expenv.update(rdi.next()); //id=8 t=8 u=2 i=2 s=1
  EXPECT_EQ(3,items->size());
  EXPECT_EQ(3,users->size());

  expenv.update(rdi.next()); //id=9 t=9 u=2 i=3 s=1
  EXPECT_EQ(4,items->size());
  EXPECT_EQ(3,users->size());
  EXPECT_EQ(0,items->at(0));
  EXPECT_EQ(1,items->at(1));
  EXPECT_EQ(2,items->at(2));
  EXPECT_EQ(3,items->at(3));
  EXPECT_EQ(0,users->at(0));
  EXPECT_EQ(1,users->at(1));
  EXPECT_EQ(2,users->at(2));
}
TEST_F(TestExperimentEnvironment, users_items_initall) {
  //data:
  //id=0 t=0 u=0 i=0 s=1
  //id=1 t=1 u=0 i=0 s=1
  //id=2 t=2 u=0 i=0 s=1
  //id=3 t=3 u=0 i=1 s=1
  //id=4 t=4 u=1 i=1 s=1
  //id=5 t=5 u=1 i=1 s=1
  //id=6 t=6 u=1 i=2 s=1
  //id=7 t=7 u=1 i=2 s=1
  //id=8 t=8 u=2 i=2 s=1
  //id=9 t=9 u=2 i=3 s=1

  params.initialize_all = true;
  params.max_user = 2;
  params.max_item = 3;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());
  const vector<int>* items = expenv.get_items();
  const vector<int>* users = expenv.get_users();
  while(rdi.has_next()){
    EXPECT_EQ(4,items->size());
    EXPECT_EQ(0,items->at(0));
    EXPECT_EQ(1,items->at(1));
    EXPECT_EQ(2,items->at(2));
    EXPECT_EQ(3,items->at(3));
    EXPECT_EQ(3,users->size());
    EXPECT_EQ(0,users->at(0));
    EXPECT_EQ(1,users->at(1));
    EXPECT_EQ(2,users->at(2));
    expenv.update(rdi.next());
  }
}
TEST_F(TestExperimentEnvironment, train_matrix) {
  //data:
  //id=0 t=0 u=0 i=0 s=1
  //id=1 t=1 u=0 i=0 s=1
  //id=2 t=2 u=0 i=0 s=1
  //id=3 t=3 u=0 i=1 s=1
  //id=4 t=4 u=1 i=1 s=1
  //id=5 t=5 u=1 i=1 s=1
  //id=6 t=6 u=1 i=2 s=1
  //id=7 t=7 u=1 i=2 s=1
  //id=8 t=8 u=2 i=2 s=1
  //id=9 t=9 u=2 i=3 s=1

  params.initialize_all = false;
  params.max_user = 2;
  params.max_item = 3;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());

  const SpMatrix* train_matrix = expenv.get_train_matrix();

  EXPECT_EQ(0,train_matrix->size());

  expenv.update(rdi.next()); //id=0 t=0 u=0 i=0 s=1
  EXPECT_EQ(1, train_matrix->size());
  EXPECT_EQ(1, train_matrix->row_size(0));
  EXPECT_EQ(1, train_matrix->get(0,0));

  expenv.update(rdi.next()); //id=1 t=1 u=0 i=0 s=1
  expenv.update(rdi.next()); //id=2 t=2 u=0 i=0 s=1
  expenv.update(rdi.next()); //id=3 t=3 u=0 i=1 s=1
  EXPECT_EQ(1, train_matrix->size());
  EXPECT_EQ(2, train_matrix->row_size(0));
  EXPECT_EQ(1, train_matrix->get(0,0));
  EXPECT_EQ(1, train_matrix->get(0,1));

  expenv.update(rdi.next()); //id=4 t=4 u=1 i=1 s=1
  EXPECT_EQ(2, train_matrix->size());
  EXPECT_EQ(2, train_matrix->row_size(0));
  EXPECT_EQ(1, train_matrix->row_size(1));
  EXPECT_EQ(1, train_matrix->get(0,0));
  EXPECT_EQ(1, train_matrix->get(0,1));
  EXPECT_EQ(1, train_matrix->get(1,1));
}
TEST_F(TestExperimentEnvironment, rdi) {
  //data:
  //id=0 t=0 u=0 i=0 s=1
  //id=1 t=1 u=0 i=0 s=1
  //id=2 t=2 u=0 i=0 s=1
  //id=3 t=3 u=0 i=1 s=1
  //id=4 t=4 u=1 i=1 s=1
  //id=5 t=5 u=1 i=1 s=1
  //id=6 t=6 u=1 i=2 s=1
  //id=7 t=7 u=1 i=2 s=1
  //id=8 t=8 u=2 i=2 s=1
  //id=9 t=9 u=2 i=3 s=1

  params.initialize_all = false;
  params.max_user = 2;
  params.max_item = 3;
  ExperimentEnvironment expenv;
  expenv.set_parameters(&params);
  expenv.set_recommender_data_iterator(&rdi);
  EXPECT_TRUE(expenv.self_test());
  const RecommenderDataIterator* rdi2 = expenv.get_recommender_data_iterator();
  EXPECT_EQ(&rdi,rdi2);
}
int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

