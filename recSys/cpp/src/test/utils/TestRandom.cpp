
#include <gtest/gtest.h>
#include "../../main/utils/Random.h"
#include <math.h>

namespace {

class TestRandom : public ::testing::Test{
};
} //namespace

TEST_F(TestRandom, max){
  Random random(234523523);
  EXPECT_LT(random.get(10),10);
  EXPECT_LT(random.get(5),5);
  EXPECT_EQ(random.get(1),0);
}
 //TODO
TEST_F(TestRandom, seed){
  Random random1(123456);
  Random random2(123456);
  EXPECT_EQ(random1.get(10), random2.get(10));
  EXPECT_EQ(random1.get(234), random2.get(234));
  EXPECT_EQ(random1.get(35), random2.get(35));
  EXPECT_EQ(random1.get(4), random2.get(4));
}

TEST_F(TestRandom, setSeed){
  Random random1(3434123);
  Random random2;
  random2.set(3434123);
  for(int i=0;i<100;i++){
    EXPECT_EQ(random1(10),random2(10));
  }
}

TEST_F(TestRandom, persistence){
  Random random(123123);
  random(10);
  random(10);
  random(10);
  ofstream ofs("testRandom.txt", ios::binary);
  random.write(ofs);
  ofs.close();
  Random random2;
  ifstream ifs("testRandom.txt", ios::binary);
  random2.read(ifs);
  ifs.close();
  for(int i=0;i<100;i++){
    EXPECT_EQ(random(10),random2(10));
  }
  remove("testRandom.txt");
}


//algoritmusfuggo tesztek: ha mas lesz a belseje, lehet, hogy baj lesz veluk
TEST_F(TestRandom, distribution){
  Random random(3423452);
  vector<int> distrib(10);
  for(int i=0;i<100000;i++){
    distrib[random(distrib.size())]++;
  }
  for(int i=0;i<distrib.size();i++){
    EXPECT_LT(100000/10*0.9,distrib[i]);
    EXPECT_GT(100000/10*1.1,distrib[i]);
  }
}

TEST_F(TestRandom, get_linear){
  Random random(3423452);
  vector<int> distrib(20);
  for(int i=0;i<20000000;i++){
    distrib[random.get_linear(distrib.size())]++;
  }
  for(int i=0;i<distrib.size();i++){
    EXPECT_LT(2*i+0.8, distrib[i]/(double)distrib[0]);
    EXPECT_GT(2*i+1.2, distrib[i]/(double)distrib[0]);
  }
}


TEST_F(TestRandom, get_arctg){
  Random random(3423452);
  int distrsize = 20;
  vector<int> distrib(distrsize);
  int all=200000;
  int y=50;
  for(int i=0;i<all;i++){
    distrib[random.get_arctg(y,distrib.size())]++;
  }
  vector<double> theor_distrib(distrsize);
  for(int i = 0; i < distrsize;i++) {
    theor_distrib[i]= atan((double)(i+1)*y/distrsize)/atan(y)-atan((double)i*y/distrsize)/atan(y);
  }
  for(int i=0;i<distrib.size();i++){
    EXPECT_LT(theor_distrib[i]*0.7, (double)distrib[i]/all);
    EXPECT_GT(theor_distrib[i]*1.3, (double)distrib[i]/all);
  }
}


/*
TEST_F(TestRandom, get_geometric2){
  Random random(3423452);
  int max = 10;
  vector<int> distrib(max+1);
  int all=200000;
  double p=0.2;
  for(int i=0;i<all;i++){
    distrib.at(random.get_geometric(max,p,1))++;
  }
  vector<double> theor_distrib(max);
  for(int i = 0; i < max+1;i++) {
    //theor_distrib[i]= atan((double)(i+1)*y/distrsize)/atan(y)-atan((double)i*y/distrsize)/atan(y);
    cout << i << ": " << distrib.at(i) << endl;
  }
    cout <<"asd"<< endl;
  /*for(int i=0;i<max;i++){
    EXPECT_LT(theor_distrib[i]*0.7, (double)distrib[i]/all);
    EXPECT_GT(theor_distrib[i]*1.3, (double)distrib[i]/all);
  }
}*/

TEST_F(TestRandom, get_geometric){
  Random random(3423452);
  int max = 10;
  vector<int> distrib(max);
  int all=200000;
  double p=0.8;
  for(int i=0;i<all;i++){
    distrib[random.get_geometric(p,max)]++;
  }
  vector<double> theor_distrib(max);
  for(int i = 0; i < max;i++) {
    theor_distrib[i]= (1-p)*pow(p,i);
  }
  for(int i=0;i<max;i++){
    EXPECT_LT(theor_distrib[i]*0.8, (double)distrib[i]/all);
    EXPECT_GT(theor_distrib[i]*1.2, (double)distrib[i]/all);
  }
}


TEST_F(TestRandom, zero){
  Random random(0);
  for(int i=0;i<100000;i++){
    EXPECT_EQ(0, random(4));
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
