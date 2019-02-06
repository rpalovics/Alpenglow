#include <vector>
#include <gtest/gtest.h>
#include "../../main/recommender_data/RecommenderData.h"


namespace {
RecDat create_rec_dat(double time, int user, int item, int id, int score, int eval, int category=0){
   RecDat rec_dat;
   rec_dat.time = time;
   rec_dat.user = user;
   rec_dat.item = item;
   rec_dat.id = id;
   rec_dat.score = score;
   rec_dat.eval = eval;
   rec_dat.category = category;
   return rec_dat;
}
void  test_recdata_eq(vector<RecDat>* expected_data, vector<RecDat>* data){
  ASSERT_TRUE(expected_data!=NULL);
  ASSERT_TRUE(data!=NULL);
  ASSERT_EQ(expected_data->size(),data->size());
  for(uint i=0;i<expected_data->size();i++){
    EXPECT_EQ(expected_data->at(i).time,data->at(i).time);
    EXPECT_EQ(expected_data->at(i).user,data->at(i).user);
    EXPECT_EQ(expected_data->at(i).item,data->at(i).item);
    EXPECT_EQ(expected_data->at(i).id,data->at(i).id);
    EXPECT_EQ(expected_data->at(i).score,data->at(i).score);
    EXPECT_EQ(expected_data->at(i).eval,data->at(i).eval);
  }
}
bool in(int element, vector<int>* list){
  //element is in the list
  for(uint i=0; i<list->size(); i++){
    if(list->at(i)==element) return true;
  }
  return false;
}

class TestRecommenderData : public ::testing::Test  {
public:
  RecommenderData rd;
  vector<RecDat> data;

  TestRecommenderData() {
    EXPECT_TRUE(rd.initialize());
    data.push_back(create_rec_dat(100,1,1,0,1,1));
    data.push_back(create_rec_dat(100,1,4,1,1,1));
                                           
    data.push_back(create_rec_dat(100,2,1,4,1,1));
    data.push_back(create_rec_dat(100,2,1,6,1,0));
                                           
    data.push_back(create_rec_dat(100,3,5,3,1,1));
    data.push_back(create_rec_dat(100,3,8,2,1,1));
    data.push_back(create_rec_dat(100,3,8,9,1,0));
    data.push_back(create_rec_dat(100,3,5,8,1,1));
    rd.set_rec_data(data);
  }
  virtual ~TestRecommenderData() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
  }
  virtual void TearDown(){
  }
};
class TestLegacyRecommenderData : public ::testing::Test  {
public:
  TestLegacyRecommenderData() {
  }
  virtual ~TestLegacyRecommenderData() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
  }
  virtual void TearDown(){
  }
};

}
TEST_F(TestRecommenderData, test) { //size, set, get, clear
  EXPECT_EQ(data.size(),rd.size());
  vector<RecDat>* data_get = rd.get_rec_data();
  test_recdata_eq(&data,data_get);
  rd.clear();
  EXPECT_EQ(0,rd.size());
  rd.set_rec_data(data);
  EXPECT_EQ(data.size(),rd.size());
}
TEST_F(TestRecommenderData, items) {
  vector<int>* items = rd.get_all_items();
  EXPECT_EQ(4,items->size());
  EXPECT_TRUE(in(1,items));
  EXPECT_TRUE(in(4,items));
  EXPECT_TRUE(in(5,items));
  EXPECT_TRUE(in(8,items));
}
TEST_F(TestRecommenderData, users) {
  vector<int>* users = rd.get_all_users();
  EXPECT_EQ(3,users->size());
  EXPECT_TRUE(in(1,users));
  EXPECT_TRUE(in(2,users));
  EXPECT_TRUE(in(3,users));
}
TEST_F(TestRecommenderData, matrix) {
  SpMatrix* matrix = rd.get_full_matrix();
  EXPECT_EQ(1,matrix->get(1,1));
  EXPECT_EQ(1,matrix->get(1,4));
  EXPECT_EQ(1,matrix->get(2,1));
  EXPECT_EQ(1,matrix->get(3,8));
  EXPECT_EQ(0,matrix->get(2,7));
  EXPECT_EQ(0,matrix->get(4,7));
}

