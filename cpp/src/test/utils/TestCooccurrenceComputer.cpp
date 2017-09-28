#include <gtest/gtest.h>
#include "../../main/utils/CooccurrenceComputer.h"


namespace {

class TestCooccurrenceComputer : public ::testing::Test  {
public:
  TestCooccurrenceComputer() {
  }
  virtual ~TestCooccurrenceComputer() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){}
  virtual void TearDown(){}
  bool contains(set<ITEM>& my_set, ITEM i){
    auto it = my_set.find(i);
    return (it!=my_set.end());
  }
  RecDat create_rec_dat(double time, int user, int item, int id, int score, int eval){
     RecDat rec_dat;
     rec_dat.time = time;
     rec_dat.user = user;
     rec_dat.item = item;
     rec_dat.id = id;
     rec_dat.score = score;
     rec_dat.eval = eval;
     return rec_dat;
  }
};

} //namespace

TEST_F(TestCooccurrenceComputer, run) {
  RecommenderData recommender_data;
  vector<RecDat> rec_data;
  rec_data.push_back(create_rec_dat(100,1,1,0,1,1));
  rec_data.push_back(create_rec_dat(100,1,2,0,1,1));
  rec_data.push_back(create_rec_dat(100,1,3,0,1,1));
  rec_data.push_back(create_rec_dat(100,1,4,0,1,1));

  rec_data.push_back(create_rec_dat(100,2,1,0,1,1));
  rec_data.push_back(create_rec_dat(100,2,1,0,1,1));
  rec_data.push_back(create_rec_dat(100,2,1,0,1,1));
  rec_data.push_back(create_rec_dat(100,2,1,0,1,1));

  rec_data.push_back(create_rec_dat(100,3,5,0,1,1));
  rec_data.push_back(create_rec_dat(100,3,8,0,1,1));
  rec_data.push_back(create_rec_dat(100,3,8,0,1,1));
  rec_data.push_back(create_rec_dat(100,3,5,0,1,1));

  rec_data.push_back(create_rec_dat(100,4,1,0,1,1));
  rec_data.push_back(create_rec_dat(100,4,3,0,1,1));

  rec_data.push_back(create_rec_dat(100,5,1,0,1,1));
  rec_data.push_back(create_rec_dat(100,5,3,0,1,1));

  rec_data.push_back(create_rec_dat(100,6,1,0,1,1));
  rec_data.push_back(create_rec_dat(100,6,3,0,1,1));
  recommender_data.set_rec_data(rec_data);

  vector<set<ITEM>> users_items;
  vector<map<ITEM,WEIGHT>> edge_weights;
  stringstream ofs;
  CooccurrenceComputer cocmp;
  cocmp.set_parameters(5,3); //max_len=5, slice_size=3
  cocmp.run(recommender_data, users_items, edge_weights, ofs);
  stringstream expected_ofs;
  expected_ofs << "0 0 -1" << endl;
  expected_ofs << "1 3 -1 3,4 2,1 4,1" << endl;
  expected_ofs << "2 3 -1 1,1 3,1 4,1" << endl;
  expected_ofs << "3 3 -1 1,4 2,1 4,1" << endl;
  expected_ofs << "4 3 -1 1,1 2,1 3,1" << endl;
  expected_ofs << "5 1 -1 8,1" << endl;
  expected_ofs << "6 0 -1" << endl;
  expected_ofs << "7 0 -1" << endl;
  expected_ofs << "8 1 -1 5,1" << endl;
  
  while(!expected_ofs.fail()){
    string line, expected_line;
    getline(ofs,line);
    getline(expected_ofs,expected_line);
    if(expected_ofs.eof()) break;
    EXPECT_EQ(expected_line,line);
    //cerr << "ex: " << expected_line << " get: " << line << endl;
  }
  //EXPECT_TRUE(ofs.eof()); //OPTP
}

