#include "SharedPtr.hpp"
#include <iostream>
#include <utility>

int main() {
    SharedPtr<int> p(new int(42));
    SharedPtr<int> q = p;
    std::cout << "p.use_count = " << p.use_count() << "\n";

    SharedPtr<int> r = std::move(q);
    std::cout << "q пуст: " << static_cast<bool>(q) << "\n";
    std::cout << "r.use_count = " << r.use_count() << "\n";
    std::cout << "*r = " << *r << "\n";

    SharedPtr<int> a(new int(1));
    SharedPtr<int> b = a;
    a.reset(new int(2));
    std::cout << "a = " << *a << ", b = " << *b << "\n";
    std::cout << "a.use_count = " << a.use_count()
              << ", b.use_count = " << b.use_count() << "\n";
    return 0;
}