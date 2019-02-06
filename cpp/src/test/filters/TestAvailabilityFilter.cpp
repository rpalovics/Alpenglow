#include <gtest/gtest.h>
#include "../../main/filters/AvailabilityFilter.h"


namespace {
	class TestAvailabilityFilter : public ::testing::Test { 
	  public:
	    TestAvailabilityFilter(){}
	    virtual ~TestAvailabilityFilter(){}
	    virtual void SetUp(){} 
	    virtual void TearDown(){}
	};
}

TEST_F(TestAvailabilityFilter, test){
	AvailabilityFilter filter;
	EXPECT_EQ(0,filter.availabilites_.size());
	filter.add_availability(10, 1, 100);
	EXPECT_EQ(1,filter.availabilites_.size());

	filter.add_availability(20, 2, 100);
	filter.add_availability(30, 3, 90);
	filter.add_availability(20, 4, 100);
	filter.add_availability(50, 5, 100);
	filter.add_availability(100, 5, 100);

	RecDat d;
	d.time = 10;
	filter.run(&d);

	EXPECT_EQ(1,filter.available_items_set_.size());
	EXPECT_EQ(5,filter.availabilites_.size());
	EXPECT_EQ(1,filter.availability_ends_.size());
	EXPECT_EQ(110,get<0>(filter.availability_ends_.top()));
	EXPECT_EQ(1,get<1>(filter.availability_ends_.top()));
	EXPECT_EQ(1, filter.get_global_items()->size());
	EXPECT_EQ(1, (*filter.get_global_items())[0].first);
	d.item = 1;
	EXPECT_TRUE(filter.active(&d));
	d.item = 2;
	EXPECT_FALSE(filter.active(&d));
	d.item = 3;
	EXPECT_FALSE(filter.active(&d));

	d.time = 20;
	filter.run(&d);
	EXPECT_EQ(3, filter.get_global_items()->size());
	/*auto items =*/ filter.get_global_items();
	auto expected_set = set<pair<int,double>>({
		make_pair(1,-1.0),
		make_pair(2,-1.0),
		make_pair(4,-1.0),
	});
	EXPECT_TRUE(expected_set == filter.available_items_set_);
	d.item = 1;
	EXPECT_TRUE(filter.active(&d));
	d.item = 2;
	EXPECT_TRUE(filter.active(&d));
	d.item = 3;
	EXPECT_FALSE(filter.active(&d));
	d.item = 4;
	EXPECT_TRUE(filter.active(&d));

	d.time = 120;
	filter.run(&d);
	expected_set = set<pair<int,double>>({
		make_pair(3,-1.0),
		make_pair(5,-1.0),
	});
	EXPECT_TRUE(expected_set == filter.available_items_set_);

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
