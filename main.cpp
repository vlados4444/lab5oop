#include "SharedPtr.hpp"
#include <iostream>
#include <utility>

int main() {
    SharedPtr<int> p(new int(42));
    SharedPtr<int> q = p;
    std::cout << "p.use_count = " << p.use_count() << "\n";

    SharedPtr<int> r = std::move(q);
    std::cout << "*r = " << *r << "\n";

    SharedPtr<int> a(new int(1));
    SharedPtr<int> b = a;
    a.reset(new int(2));
    std::cout << "a = " << *a << ", b = " << *b << "\n";

    SharedPtr<int[]> arr(new int[5]{1, 2, 3, 4, 5});
    std::cout << "arr[2] = " << arr[2] << "\n";
    return 0;
}