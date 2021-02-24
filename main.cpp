#include <iostream>
#include "Vector.h"

int main() {

    try {
        Vector<int> intv(200000000, 1729);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Exception";
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
