#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QString>

class Calculator
{
public:
    // Computes f(x) with parameters a, b, c
    static double calculate(double x, double a, double b, double c);

    // Generates graph data string: "graph||x1;y1||x2;y2||..."
    static QString generateGraphData(double xMin, double xMax, double step,
                                     double a, double b, double c);
};

#endif // CALCULATOR_H
