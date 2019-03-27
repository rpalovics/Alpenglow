#include <vector>
#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "../../main/utils/Factors.h"


namespace {

class TestFactors : public ::testing::Test  {
public:
  Factors factors;
  TestFactors() {
  }
  virtual ~TestFactors() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
  virtual void SetUp(){
    FactorsParameters parameters;
    parameters.dimension = 2;
    parameters.begin_min = -1;
    parameters.begin_max = 1;
    factors.set_parameters(parameters);
  }
  virtual void TearDown(){
  }
};

}
TEST_F(TestFactors, set) {
   vector <double> tester;
   tester.push_back(1);
   tester.push_back(2);
   factors.set(1,&tester);
   vector <double> * factor = factors.get(1);
   EXPECT_EQ(1,(*factor)[0]);
   EXPECT_EQ(2,(*factor)[1]);
}

TEST_F(TestFactors, setField) {
   factors.init(2);
   factors.set(2, 1, 13);
   factors.set(2, 0, -2);
   vector <double> * factor = factors.get(2);
   EXPECT_EQ(-2,(*factor)[0]);
   EXPECT_EQ(13,(*factor)[1]);
}

TEST_F(TestFactors, set_zero) {
   factors.resize(2);
   factors.set_zero(2);
   vector <double> * factor = factors.get(2);
   EXPECT_EQ(0,(*factor)[0]);
   EXPECT_EQ(0,(*factor)[1]);
}

TEST_F(TestFactors, set_rand) {
   factors.resize(0);
   factors.set_rand(0);
   vector <double> * factor = factors.get(0);
   EXPECT_LE(-1,(*factor)[0]);
   EXPECT_LE(-1,(*factor)[1]);
   EXPECT_GE(1,(*factor)[0]);
   EXPECT_GE(1,(*factor)[1]);
}

TEST_F(TestFactors, init) {
   factors.init(0);
   vector <double> * factor = factors.get(0);
   EXPECT_LE(-1,(*factor)[0]);
   EXPECT_LE(-1,(*factor)[1]);
   EXPECT_GE(1,(*factor)[0]);
   EXPECT_GE(1,(*factor)[1]);

   factors.init(3);
   factor = factors.get(3);
   EXPECT_LE(-1,(*factor)[0]);
   EXPECT_LE(-1,(*factor)[1]);
   EXPECT_GE(1,(*factor)[0]);
   EXPECT_GE(1,(*factor)[1]);
}

TEST_F(TestFactors, add) {
   factors.init(1);
   factors.set_zero(1);
   vector <double> tester;
   tester.push_back(1);
   tester.push_back(2);
   factors.add(1,&tester);
   vector <double> * factor = factors.get(1);
   EXPECT_EQ(1,(*factor)[0]);
   EXPECT_EQ(2,(*factor)[1]);
}

TEST_F(TestFactors, addField) {
   factors.init(2);
   factors.set_zero(2);
   factors.add(2, 1, 13);
   factors.add(2, 0, -2);
   vector <double> * factor = factors.get(2);
   EXPECT_EQ(-2,(*factor)[0]);
   EXPECT_EQ(13,(*factor)[1]);
}
TEST_F(TestFactors, lin_combine) {
   vector <double> tester;
   tester.push_back(1);
   tester.push_back(2);
   factors.set(1,&tester);
   factors.lin_combine(1,0.1,&tester);
   vector <double> * factor = factors.get(1);
   cerr << (*factor)[0] << " " << (*factor)[1] << endl;
   EXPECT_DOUBLE_EQ(1.1,(*factor)[0]);
   EXPECT_DOUBLE_EQ(2.2,(*factor)[1]);
}

TEST_F(TestFactors, persistency){
   FactorsParameters parameters;
   parameters.dimension = 2;
   parameters.begin_min = -1;
   parameters.begin_max = 1;
   Factors factors(parameters);
   vector <double> tester;
   tester.push_back(1);
   tester.push_back(2);
   factors.set(1,&tester);
   tester[0]=3;
   tester[1]=6;
   factors.set(4, &tester);
   ofstream ofs("testFile.txt", ios::binary);
   factors.write(ofs);
   ofs.close();

   Factors factors2(parameters);
   ifstream ifs("testFile.txt", ios::binary);
   factors2.read(ifs);
   ifs.close();
   vector<double> * vector2 = factors2.get(1);
   ASSERT_TRUE(vector2!=NULL);
   EXPECT_EQ(1, vector2->at(0));
   EXPECT_EQ(2, vector2->at(1));
   vector2 = factors2.get(4);
   ASSERT_TRUE(vector2!=NULL);
   EXPECT_EQ(3, vector2->at(0));
   EXPECT_EQ(6, vector2->at(1));
   remove("testFile.txt");
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

