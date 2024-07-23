#include <gtest/gtest.h>
#include "lib/sheduller.h"
#include <bits/stdc++.h>

TEST(SchedulerTest, FirstTestWithAll) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id1), 0);
    ASSERT_EQ(scheduler.getResult<float>(id2), 4);
    ASSERT_EQ(scheduler.getResult<float>(id3), 4);
    ASSERT_EQ(scheduler.getResult<float>(id4), 0);
    ASSERT_EQ(scheduler.getResult<float>(id5), 2);
    ASSERT_EQ(scheduler.getResult<float>(id6), 0);
}

TEST(SchedulerTest, FirstTestWithOutAll) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

    ASSERT_EQ(scheduler.getResult<float>(id1), 0);
    ASSERT_EQ(scheduler.getResult<float>(id2), 4);
    ASSERT_EQ(scheduler.getResult<float>(id3), 4);
    ASSERT_EQ(scheduler.getResult<float>(id4), 0);
    ASSERT_EQ(scheduler.getResult<float>(id5), 2);
    ASSERT_EQ(scheduler.getResult<float>(id6), 0);
}

TEST(SchedulerTest, NoArgumentsFunctionTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](){return 5;});

    ASSERT_EQ(scheduler.getResult<int>(id), 5);
}

TEST(SchedulerTest, SingleArgumentFunctionTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](int x){return x * x;}, 3);

    ASSERT_EQ(scheduler.getResult<int>(id), 9);
}
   
TEST(SchedulerTest, DependencyTest) {
    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](){return 2;});
    auto id2 = scheduler.add([](int x){return 2 * x;}, scheduler.getFutureResult<int>(id1));

    ASSERT_EQ(scheduler.getResult<int>(id2), 4);
}

TEST(SchedulerTest, DependencyTest2) {
    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](int x){ return x + 1; }, 5);
    auto id2 = scheduler.add([](int y){ return y * 2; }, scheduler.getFutureResult<int>(id1));

    ASSERT_EQ(scheduler.getResult<int>(id2), 12);
}
   
TEST(SchedulerTest, ErrorHandlingTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](int x){if (x == 0) throw std::runtime_error("Divide by zero"); return 10 / x;}, 0);

    ASSERT_THROW(scheduler.getResult<int>(id), std::runtime_error);
}

TEST(SchedulerTest, TwoArgumentFunctionTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](int a, int b){return a + b;}, 5, 3);

    ASSERT_EQ(scheduler.getResult<int>(id), 8);
}

TEST(SchedulerTest, ErrorHandlingTest2) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](int x){return 10 / x;}, 2);

    ASSERT_THROW(scheduler.getResult<int>(id + 1), std::runtime_error);
}

TEST(SchedulerTest, SimpleFunctionComplexDependencyTest) {
    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](int x, int y) { return x + y; }, 2, 3);
    auto id2 = scheduler.add([](int z) { return z * 10; }, scheduler.getFutureResult<int>(id1));
    auto id3 = scheduler.add([](int w) { return w - 40; }, scheduler.getFutureResult<int>(id2));
    auto id4 = scheduler.add([](int v, int u) { return v + u; }, scheduler.getFutureResult<int>(id3), scheduler.getFutureResult<int>(id1));

    ASSERT_EQ(scheduler.getResult<int>(id1), 5); // 2 + 3 = 5
    ASSERT_EQ(scheduler.getResult<int>(id2), 50); // 5 * 10 = 50
    ASSERT_EQ(scheduler.getResult<int>(id3), 10); // 50 - 40 = 10
    ASSERT_EQ(scheduler.getResult<int>(id4), 15); // 10 + 5 = 15
}

