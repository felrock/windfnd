#include <gtest/gtest.h>
#include "util.h" // Ensure this includes the declaration of get_day function and days_of_week array

using namespace util;

// Test fixture for the get_day function
class GetDayTest : public ::testing::Test {};

// Test valid dates with time
TEST_F(GetDayTest, ValidDatesWithTime) {
    std::string day;

    // Check a few known dates with time
    EXPECT_EQ(get_day("2024-05-17T01:00", day), 0);
    EXPECT_EQ(day, "Friday");

    EXPECT_EQ(get_day("2023-12-25T15:30", day), 0);
    EXPECT_EQ(day, "Monday");

    EXPECT_EQ(get_day("2000-01-01T12:00", day), 0);
    EXPECT_EQ(day, "Saturday");
}

// Test invalid date formats
TEST_F(GetDayTest, InvalidDateFormats) {
    std::string day;

    // Check various invalid date formats
    EXPECT_EQ(get_day("2024/05/17T01:00", day), 1);
    EXPECT_EQ(get_day("17-05-2024T01:00", day), 1);
    EXPECT_EQ(get_day("May 17, 2024 01:00", day), 1);
    EXPECT_EQ(get_day("2024-5-17T01:00", day), 1);
    EXPECT_EQ(get_day("2024-05-17", day), 1);  // Missing time part
}

// Test invalid dates
TEST_F(GetDayTest, InvalidDates) {
    std::string day;

    // Check out of range and nonsensical dates
    EXPECT_EQ(get_day("2024-13-17T01:00", day), 1);
    EXPECT_EQ(get_day("2024-00-17T01:00", day), 1);
    EXPECT_EQ(get_day("2024-01-32T01:00", day), 1);
    EXPECT_EQ(get_day("2024-02-30T01:00", day), 1);
    EXPECT_EQ(get_day("", day), 1);
}

// Test edge cases
TEST_F(GetDayTest, EdgeCases) {
    std::string day;

    // Check leap year
    EXPECT_EQ(get_day("2020-02-29T01:00", day), 0);
    EXPECT_EQ(day, "Saturday");

    // Check non-leap year
    EXPECT_EQ(get_day("2019-02-28T01:00", day), 0);
    EXPECT_EQ(day, "Thursday");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
