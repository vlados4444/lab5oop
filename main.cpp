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

    // 2. Тесты шаблонных функций zeroFill и valueEqual
    SharedPtr<int> x = make_shared<int>(5);
    SharedPtr<int> y = make_shared<int>(5);
    
    // valueEqual проверяет совпадение значений
    std::cout << "valueEqual(x,y) = " << valueEqual(x, y) << "\n";
    
    // zeroFill сбрасывает значение в 0
    zeroFill(x);
    std::cout << "x после zeroFill = " << *x << "\n";
    
    return 0;
}