TEST_F(TestLegacyRecommenderData, online) {
  LegacyRecommenderData recommender_data;
  stringstream ifs;
  ifs << "100 1 1 1 1" << endl;
  ifs << "100 1 4 1 1" << endl;

  ifs << "100 2 1 1 1" << endl;
  ifs << "100 2 1 1 0" << endl;
  
  ifs << "100 3 5 1 1" << endl;
  ifs << "100 3 8 1 1" << endl;
  ifs << "100 3 8 1 0" << endl;
  ifs << "100 3 5 1 1";

  string type = "online";
  recommender_data.read_from_file_core(ifs, type);
  vector<RecDat>* data = recommender_data.get_rec_data();

  vector<RecDat> expected_data;
  expected_data.push_back(create_rec_dat(100,1,1,0,1,1));
  expected_data.push_back(create_rec_dat(100,1,4,1,1,1));
                                                  
  expected_data.push_back(create_rec_dat(100,2,1,2,1,1));
  expected_data.push_back(create_rec_dat(100,2,1,3,1,0));
                                                  
  expected_data.push_back(create_rec_dat(100,3,5,4,1,1));
  expected_data.push_back(create_rec_dat(100,3,8,5,1,1));
  expected_data.push_back(create_rec_dat(100,3,8,6,1,0));
  expected_data.push_back(create_rec_dat(100,3,5,7,1,1));

  test_recdata_eq(&expected_data,data);
}
TEST_F(TestLegacyRecommenderData, online_id) {
  LegacyRecommenderData recommender_data;
  stringstream ifs;
  ifs << "100 1 1 0 1 1" << endl;
  ifs << "100 1 4 1 1 1" << endl;

  ifs << "100 2 1 4 1 1" << endl;
  ifs << "100 2 1 6 1 0" << endl;
  
  ifs << "100 3 5 3 1 1" << endl;
  ifs << "100 3 8 2 1 1" << endl;
  ifs << "100 3 8 9 1 0" << endl;
  ifs << "100 3 5 8 1 1";

  string type = "online_id";
  recommender_data.read_from_file_core(ifs, type);
  vector<RecDat>* data = recommender_data.get_rec_data();

  vector<RecDat> expected_data;
  expected_data.push_back(create_rec_dat(100,1,1,0,1,1));
  expected_data.push_back(create_rec_dat(100,1,4,1,1,1));
                                                  
  expected_data.push_back(create_rec_dat(100,2,1,4,1,1));
  expected_data.push_back(create_rec_dat(100,2,1,6,1,0));
                                                  
  expected_data.push_back(create_rec_dat(100,3,5,3,1,1));
  expected_data.push_back(create_rec_dat(100,3,8,2,1,1));
  expected_data.push_back(create_rec_dat(100,3,8,9,1,0));
  expected_data.push_back(create_rec_dat(100,3,5,8,1,1));

  test_recdata_eq(&expected_data,data);
}
TEST_F(TestLegacyRecommenderData, offline) {
  LegacyRecommenderData recommender_data;
  stringstream ifs;
  ifs << "1 1 1" << endl;
  ifs << "1 4 1" << endl;

  ifs << "2 1 1" << endl;
  ifs << "2 1 1" << endl;
  
  ifs << "3 5 1" << endl;
  ifs << "3 8 1" << endl;
  ifs << "3 8 1" << endl;
  ifs << "3 5 1";

  string type = "offline";
  recommender_data.read_from_file_core(ifs, type);
  vector<RecDat>* data = recommender_data.get_rec_data();

  vector<RecDat> expected_data;
  expected_data.push_back(create_rec_dat(0,1,1,0,1,1));
  expected_data.push_back(create_rec_dat(0,1,4,1,1,1));
                                               
  expected_data.push_back(create_rec_dat(0,2,1,2,1,1));
  expected_data.push_back(create_rec_dat(0,2,1,3,1,1));
                                               
  expected_data.push_back(create_rec_dat(0,3,5,4,1,1));
  expected_data.push_back(create_rec_dat(0,3,8,5,1,1));
  expected_data.push_back(create_rec_dat(0,3,8,6,1,1));
  expected_data.push_back(create_rec_dat(0,3,5,7,1,1));

  test_recdata_eq(&expected_data,data);
}
TEST_F(TestLegacyRecommenderData, offline_timestamp) {
  LegacyRecommenderData recommender_data;
  stringstream ifs;
  ifs << "13 1 1 1" << endl;
  ifs << "14 1 4 1" << endl;

  ifs << "12 2 1 1" << endl;
  ifs << "19 2 1 1" << endl;

  ifs << "13 3 5 1" << endl;
  ifs << "14 3 8 1" << endl;
  ifs << "12 3 8 1" << endl;
  ifs << "11 3 5 1";

  string type = "offlineTimestamp";
  recommender_data.read_from_file_core(ifs, type);
  vector<RecDat>* data = recommender_data.get_rec_data();

  vector<RecDat> expected_data;
  expected_data.push_back(create_rec_dat(13,1,1,0,1,1));
  expected_data.push_back(create_rec_dat(14,1,4,1,1,1));
                                                
  expected_data.push_back(create_rec_dat(12,2,1,2,1,1));
  expected_data.push_back(create_rec_dat(19,2,1,3,1,1));
                                                
  expected_data.push_back(create_rec_dat(13,3,5,4,1,1));
  expected_data.push_back(create_rec_dat(14,3,8,5,1,1));
  expected_data.push_back(create_rec_dat(12,3,8,6,1,1));
  expected_data.push_back(create_rec_dat(11,3,5,7,1,1));

  test_recdata_eq(&expected_data,data);
}
TEST_F(TestLegacyRecommenderData, category) {
  LegacyRecommenderData recommender_data;
  stringstream ifs;
  ifs << "100 1 1 1 1 3" << endl;
  ifs << "100 1 4 1 1 4" << endl;

  ifs << "100 2 1 1 1 3" << endl;
  ifs << "100 2 1 1 0 4" << endl;
  
  ifs << "100 3 5 1 1 3" << endl;
  ifs << "100 3 8 1 1 6" << endl;
  ifs << "100 3 8 1 0 2" << endl;
  ifs << "100 3 5 1 1 3";

  string type = "category";
  recommender_data.read_from_file_core(ifs, type);
  vector<RecDat>* data = recommender_data.get_rec_data();

  vector<RecDat> expected_data;
  expected_data.push_back(create_rec_dat(100,1,1,0,1,1,3));
  expected_data.push_back(create_rec_dat(100,1,4,1,1,1,4));

  expected_data.push_back(create_rec_dat(100,2,1,2,1,1,3));
  expected_data.push_back(create_rec_dat(100,2,1,3,1,0,4));

  expected_data.push_back(create_rec_dat(100,3,5,4,1,1,3));
  expected_data.push_back(create_rec_dat(100,3,8,5,1,1,6));
  expected_data.push_back(create_rec_dat(100,3,8,6,1,0,2));
  expected_data.push_back(create_rec_dat(100,3,5,7,1,1,3));

  test_recdata_eq(&expected_data,data);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

