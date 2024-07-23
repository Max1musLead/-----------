#include "lib/sheduller.h"

int main() {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

    auto id4 = scheduler.add([](float b, std::vector<int> d){return -b - std::sqrt(d);}, b, );

    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    std::cout << "x1 = " << scheduler.getResult<float>(id1) << std::endl;
    std::cout << "x2 = " << scheduler.getResult<float>(id2) << std::endl;
    std::cout << "x3 = " << scheduler.getResult<float>(id3) << std::endl;
    std::cout << "x4 = " << scheduler.getResult<float>(id4) << std::endl;
    std::cout << "x5 = " << scheduler.getResult<float>(id5) << std::endl;
    std::cout << "x6 = " << scheduler.getResult<float>(id6) << std::endl;
}
