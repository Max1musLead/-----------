#include "lib/3DArray.h"
#include <gtest/gtest.h>

TEST(TriDimArrayTestSuite, DoArrayTest) {
    ASSERT_NO_THROW(BigArray test_arr = BigArray::make_array(5, 5, 5));
}

TEST(TriDimArrayTestSuite, ConstructorTest) {
    BigArray test_arr = BigArray::make_array(100, 120, 150);

    ASSERT_EQ(test_arr.GetX(), 100);
    ASSERT_EQ(test_arr.GetY(), 120);
    ASSERT_EQ(test_arr.GetZ(), 150);
}

TEST(TriDimArrayTestSuite, AccordanceTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);

    test_arr[0][0][0] = 25;

    ASSERT_EQ(static_cast<int>(test_arr[0][0][0]), 25);
}

TEST(TriDimArrayTestSuite, ManyAccordanceTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);

    test_arr[0][0][0] = 25;
    test_arr[0][0][1] = 12500;
    test_arr[0][1][0] = 120000;
    test_arr[0][0][2] = 17;

    ASSERT_EQ(static_cast<int>(test_arr[0][0][0]), 25);
    ASSERT_EQ(static_cast<int>(test_arr[0][0][1]), 12500);
    ASSERT_EQ(static_cast<int>(test_arr[0][1][0]), 120000);
    ASSERT_EQ(static_cast<int>(test_arr[0][0][2]), 17);
}

TEST(TriDimArrayTestSuite, ArrayPlusTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);
    BigArray test_arr2 = BigArray::make_array(5, 5, 5);
    BigArray res;

    test_arr[0][0][0] = 15000;
    test_arr2[0][0][0] = 15000;

    res = test_arr + test_arr2;

    ASSERT_EQ(static_cast<int>(res[0][0][0]), 30000);
}

TEST(TriDimArrayTestSuite, ManyPlusTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);
    BigArray test_arr2 = BigArray::make_array(5, 5, 5);
    BigArray res;

    test_arr[0][0][0] = 15000;
    test_arr2[0][0][0] = 15000;

    test_arr[0][1][3] = 40000;

    test_arr[0][0][1] = 0;
    test_arr2[0][0][1] = 25000;

    test_arr[0][2][0] = 100000;
    test_arr2[0][2][0] = 12;

    res = test_arr + test_arr2;

    ASSERT_EQ(static_cast<int>(res[0][0][0]), 30000);
    ASSERT_EQ(static_cast<int>(res[0][1][3]), 40000);
    ASSERT_EQ(static_cast<int>(res[0][0][1]), 25000);
    ASSERT_EQ(static_cast<int>(res[0][2][0]), 100012);
}

TEST(TriDimArrayTestSuite, ArrayMinusTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);
    BigArray test_arr2 = BigArray::make_array(5, 5, 5);
    BigArray res;

    test_arr[0][0][0] = 125500;
    test_arr2[0][0][0] = 15000;

    res = test_arr - test_arr2;

    ASSERT_EQ(static_cast<int>(res[0][0][0]), 110500);
}

TEST(TriDimArrayTestSuite, ManyMinusTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);
    BigArray test_arr2 = BigArray::make_array(5, 5, 5);
    BigArray res;

    test_arr[0][0][0] = 15000;
    test_arr2[0][0][0] = 15000;

    test_arr[0][1][3] = 40000;

    test_arr[0][0][1] = 25000;
    test_arr2[0][0][1] = 0;

    test_arr[0][2][0] = 100000;
    test_arr2[0][2][0] = 12;

    res = test_arr - test_arr2;

    ASSERT_EQ(static_cast<int>(res[0][0][0]), 0);
    ASSERT_EQ(static_cast<int>(res[0][1][3]), 40000);
    ASSERT_EQ(static_cast<int>(res[0][0][1]), 25000);
    ASSERT_EQ(static_cast<int>(res[0][2][0]), 99988);
}

TEST(TriDimArrayTestSuite, MultiChislTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);
    int value = 10;

    test_arr[0][0][0] = 100;
    test_arr[0][0][1] = 200;
    test_arr[1][1][1] = 300;
    test_arr[2][2][2] = 400;

    test_arr = test_arr * value;

    ASSERT_EQ(static_cast<int>(test_arr[0][0][0]), 1000);
    ASSERT_EQ(static_cast<int>(test_arr[0][0][1]), 2000);
    ASSERT_EQ(static_cast<int>(test_arr[1][1][1]), 3000);
    ASSERT_EQ(static_cast<int>(test_arr[2][2][2]), 4000);
}

TEST(TriDimArrayTestSuite, ManyMultiChislTest) {
    BigArray test_arr = BigArray::make_array(5, 5, 5);
    int value = 100;

    test_arr[0][0][0] = 10;
    test_arr[0][0][1] = 20;
    test_arr[0][0][2] = 30;
    test_arr[0][0][3] = 40;
    test_arr[0][0][4] = 50;
    test_arr[0][1][0] = 60;
    test_arr[0][2][0] = 70;
    test_arr[0][3][0] = 80;
    test_arr[0][4][0] = 90;

    test_arr = test_arr * value;

    ASSERT_EQ(static_cast<int>(test_arr[0][0][0]), 1000);
    ASSERT_EQ(static_cast<int>(test_arr[0][0][1]), 2000);
    ASSERT_EQ(static_cast<int>(test_arr[0][0][2]), 3000);
    ASSERT_EQ(static_cast<int>(test_arr[0][0][3]), 4000);
    ASSERT_EQ(static_cast<int>(test_arr[0][0][4]), 5000);
    ASSERT_EQ(static_cast<int>(test_arr[0][1][0]), 6000);
    ASSERT_EQ(static_cast<int>(test_arr[0][2][0]), 7000);
    ASSERT_EQ(static_cast<int>(test_arr[0][3][0]), 8000);
    ASSERT_EQ(static_cast<int>(test_arr[0][4][0]), 9000);
}

TEST(TriDimArrayTestSuite, BigArrayTest) {
    ASSERT_NO_THROW(BigArray test_arr3 = BigArray::make_array(800, 800, 800));
}
