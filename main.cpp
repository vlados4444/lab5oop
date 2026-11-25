/**
 * @file main.cpp
 * @brief Тесты для SharedPtr и шаблонных функций.
 */

#include "SharedPtr.hpp"
#include <iostream>
#include <utility>

int main() {
    // 1. Базовые тесты
    SharedPtr<int> p = make_shared<int>(42);
    SharedPtr<int> q = p;
    std::cout << "p.use_count = " << p.use_count() << "\n";

    SharedPtr<int> r = std::move(q);
    std::cout << "*r = " << *r << "\n";

    SharedPtr<int> a = make_shared<int>(1);
    SharedPtr<int> b = a;
    a.reset(new int(2));
    std::cout << "a = " << *a << ", b = " << *b << "\n";

    SharedPtr<int[]> arr = make_shared<int[]>(5);
    for (int i = 0; i < 5; ++i) arr[i] = i * 10;
    std::cout << "arr[2] = " << arr[2] << "\n";

    // 2. Шаблонные функции
    SharedPtr<int> x = make_shared<int>(5);
    SharedPtr<int> y = make_shared<int>(5);
    std::cout << "valueEqual(x,y) = " << valueEqual(x, y) << "\n";
    zeroFill(x);
    std::cout << "x после zeroFill = " << *x << "\n";

    // 3. Специализация для double
    SharedPtr<double> d1 = make_shared<double>(3.14);
    zeroFill(d1);
    std::cout << "d1 после zeroFill<double> = " << *d1 << "\n";  // -1

    SharedPtr<double> d2 = make_shared<double>(-1.0);
    std::cout << "valueEqual<double> = " << valueEqual(d1, d2) << "\n";  // 1
    
    return 0;
}