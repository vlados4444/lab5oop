/**
 * @file main.cpp
 * @brief Тесты для SharedPtr.
 */

#include "SharedPtr.hpp"
#include <iostream>
#include <utility>

int main() {
    // Создание одиночного объекта
    SharedPtr<int> p = make_shared<int>(42);
    SharedPtr<int> q = p;
    std::cout << "p.use_count = " << p.use_count() << "\n";

    SharedPtr<int> r = std::move(q);
    std::cout << "*r = " << *r << "\n";

    SharedPtr<int> a = make_shared<int>(1);
    SharedPtr<int> b = a;
    a.reset(new int(2));
    std::cout << "a = " << *a << ", b = " << *b << "\n";

    // Создание массива
    SharedPtr<int[]> arr = make_shared<int[]>(5);
    for (int i = 0; i < 5; ++i) arr[i] = i * 10;
    std::cout << "arr[2] = " << arr[2] << "\n";
    
    return 0;
}