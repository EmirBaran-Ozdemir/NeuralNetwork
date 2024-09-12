#pragma once
#include "NeuralNetwork.h"


namespace NNCore {

	NeuralNetwork::NeuralNetwork(const std::vector<int>& topology, const std::vector<double>& startingInputValues, const std::vector<double>& targetOutputValues, NeuronActivation::ActivationFunction activationFunction)
		: m_Topology(topology), m_ActivationFunction(activationFunction)
	{
		if(m_Topology.size() <= 0)
		{
			throw std::invalid_argument("Should give valid size of topology");
		}

		//! Initialize layers
		for(const auto& layerSize : m_Topology) {
			m_Layers.emplace_back(std::make_unique<Layer>(layerSize, m_ActivationFunction));
		}

		//! Initialize weights
		for(size_t i = 0; i < m_Layers.size() - 1; ++i) {
			m_Weights.emplace_back(std::make_unique<Utils::Matrix>(m_Layers[i]->GetSize(), m_Layers[i + 1]->GetSize(), true));
		}

		if(startingInputValues.size() != m_Layers[0]->GetSize() || targetOutputValues.size() != m_Layers.back()->GetSize()) {
			throw std::invalid_argument("Input or output size does not match the network topology.");
		}
		m_TargetOutputValues = targetOutputValues;
		m_Layers[0].get()->SetLayer(startingInputValues);
	}

	void NeuralNetwork::Train(int epochs)
	{
		m_FirstEpoch = true;
		
		for(int epoch = 0; epoch < epochs; ++epoch)
		{
			// Add timer functionality
			double currentCost = 0.00;
			//! If this is not the first run then re-calculate weights
			if(!m_FirstEpoch)
			{

			}
			//! Loop all layers
			this->ForwardPropagation();
			std::cout << "Cost "<< this->CalculateCost() << std::endl;
			this->BackwardPropagation();
		}
	}

	void NeuralNetwork::ForwardPropagation()
	{
		for(size_t layerIndex = 0; layerIndex < m_Layers.size() - 1; ++layerIndex)
		{
			size_t numCols = m_Weights[layerIndex]->GetNumCols();
			size_t numRows = m_Weights[layerIndex]->GetNumRows();
			const std::vector<std::vector<double>>& matrix = m_Weights[layerIndex]->GetMatrixValues();
			const std::vector<std::unique_ptr<Neuron>>& neuronValues = m_Layers[layerIndex]->GetNeurons();

			for(size_t column = 0; column < numCols; ++column)
			{
				double addedValues = 0.0;
				for(size_t row = 0; row < numRows; ++row)
				{
					addedValues += matrix[row][column] * neuronValues[row]->GetActivatedValue();
				}
				m_Layers[layerIndex + 1]->SetNeuron(column, std::make_unique<Neuron>(addedValues, m_ActivationFunction));
			}
		}
	}


	double NeuralNetwork::CalculateCost()
	{
		std::unique_ptr<Layer>& outputLayer = m_Layers.back();
		if(outputLayer->GetSize() != m_TargetOutputValues.size())
		{
			THROW_ERROR_ARGS("Output layer should have same size as target output layer");
		}
		double cost = 0;
		auto& neurons = outputLayer.get()->GetNeurons();
		for(size_t neuronIndex = 0; neuronIndex < neurons.size(); neuronIndex++)
		{
			cost += pow(neurons[neuronIndex]->GetActivatedValue() - m_TargetOutputValues[neuronIndex], 2) ;
		}
		return cost;
	}

	void NeuralNetwork::BackwardPropagation()
	{
		auto& outputLayer = m_Layers.back();
		std::vector<double> outputDeltas(outputLayer->GetSize());
		auto& neurons = outputLayer.get()->GetNeurons();

		for(size_t neuronIndex = 0; neuronIndex < outputLayer.get()->GetSize(); neuronIndex++)
		{
			//! Calculate deltas
			double outputValue = outputLayer.get()->GetNeurons()[neuronIndex]->GetActivatedValue();
			double targetValue = m_TargetOutputValues[neuronIndex];
			double delta = (outputValue - targetValue) * NeuronActivation::Derivate(outputValue, m_ActivationFunction);
			outputDeltas[neuronIndex] = delta;
		}
	}
}