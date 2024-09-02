#pragma once
#include "Matrix.h"

namespace Utils {

	Matrix::Matrix(int numRows, int numCols, bool isRandomized) : m_NumRows(numRows), m_NumCols(numCols)
	{
		for (int i = 0; i < m_NumRows; i++)
		{
			std::vector<double> colValues;
			for (int j = 0; j < m_NumCols; j++)
			{
				double initValue = 0.00;
				if (isRandomized)
				{
					initValue = Utils::Random::GetDoubleZeroToOne();
				}
				colValues.push_back(initValue);
			}
			m_Values.push_back(colValues);
		}
	}

	void Matrix::Transpose()
	{
		std::vector<std::vector<double>> transposed(m_NumCols, std::vector<double>(m_NumRows));

		for (int i = 0; i < m_NumRows; ++i)
		{
			for (int j = 0; j < m_NumCols; ++j)
			{
				transposed[j][i] = m_Values[i][j];
			}
		}

		m_Values.swap(transposed);
		std::swap(m_NumRows, m_NumCols);
	}


	void Matrix::SetMatrix(const std::vector<std::vector<double>>& matrix)
	{
		m_Values = matrix;
		m_NumRows = matrix.size();
		m_NumCols = matrix.at(0).size();
	}

}