#include <iostream>
#include <cmath>
#include <cassert>

// Копия функции calculate из calculator.cpp (для автономного тестирования)
double calculate(double x, double a, double b, double c)
{
    if (x < -2.0) {
        return std::fabs(x * a) - 2.0;
    } else if (x >= -2.0 && x < 2.0) {
        return b * (x * x) + x + 1.0;
    } else {
        return std::fabs(x - 2.0) + 1.0 * c;
    }
}

bool approxEqual(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

int main()
{
    std::cout << "=== Тестирование функции №9 (с параметрами a, b, c) ===" << std::endl;
    std::cout << "f(x) = |x*a|-2 при x<-2, b*(x^2)+x+1 при -2<=x<2, |x-2|+1*c при x>=2" << std::endl;
    std::cout << std::endl;

    // ═══ Тесты с a=1, b=1, c=1 (стандартные значения) ═══
    std::cout << "--- a=1, b=1, c=1 ---" << std::endl;

    // Левая ветвь: |x*1| - 2
    assert(approxEqual(calculate(-5.0, 1, 1, 1), 3.0));
    std::cout << "[OK] f(-5) = " << calculate(-5.0, 1, 1, 1) << " (ожидалось 3)" << std::endl;

    assert(approxEqual(calculate(-3.0, 1, 1, 1), 1.0));
    std::cout << "[OK] f(-3) = " << calculate(-3.0, 1, 1, 1) << " (ожидалось 1)" << std::endl;

    // Средняя ветвь: 1*(x^2) + x + 1
    assert(approxEqual(calculate(-2.0, 1, 1, 1), 3.0));  // 4 + (-2) + 1 = 3
    std::cout << "[OK] f(-2) = " << calculate(-2.0, 1, 1, 1) << " (ожидалось 3)" << std::endl;

    assert(approxEqual(calculate(0.0, 1, 1, 1), 1.0));  // 0 + 0 + 1 = 1
    std::cout << "[OK] f(0) = " << calculate(0.0, 1, 1, 1) << " (ожидалось 1)" << std::endl;

    assert(approxEqual(calculate(1.0, 1, 1, 1), 3.0));  // 1 + 1 + 1 = 3
    std::cout << "[OK] f(1) = " << calculate(1.0, 1, 1, 1) << " (ожидалось 3)" << std::endl;

    // Правая ветвь: |x-2| + 1*1
    assert(approxEqual(calculate(2.0, 1, 1, 1), 1.0));  // 0 + 1 = 1
    std::cout << "[OK] f(2) = " << calculate(2.0, 1, 1, 1) << " (ожидалось 1)" << std::endl;

    assert(approxEqual(calculate(5.0, 1, 1, 1), 4.0));  // 3 + 1 = 4
    std::cout << "[OK] f(5) = " << calculate(5.0, 1, 1, 1) << " (ожидалось 4)" << std::endl;

    // ═══ Тесты с кастомными параметрами ═══
    std::cout << std::endl << "--- a=2, b=0.5, c=3 ---" << std::endl;

    // Левая ветвь: |x*2| - 2
    // f(-5) = |-5*2| - 2 = 10 - 2 = 8
    assert(approxEqual(calculate(-5.0, 2, 0.5, 3), 8.0));
    std::cout << "[OK] f(-5, a=2) = " << calculate(-5.0, 2, 0.5, 3) << " (ожидалось 8)" << std::endl;

    // Средняя ветвь: 0.5*(x^2) + x + 1
    // f(0) = 0 + 0 + 1 = 1
    assert(approxEqual(calculate(0.0, 2, 0.5, 3), 1.0));
    std::cout << "[OK] f(0, b=0.5) = " << calculate(0.0, 2, 0.5, 3) << " (ожидалось 1)" << std::endl;

    // f(1) = 0.5*1 + 1 + 1 = 2.5
    assert(approxEqual(calculate(1.0, 2, 0.5, 3), 2.5));
    std::cout << "[OK] f(1, b=0.5) = " << calculate(1.0, 2, 0.5, 3) << " (ожидалось 2.5)" << std::endl;

    // Правая ветвь: |x-2| + 1*3
    // f(5) = |5-2| + 3 = 3 + 3 = 6
    assert(approxEqual(calculate(5.0, 2, 0.5, 3), 6.0));
    std::cout << "[OK] f(5, c=3) = " << calculate(5.0, 2, 0.5, 3) << " (ожидалось 6)" << std::endl;

    // f(2) = |2-2| + 3 = 0 + 3 = 3
    assert(approxEqual(calculate(2.0, 2, 0.5, 3), 3.0));
    std::cout << "[OK] f(2, c=3) = " << calculate(2.0, 2, 0.5, 3) << " (ожидалось 3)" << std::endl;

    // ═══ Тесты с отрицательными параметрами ═══
    std::cout << std::endl << "--- a=-1, b=-1, c=-2 ---" << std::endl;

    // Левая: |x*(-1)| - 2 = |x| - 2 (тот же результат что и a=1)
    assert(approxEqual(calculate(-5.0, -1, -1, -2), 3.0));
    std::cout << "[OK] f(-5, a=-1) = " << calculate(-5.0, -1, -1, -2) << " (ожидалось 3)" << std::endl;

    // Средняя: -1*(x^2) + x + 1
    // f(1) = -1 + 1 + 1 = 1
    assert(approxEqual(calculate(1.0, -1, -1, -2), 1.0));
    std::cout << "[OK] f(1, b=-1) = " << calculate(1.0, -1, -1, -2) << " (ожидалось 1)" << std::endl;

    // Правая: |x-2| + 1*(-2) = |x-2| - 2
    // f(5) = 3 + (-2) = 1
    assert(approxEqual(calculate(5.0, -1, -1, -2), 1.0));
    std::cout << "[OK] f(5, c=-2) = " << calculate(5.0, -1, -1, -2) << " (ожидалось 1)" << std::endl;

    std::cout << std::endl;
    std::cout << "=== Все тесты пройдены! ===" << std::endl;

    return 0;
}
