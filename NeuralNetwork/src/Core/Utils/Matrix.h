#pragma once
#include <vector>

namespace Utils {
	class Matrix {
	public:
		Matrix(int numRows, int numCols, bool isRandomized);
		Matrix(const Matrix&) = delete;
		Matrix& operator=(const Matrix&) = delete;

		void Transpose();

		void SetValue(int row, int column, double value) { m_Values[row][column] = value; }
		double GetValue(int row, int column) const { return m_Values.at(row).at(column); }
		void SetMatrix(const std::vector<std::vector<double>>& matrix);
		void SetMatrix(const Matrix& matrix);
		std::vector<std::vector<double>> GetMatrixValues() const { return m_Values; }
		size_t GetNumRows() const { return m_NumRows; }
		size_t GetNumCols() const { return m_NumCols; }

		operator std::string() const {
			std::string str;
			for (int i = 0; i < m_NumRows; i++)
			{
				for (int j = 0; j < m_NumCols; j++)
				{
					str += std::to_string(m_Values.at(i).at(j)) + " ";
				}
				str += "\n";
			}
			return str;
		}

	private:
		size_t m_NumRows, m_NumCols;
		std::vector<std::vector<double>> m_Values;
	};
}