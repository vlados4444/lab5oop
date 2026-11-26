/**
 * @file main.cpp
 * @brief Тесты для SharedPtr, шаблонных функций и чтение данных из файла.
 */

#include "SharedPtr.hpp"
#include <iostream>
#include <utility>
#include <fstream>
#include <string>
#include <vector>

/**
 * @brief Простая запись о студенте: имя и оценка.
 */
struct Student {
    std::string name;  ///< Имя студента.
    int grade;         ///< Оценка.

    /**
     * @brief Сравнение студентов по имени и оценке.
     * @param o Другой студент.
     * @return true, если имя и оценка совпадают.
     */
    bool operator==(const Student& o) const {
        return name == o.name && grade == o.grade;
    }
};

/**
 * @brief Выводит студента в формате "Имя (оценка)".
 * @param os Поток вывода.
 * @param s Студент.
 * @return os.
 */
std::ostream& operator<<(std::ostream& os, const Student& s) {
    return os << s.name << " (" << s.grade << ")";
}

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
    std::cout << "d1 после zeroFill<double> = " << *d1 << "\n";

    SharedPtr<double> d2 = make_shared<double>(-1.0);
    std::cout << "valueEqual<double> = " << valueEqual(d1, d2) << "\n";

    // 4. Тест дружественной функции operator<<
    std::cout << "\nТест operator<<:\n";
    std::cout << p << "\n";
    std::cout << arr << "\n";

    // 5. Чтение 10 студентов из файла
    std::ifstream fin("data.txt");
    if (!fin) {
        std::cerr << "Не найден data.txt\n";
        return 1;
    }

    std::vector<SharedPtr<Student>> students;
    std::string name;
    int grade;
    while (students.size() < 10 && fin >> name >> grade) {
        students.push_back(make_shared<Student>(Student{name, grade}));
    }

    std::cout << "\nСчитано студентов: " << students.size() << "\n";
    for (auto& s : students) {
        std::cout << *s << "\n";
    }
    
    std::cout << "Активных SharedPtr<Student>: " 
              << SharedPtr<Student>::activeCount() << "\n";
    
    return 0;
}