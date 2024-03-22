#include "gtest/gtest.h"
#include <utility>
#include <cstdio>
#include "test_utils.h"
#include "file_tape.h"


TEST(ConfigTest, ltrim_test)
{
    std::string test1("     test");
    std::string test2("\t 82384762");
    std::string test3("   \t23   1312");
    file_tape::config::ltrim(test1);
    file_tape::config::ltrim(test2);
    file_tape::config::ltrim(test3);
    EXPECT_EQ(test1, "test");
    EXPECT_EQ(test2, "82384762");
    EXPECT_EQ(test3, "23   1312");
}

TEST(ConfigTest, rtrim_test)
{
    std::string test1("test      ");
    std::string test2("82384762    \t");
    std::string test3("23   1312\t      \t");
    file_tape::config::rtrim(test1);
    file_tape::config::rtrim(test2);
    file_tape::config::rtrim(test3);
    EXPECT_EQ(test1, "test");
    EXPECT_EQ(test2, "82384762");
    EXPECT_EQ(test3, "23   1312");
}

TEST(ConfigTest, full_trim_test)
{
    std::string test1("         test      ");
    std::string test2("\t\t    82384762    \t");
    std::string test3("\t\t      23   1312\t      \t");
    file_tape::config::full_trim(test1);
    file_tape::config::full_trim(test2);
    file_tape::config::full_trim(test3);
    EXPECT_EQ(test1, "test");
    EXPECT_EQ(test2, "82384762");
    EXPECT_EQ(test3, "23   1312");
}

TEST(ConfigTest, get_keyword_and_value_test)
{
    std::string test("r=1");
    auto equals_pos = test.find("=");
    auto pair = std::make_pair(std::string("r"), 1);
    ASSERT_EQ(file_tape::config::get_keyword_and_value(test, equals_pos), pair);

    test = "read=1";
    equals_pos = test.find("=");
    pair = std::make_pair(std::string("read"), 1);
    ASSERT_EQ(file_tape::config::get_keyword_and_value(test, equals_pos), pair);

    test = "\t\tr  =    \t1\t";
    equals_pos = test.find("=");
    pair = std::make_pair(std::string("r"), 1);
    ASSERT_EQ(file_tape::config::get_keyword_and_value(test, equals_pos), pair);

    test = "r=nkghl1";
    equals_pos = test.find("=");
    pair = std::make_pair(std::string("r"), -1);
    ASSERT_EQ(file_tape::config::get_keyword_and_value(test, equals_pos), pair);
}

TEST(ConfigTest, emptyConfigTest)
{
    ASSERT_THROW(file_tape::config("non_existing_config.cfg"), std::runtime_error);
}

TEST(ConfigTest, regularConfigTest)
{
    auto config_file_name = create_temp_config("r", 1, "w", 1, "rd", 1, "mv", 1);

    file_tape::config cfg{config_file_name};
    file_tape::config expected;
    expected.readTime = std::chrono::milliseconds(1);
    expected.writeTime = std::chrono::milliseconds(1);
    expected.rewindTime = std::chrono::milliseconds(1);
    expected.moveTime = std::chrono::milliseconds(1);
    EXPECT_EQ(cfg, expected);
    std::remove(config_file_name.c_str());
}

TEST(ConfigTest, configWithMissedArgNames)
{
    auto config_file_name = create_temp_config("r", 1, "w", 1, "abracadabra", 1, "mv", 1);

    file_tape::config cfg{config_file_name};
    file_tape::config expected;
    expected.readTime = std::chrono::milliseconds(1);
    expected.writeTime = std::chrono::milliseconds(1);
    expected.rewindTime = std::chrono::milliseconds(0);
    expected.moveTime = std::chrono::milliseconds(1);
    EXPECT_EQ(cfg, expected);
    std::remove(config_file_name.c_str());
}