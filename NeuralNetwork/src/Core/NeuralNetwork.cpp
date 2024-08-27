#pragma once
#include "NeuralNetwork.h"



namespace NNCore {
	ActivationFunction NeuralNetwork::s_ActivationFunction; // Default or initial value

	NeuralNetwork::NeuralNetwork(const std::vector<int>& topology)
		: m_Topology(topology)
	{
		for (const auto& layerSize : m_Topology) {
			m_Layers.emplace_back(std::make_unique<Layer>(layerSize, s_ActivationFunction));
		}

	}

	void NeuralNetwork::Initialize(std::vector<double> startingValues, bool isWeightsRandomized)
	{
		std::vector<std::unique_ptr<Neuron>> startingNeurons;
		for (int i = 0; i < startingValues.size(); i++)
		{
			startingNeurons.push_back(std::make_unique<Neuron>(startingValues[i], s_ActivationFunction));
		}
		for (size_t i = 0; i < m_Layers.size() - 1; ++i) {
			m_Weights.emplace_back(std::make_unique<Utils::Matrix>(m_Layers[i]->GetSize(), m_Layers[i + 1]->GetSize(), isWeightsRandomized));
		}
		m_Layers[0]->SetLayer(std::move(startingNeurons));
		m_FirstRun = true;
	}

	void NeuralNetwork::Run()
	{
		//! Loop all layers
		for (int layerIndex = 0; layerIndex < m_Layers.size() - 1; layerIndex++)
		{
			size_t numCols = m_Weights[layerIndex]->GetNumCols();
			size_t numRows = m_Weights[layerIndex]->GetNumRows();
			std::vector<std::vector<double>> matrix = m_Weights[layerIndex]->GetMatrixValues();
			const std::vector<std::unique_ptr<Neuron>>& neuronValues = m_Layers[layerIndex]->GetNeurons();
			double addedValues = 0.00;

			//! Loop column
			for (int column = 0; column < numCols; column++)
			{
				for (int row = 0; row < numRows; row++)
				{
					addedValues += matrix[row][column] * neuronValues[row]->GetActivatedValue();
				}
				m_Layers[layerIndex + 1]->SetNeuron(column, std::make_unique<Neuron>(addedValues, s_ActivationFunction));
			}
		}
	}
}