TEST_F(TestCooccurrenceComputer, compute_itemset_for_users){
  LegacyRecommenderData recommender_data;
  stringstream ifs;
  ifs << "100 1 1 0 1 1" << endl;
  ifs << "100 1 2 0 1 1" << endl;
  ifs << "100 1 3 0 1 1" << endl;
  ifs << "100 1 4 0 1 1" << endl;

  ifs << "100 2 1 0 1 1" << endl;
  ifs << "100 2 1 0 1 1" << endl;
  ifs << "100 2 1 0 1 1" << endl;
  ifs << "100 2 1 0 1 1" << endl;
  
  ifs << "100 3 5 0 1 1" << endl;
  ifs << "100 3 6 0 1 1" << endl;
  ifs << "100 3 6 0 1 1" << endl;
  ifs << "100 3 5 0 1 1" << endl;

  ifs << "100 4 1 0 1 1" << endl;
  ifs << "100 4 3 0 1 1" << endl;

  ifs << "100 5 1 0 1 1" << endl;
  ifs << "100 5 3 0 1 1" << endl;

  ifs << "100 6 1 0 1 1" << endl;
  ifs << "100 6 3 0 1 1";
  string type = "online_id";
  recommender_data.read_from_file_core(ifs, type);
  vector<set<ITEM>> users_items;
  CooccurrenceComputer cocmp;
  cocmp.set_parameters(4,4); //slice 0: items 0-3, slice 1: items 4-8
  int maxitem = cocmp.compute_itemset_for_users(recommender_data, users_items);
  EXPECT_EQ(6,maxitem);
  ASSERT_EQ(7,users_items.size());
  EXPECT_EQ(0,users_items[0].size());
  EXPECT_EQ(4,users_items[1].size());
  EXPECT_EQ(1,users_items[1].count(1));
  EXPECT_EQ(1,users_items[1].count(2));
  EXPECT_EQ(1,users_items[1].count(3));
  EXPECT_EQ(1,users_items[1].count(4));
  EXPECT_EQ(1,users_items[2].size());
  EXPECT_EQ(1,users_items[2].count(1));
  EXPECT_EQ(2,users_items[3].size());
  EXPECT_EQ(1,users_items[3].count(5));
  EXPECT_EQ(1,users_items[3].count(6));
  EXPECT_EQ(2,users_items[4].size());
  EXPECT_EQ(1,users_items[4].count(1));
  EXPECT_EQ(1,users_items[4].count(3));
  EXPECT_EQ(2,users_items[5].size());
  EXPECT_EQ(1,users_items[5].count(1));
  EXPECT_EQ(1,users_items[5].count(3));
  EXPECT_EQ(2,users_items[6].size());
  EXPECT_EQ(1,users_items[6].count(1));
  EXPECT_EQ(1,users_items[6].count(3));
}
TEST_F(TestCooccurrenceComputer, compute_edge_weights){
  vector<set<ITEM>> users_items;
  users_items.resize(6);
  users_items[1].insert(3);
  users_items[1].insert(1);
  users_items[1].insert(6);
  users_items[1].insert(2);

  users_items[5].insert(2);
  users_items[5].insert(4);
  users_items[5].insert(3);
  users_items[5].insert(2);

  users_items[2].insert(2);
  users_items[2].insert(2);
  users_items[2].insert(2);

  users_items[3].insert(4);
  users_items[3].insert(4);
  vector<map<ITEM,WEIGHT>> edge_weights;
  CooccurrenceComputer cocmp;
  cocmp.set_parameters(4,4); //slice 0: items 0-3, slice 1: items 4-8
  cocmp.compute_edge_weights(users_items,edge_weights,0);
  ASSERT_EQ(4,edge_weights.size());
  EXPECT_EQ(0,edge_weights[0].size());
  EXPECT_EQ(3,edge_weights[1].size());
  EXPECT_EQ(1,edge_weights[1][2]);
  EXPECT_EQ(1,edge_weights[1][3]);
  EXPECT_EQ(1,edge_weights[1][6]);
  EXPECT_EQ(4,edge_weights[2].size());
  EXPECT_EQ(1,edge_weights[2][1]);
  EXPECT_EQ(1,edge_weights[2][6]);
  EXPECT_EQ(1,edge_weights[2][4]);
  EXPECT_EQ(2,edge_weights[2][3]);
  EXPECT_EQ(4,edge_weights[3].size());
  EXPECT_EQ(1,edge_weights[3][1]);
  EXPECT_EQ(1,edge_weights[3][6]);
  EXPECT_EQ(1,edge_weights[3][4]);
  EXPECT_EQ(2,edge_weights[3][2]);

  edge_weights.clear();
  cocmp.compute_edge_weights(users_items,edge_weights,1);
  //ASSERT_EQ(3,edge_weights.size()); //OPTP D
  ASSERT_LE(3,edge_weights.size()); //OPTP A
  EXPECT_EQ(2,edge_weights[0].size()); //4
  EXPECT_EQ(1,edge_weights[0][2]);
  EXPECT_EQ(1,edge_weights[0][3]);
  EXPECT_EQ(0,edge_weights[1].size()); //5
  EXPECT_EQ(3,edge_weights[2].size()); //6
  EXPECT_EQ(1,edge_weights[2][1]);
  EXPECT_EQ(1,edge_weights[2][2]);
  EXPECT_EQ(1,edge_weights[2][3]);
}
TEST_F(TestCooccurrenceComputer, write_toplists){
  vector<map<ITEM,WEIGHT>> edge_weights;
  edge_weights.resize(4);
  edge_weights[3][1]=4;
  edge_weights[3][3]=2;
  edge_weights[3][5]=9;
  edge_weights[2][4]=9;
  for(int i=0;i<10;i++){
    edge_weights[1][i]=(i+5)%12;
  }
  stringstream ofs;
  CooccurrenceComputer cocmp;
  cocmp.set_parameters(5,5);
  cocmp.write_toplists(edge_weights,ofs,0);
  string line;
  getline(ofs,line);
  EXPECT_EQ("0 0 -1", line);
  getline(ofs,line);
  EXPECT_EQ("1 10 -1 6,11 5,10 4,9 3,8 2,7", line);
  getline(ofs,line);
  EXPECT_EQ("2 1 -1 4,9", line);
  getline(ofs,line);
  EXPECT_EQ("3 3 -1 5,9 1,4 3,2", line);

  cocmp.write_toplists(edge_weights,ofs,3);
  getline(ofs,line);
  EXPECT_EQ("15 0 -1", line);
  getline(ofs,line);
  EXPECT_EQ("16 10 -1 6,11 5,10 4,9 3,8 2,7", line);
  getline(ofs,line);
  EXPECT_EQ("17 1 -1 4,9", line);
  getline(ofs,line);
  EXPECT_EQ("18 3 -1 5,9 1,4 3,2", line);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

