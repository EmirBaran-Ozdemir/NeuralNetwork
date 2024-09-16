#pragma once
#include "NeuralNetwork.h"


namespace NNCore {

	NeuralNetwork::NeuralNetwork(const std::vector<int>& topology, const std::vector<double>& startingInputValues, const std::vector<double>& targetOutputValues, int maxEpoch, NeuronActivation::ActivationFunction activationFunction)
		: m_Topology(topology), m_ActivationFunction(activationFunction), m_MaxEpoch(maxEpoch)
	{
		if(m_Topology.size() <= 0)
		{
			throw std::invalid_argument("Should give valid size of topology");
		}

		if(startingInputValues.size() != m_Topology[0] || targetOutputValues.size() != m_Topology[m_Topology.size() - 1]) {
			throw std::invalid_argument("Input or output size does not match the network topology.");
		}

		//! Initialize layers
		for(const auto& layerSize : m_Topology) {
			m_Layers.emplace_back(std::make_unique<Layer>(layerSize, m_ActivationFunction));
		}

		//! Initialize weights
		for(size_t i = 0; i < m_Layers.size() - 1; ++i) {
			m_Weights.emplace_back(std::make_unique<Utils::Matrix>(m_Layers[i]->GetSize(), m_Layers[i + 1]->GetSize(), true));
		}

		m_TargetOutputValues = targetOutputValues;
		m_Layers[0].get()->SetLayer(startingInputValues);
	}

	void NeuralNetwork::Train()
	{
		while(m_CurrentEpochIndex < m_MaxEpoch)
		{
			{
				std::lock_guard<std::mutex> lock(nnMutex);
				if(m_LoopState == Utils::LoopState::Stopped) {
					std::cout << "Training stopped" << std::endl;
					break;
				}

				// If Paused, wait until resumed
				while(m_LoopState == Utils::LoopState::Paused) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}

			this->ForwardPropagation();
			std::cout << "Cost " << this->CalculateCost() << std::endl;

			this->BackwardPropagation();
			m_CurrentEpochIndex++;
		}
	}

	void NeuralNetwork::Step()
	{
		this->ForwardPropagation();
		std::cout << "Cost " << this->CalculateCost() << std::endl;
		this->BackwardPropagation();

		m_CurrentEpochIndex++;
	}

	void NeuralNetwork::ChangeLoopState(Utils::LoopState loopState)
	{
		std::lock_guard<std::mutex> lock(nnMutex);
		m_LoopState = loopState;
	}

	void NeuralNetwork::ForwardPropagation()
	{
		std::lock_guard<std::mutex> lock(nnMutex);
		for(size_t layerIndex = 0; layerIndex < m_Layers.size() - 1; ++layerIndex)
		{
			float layerTime = 2.0f;
			float nodeTime = layerTime / m_Layers[layerIndex + 1]->GetNeurons().size();

			//TODO: Use weights->transpose then get col rows
			size_t numCols = m_Weights[layerIndex]->GetNumCols();
			size_t numRows = m_Weights[layerIndex]->GetNumRows();
			const std::vector<std::vector<double>>& matrix = m_Weights[layerIndex]->GetMatrixValues();
			const std::vector<std::unique_ptr<Neuron>>& neuronValues = m_Layers[layerIndex]->GetNeurons();

			auto lastTime = std::chrono::high_resolution_clock::now();

			for(size_t column = 0; column < numCols; ++column)
			{
				while(true)
				{
					auto currentTime = std::chrono::high_resolution_clock::now();
					std::chrono::duration<float> elapsed = currentTime - lastTime;
					if(elapsed.count() >= nodeTime)
					{
						double addedValues = 0.0;
						for(size_t row = 0; row < numRows; ++row)
						{
							addedValues += matrix[row][column] * neuronValues[row]->GetActivatedValue();
						}
						m_ProcessingNeuronColRow.first = layerIndex + 1;
						m_ProcessingNeuronColRow.second = column;
						m_Layers[layerIndex + 1]->SetNeuron(column, std::make_unique<Neuron>(addedValues, m_ActivationFunction));
						lastTime = currentTime;
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
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
			cost += pow(neurons[neuronIndex]->GetActivatedValue() - m_TargetOutputValues[neuronIndex], 2);
		}
		return cost;
	}

	void NeuralNetwork::BackwardPropagation()
	{
		//auto& outputLayer = m_Layers.back();
		//std::vector<double> outputDeltas(outputLayer->GetSize());
		//auto& neurons = outputLayer.get()->GetNeurons();

		//for(size_t neuronIndex = 0; neuronIndex < outputLayer.get()->GetSize(); neuronIndex++)
		//{
		//	//! Calculate deltas
		//	double outputValue = outputLayer.get()->GetNeurons()[neuronIndex]->GetActivatedValue();
		//	double targetValue = m_TargetOutputValues[neuronIndex];
		//	double delta = (outputValue - targetValue) * NeuronActivation::Derivate(outputValue, m_ActivationFunction);
		//	outputDeltas[neuronIndex] = delta;
		//}

		for(size_t layerIndex = 0; layerIndex < m_Layers.size() - 1; ++layerIndex)
		{
			size_t numCols = m_Weights[layerIndex]->GetNumCols();
			size_t numRows = m_Weights[layerIndex]->GetNumRows();
			float layerTime = 4.0f; // Total time for the layer
			float weightTime = layerTime / (numCols * numRows); // Time per weight
			for(size_t column = 0; column < numCols; ++column)
			{
				auto lastTime = std::chrono::high_resolution_clock::now();
				for(size_t row = 0; row < numRows; ++row)
				{
					while(true)
					{
						auto currentTime = std::chrono::high_resolution_clock::now();
						std::chrono::duration<float> elapsed = currentTime - lastTime;
						if(elapsed.count() >= weightTime)
						{

							m_Weights[layerIndex]->SetValue(row, column, Utils::Random::GetDoubleZeroToOne());
							lastTime = std::chrono::high_resolution_clock::now();
							lastTime = currentTime;
							break;
						}
					}
				}
			}
		}
	}
}