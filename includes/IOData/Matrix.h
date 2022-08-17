#pragma once
#include <vector>

class Matrix {
    int rows; // ����� �����
    int columns; // ����� ��������
    std::vector<std::vector<double>> values; // ��������

public:
    Matrix(int rows, int columns); // ����������� �� �������� ��������

    double& operator()(int i, int j); // ����������
    double operator()(int i, int j) const; // ����������
};