
#pragma once
#include "nnpch.h"
#include "Layer/Layer.h"
#include "Core/NeuronActivation/NeuronActivation.h"

namespace NNCore {


	class NN_API NeuralNetwork {

	public:
		NeuralNetwork(const std::vector<int>& topology, const std::vector<double>& startingInputValues, const std::vector<double>& targetOutputValues, int maxEpoch, NeuronActivation::ActivationFunction activationFunction);
		NeuralNetwork(const NeuralNetwork&) = delete;
		NeuralNetwork& operator=(const NeuralNetwork&) = delete;
		~NeuralNetwork() = default;

		void Train();
		void Step();

		void ChangeLoopState(Utils::LoopState loopState);
		const std::vector<std::unique_ptr<Layer>>& GetLayers() const { return m_Layers; }
		const std::vector<std::unique_ptr<Utils::Matrix>>& GetWeights() const { return m_Weights; }
		const int GetCurrentEpochIndex() const { return m_CurrentEpochIndex; }
		const Utils::LoopState GetLoopState() const { return m_LoopState; }
	private:
		void ForwardPropagation();
		double CalculateCost();
		void BackwardPropagation();
	private:
		std::vector<int> m_Topology;
		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::vector<double> m_TargetOutputValues;
		NeuronActivation::ActivationFunction m_ActivationFunction;
		int m_MaxEpoch;

		Utils::LoopState m_LoopState{ Utils::LoopState::Stopped };
		int m_CurrentEpochIndex{ 0 };
		std::vector<std::unique_ptr<Utils::Matrix>> m_Weights;
		std::map<int, double> m_EpochCostMap;

		std::mutex nnMutex;

	};
}