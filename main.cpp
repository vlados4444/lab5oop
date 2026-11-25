#include "SharedPtr.hpp"
#include <iostream>
#include <utility>

int main() {
    SharedPtr<int> p(new int(42));
    SharedPtr<int> q = p;
    std::cout << "p.use_count = " << p.use_count() << "\n";  // 2

    SharedPtr<int> r = std::move(q);
    std::cout << "q пуст: " << static_cast<bool>(q) << "\n"; // 0
    std::cout << "r.use_count = " << r.use_count() << "\n";  // 2
    std::cout << "*r = " << *r << "\n";                      // 42
    return 0;
}