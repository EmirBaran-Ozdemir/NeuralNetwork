
#pragma once
#include "nnpch.h"
#include "Layer/Layer.h"
#include "Core/NeuronActivation/NeuronActivation.h"

namespace NNCore {


	class NN_API NeuralNetwork {

	public:
		NeuralNetwork(const std::vector<int>& topology, NeuronActivation::ActivationFunction activationFunction, std::vector<double> inputValues, std::vector<double> outputValues);
		NeuralNetwork(const NeuralNetwork&) = delete;
		NeuralNetwork& operator=(const NeuralNetwork&) = delete;
		~NeuralNetwork() = default;

		void Train(int epochs);

		const std::vector<std::unique_ptr<Layer>>& GetLayers() const { return m_Layers; }
		const std::vector<std::unique_ptr<Utils::Matrix>>& GetWeights() const { return m_Weights; }

	private:
		void ForwardPropagation();
		double CalculateCost();
		void BackwardPropagation();
	private:
		std::vector<int> m_Topology;
		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::vector<std::unique_ptr<Utils::Matrix>> m_Weights;
		std::vector<double> m_TargetOutputValues;
		bool m_FirstEpoch = true;
		NeuronActivation::ActivationFunction m_ActivationFunction;
	};
}