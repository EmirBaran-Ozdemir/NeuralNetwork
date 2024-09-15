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
		auto lastTime = std::chrono::high_resolution_clock::now();

		for(size_t layerIndex = 0; layerIndex < m_Layers.size() - 1; ++layerIndex)
		{
			auto lastTime = std::chrono::high_resolution_clock::now();

			while(true) {
				auto currentTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> elapsed = currentTime - lastTime;

				if(elapsed.count() >= 2.0f)  // 4-second delay
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
					lastTime = currentTime;

					// Move to the next layer after processing
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Prevent busy-waiting
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
		auto lastTime = std::chrono::high_resolution_clock::now();

		for(size_t layerIndex = 0; layerIndex < m_Layers.size() - 1; ++layerIndex)
		{
			while(true) {
				auto currentTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> elapsed = currentTime - lastTime;
				if(elapsed.count() >= 2.0f)
				{
					size_t numCols = m_Weights[layerIndex]->GetNumCols();
					size_t numRows = m_Weights[layerIndex]->GetNumRows();
					Utils::Matrix* matrix = new Utils::Matrix(numRows, numCols, true); // Update matrix values here as necessary
					m_Weights[layerIndex]->SetMatrix(*matrix);
					lastTime = currentTime;
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}
}