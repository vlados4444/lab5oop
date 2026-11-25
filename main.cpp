
#include "SharedPtr.hpp"
#include <iostream>

int main() {
    SharedPtr<int> p(new int(42));
    std::cout << "use_count = " << p.use_count() << "\n";
    std::cout << "activeCount = " << SharedPtr<int>::activeCount() << "\n";
    return 0;
}