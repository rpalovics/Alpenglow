#include <gtest/gtest.h>
#include "../../main/utils/TrackDataContainer.h"

namespace {
  class TestTrackDataContainer : public ::testing::Test {
  public:
    TestTrackDataContainer(){}
    virtual ~TestTrackDataContainer(){}
    virtual void SetUp(){}
    virtual void TearDown(){}
  };
};

class TrackDataContainerToTest : public TrackDataContainer {
public:
  TrackDataContainerToTest(TrackDataContainerParameters* p) : TrackDataContainer(p){
    read_file(p->file_name.c_str());
  }
protected:
  virtual void read_file(const char* fname){
    std::vector<std::string> file_lines;
    file_lines = std::vector<std::string>{
      "10000009 906223 30363 152",
      "10000010 906224 30363 152",
      "10000035 906228 30363 152",
      "10000036 906229 30363 152",
      "10000037 906231 30363 152",
      "10000048 906232 30363 152",
      "10000115 906245 10315 152",
      "10000133 906247 1741633 3",
      "10000135 906247 1741633 3"
    };
    for (std::string buffer : file_lines) {
      process_line(buffer);
    }
    std::cout << "track data vector size: " << (int)track_attributes_.size() << std::endl;
  }
};

TEST_F(TestTrackDataContainer, test1) {
  std::string file_name("/mnt/idms/pszalai/sandbox/testfiles/TestTrackDataContainer");
  TrackDataContainerParameters tdcp;
  tdcp.file_name = file_name;
  TrackDataContainerToTest tdc(&tdcp);
  std::vector<std::vector<int> > v = tdc.get_data(10000009);
  std::cout << "get data done " << std::endl;
  std::cout << "size " << v.size() << std::endl;
  EXPECT_EQ(906223,v[0][0]);
  EXPECT_EQ(152,v[2][0]);
  EXPECT_EQ(30363,v[1][0]);
  v = tdc.get_data(10000135);
  EXPECT_EQ(906247,v[0][0]);
  EXPECT_EQ(3,v[2][0]);
  EXPECT_EQ(1741633,v[1][0]);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
