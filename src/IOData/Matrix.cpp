
#include "IOData/Matrix.h"


Matrix::Matrix(int rows, int columns) {
    this->rows = rows; // ��������� ����� �����
    this->columns = columns; // ��������� ����� ��������

    values = std::vector<std::vector<double>>(rows, std::vector<double>(columns, 0)); // ������ ������� ��� �������� �������
}
double& Matrix::operator()(int i, int j) {
    return values[i][j];
}

// ����������
double Matrix::operator()(int i, int j) const {
    return values[i][j];
}