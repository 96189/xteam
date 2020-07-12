
#include "parse_conf.h"
#include "gtest/gtest.h"

TEST(ParseConfTest, confParse) {
    int ret = InitConf("./test.conf");
    EXPECT_EQ(-1, ret);
    
    ret = InitConf("./conf/web.conf");
    EXPECT_EQ(0, ret);
}
