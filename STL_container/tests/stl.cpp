#include "lib/bst.h"
#include <gtest/gtest.h>

TEST(BSTTestSuite, EmptyTest) {
    //ASSERT_NO_THROW(BST<int, pre_order_tag> work);
    BST<int, pre_order_tag> copy;
    EXPECT_TRUE(copy.empty());
    copy.insert(123);
    EXPECT_FALSE(copy.empty());
}

TEST(BSTTestSuite, CopyTest) {
    BST<int, pre_order_tag> work;
    work.insert(123);
    BST<int, pre_order_tag> copy(work);
    
    ASSERT_EQ(work.GetRoot()->value, copy.GetRoot()->value);
}

TEST(BSTTestSuite, CopyTest2) {
    BST<int, pre_order_tag> work;
    BST<int, pre_order_tag> copy;
    
    work.insert(123);
    copy = work;

    ASSERT_EQ(work.GetRoot()->value, copy.GetRoot()->value);
}

TEST(BSTTestSuite, CompareTest) {
    bool first = false, second = false;
    BST<int, pre_order_tag> work;
    work.insert(123);
    BST<int, pre_order_tag> copy(work);

    if (work == copy) {
        first = true;
    }

    work.insert(124);
    if (work != copy) {
        second = true;
    }

    ASSERT_EQ(first, second);
}

TEST(BSTTestSuite, CompareStrTest) {
    BST<std::string, in_order_tag> work;
    work.insert("123");
    BST<std::string, in_order_tag> copy(work);

    EXPECT_TRUE(work == copy);
    EXPECT_FALSE(work != copy);
}

TEST(BSTTestSuite, SwapEqualTest) {
    BST<int, pre_order_tag> work;
    BST<int, pre_order_tag> work2;
    work.insert(123);
    work2.insert(56);

    BST<int, pre_order_tag> copy(work);

    work.swap(work2);
    
    EXPECT_TRUE(work2 == copy);
    EXPECT_TRUE(work != copy);
}

TEST(BSTTestSuite, SizeTest) {
    BST<int, pre_order_tag> work;

    work.insert(12);
    work.insert(123);
    work.insert(125);

    int num = std::distance(work.begin(), work.end());

    ASSERT_EQ(num, work.size());
    ASSERT_EQ(3, work.size());
}

TEST(BSTTestSuite, TestBeginEnd) {
    BST<int, pre_order_tag> work;
    work.insert(2);
    work.insert(1);
    work.insert(3);

    Iterator<int, pre_order_tag> it(work.GetRoot(), work.size());

    ASSERT_EQ(*++it, *work.begin());
    ASSERT_EQ(*++++++it, *work.end()); // Сдвиг на 3, до конца
}

TEST(BSTTestSuite, ThreeBSTTest) {
    BST<int, pre_order_tag> work;

    int arr[9] = {8, 3, 1, 6, 4, 7, 10, 14, 13};
    int in[9] = {1, 3, 4, 6, 7, 8, 10, 13, 14};
    int pre[9] = {8, 3, 1, 6, 4, 7, 10, 14, 13};
    int post[9] = {1, 4, 7, 6, 3, 13, 14, 10, 8};
    int ans_in[9] = {};
    int ans_pre[9] = {};
    int ans_post[9] = {};

    for (int i = 0; i < 9; i++) {
        work.insert(arr[i]);
    }

    Iterator<int, in_order_tag> it(work.GetRoot(), work.size()); // 1 3 4 6 7 8 10 13 14 
    Iterator<int, pre_order_tag> it2(work.GetRoot(), work.size());
    Iterator<int, post_order_tag> it3(work.GetRoot(), work.size());

    for (int i = 0; i < 9; i++) {
        ans_in[i] = *it++;
    }
    for (int i = 0; i < 9; i++) {
        ans_pre[i] = *it2++;
    }
    for (int i = 0; i < 9; i++) {
        ans_post[i] = *it3++;
    }

    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ans_in[i], in[i]);
    }
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ans_pre[i], pre[i]);
    }
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ans_post[i], post[i]);
    }
}

TEST(BSTTestSuite, AlotOfFuncTest) {
    BST<int, pre_order_tag> work;
    BST<int, pre_order_tag> copy;

    work.insert(1);
    work.insert(2);
    copy.insert(3);
    copy.insert(4);

    work.merge(copy);
    ASSERT_TRUE(work.contains(4));

    work.extract(4);
    ASSERT_FALSE(work.contains(4));

    work.clear();
    ASSERT_EQ(work.size(), 0);
    ASSERT_TRUE(work.empty());

    copy.insert(3);
    copy.insert(3);
    copy.insert(3);
    ASSERT_EQ(copy.count(3), 4);
    
    copy.erase(3);
    ASSERT_EQ(copy.count(3), 0);
}