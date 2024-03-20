#include "lib/3DArray.h"

int main(){
    BigArray test_arr = BigArray::make_array(5, 5, 5);
    BigArray test_arr2 = BigArray::make_array(5, 5, 5);
    BigArray res = BigArray::make_array(5, 5, 5);
    test_arr[0][0][0] = 10;
    test_arr2[0][0][0] = 5;

    res = test_arr - test_arr2;
    res = test_arr + test_arr2;

    std::cout << res[0][0][0] << " ";

}
