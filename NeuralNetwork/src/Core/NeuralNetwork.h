
#pragma once
#include "nnpch.h"
#include "Layer/Layer.h"
#include "Core/NeuronActivation/NeuronActivation.h"

namespace NNCore {

	struct NeuralNetworkProperties {
		std::vector<int> topology;
		std::vector<double> startingInputValues;
		std::vector<double> targetOutputValues;
		int maxEpoch;
		NeuronActivation::ActivationFunction activationFunction;
	};
	class NN_API NeuralNetwork {
		
	public:
		NeuralNetwork(const NeuralNetworkProperties& properties);
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
		const std::pair<int, int> GetProcessingNeuronColRow() const { return m_ProcessingNeuronColRow; }
		const NeuralNetworkProperties& GetProperties() const {return m_Properties;}
	private:
		void ForwardPropagation();
		double CalculateCost();
		void BackwardPropagation();
	private:
		NeuralNetworkProperties m_Properties;
		std::vector<std::unique_ptr<Layer>> m_Layers;
		//std::vector<int> m_Topology;
		//std::vector<double> m_TargetOutputValues;
		//NeuronActivation::ActivationFunction m_ActivationFunction;
		//int m_MaxEpoch;

		Utils::LoopState m_LoopState{ Utils::LoopState::Stopped };
		int m_CurrentEpochIndex{ 0 };
		std::vector<std::unique_ptr<Utils::Matrix>> m_Weights;
		std::map<int, double> m_EpochCostMap;

		std::mutex nnMutex;
		std::pair<int, int> m_ProcessingNeuronColRow{ -1,-1 };
	};
}