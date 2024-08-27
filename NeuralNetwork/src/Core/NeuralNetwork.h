#pragma once
#include "nnpch.h"
#include "Layer/Layer.h"
#include "Utils/Matrix.h"
#include "ActivationFunction/ActivationFunction.h"


namespace NNCore {



	class NeuralNetwork {

	public:
		NeuralNetwork(const std::vector<int>& topology);
		void Initialize(std::vector<double> startingValues, bool isWeightsRandomized);
		void Run();
		const std::vector<std::unique_ptr<Layer>>& GetLayers() const { return m_Layers; }
		const std::vector<std::unique_ptr<Utils::Matrix>>& GetMatrices() const { return m_Weights; }
		
		static void SetActivationFunction(ActivationFunction activationFunction) { s_ActivationFunction = activationFunction; }
		static ActivationFunction GetActivationFunction() { return s_ActivationFunction; }
		
	private:
		std::vector<int> m_Topology;
		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::vector<std::unique_ptr<Utils::Matrix>> m_Weights;
		bool m_FirstRun = true;
		static ActivationFunction s_ActivationFunction;

	};
}