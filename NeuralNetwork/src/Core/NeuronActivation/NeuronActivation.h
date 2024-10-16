#pragma once
#include "nnpch.h"

namespace NNCore {

	class NN_API NeuronActivation {
	public:
		enum ActivationFunction {

			// Non linear
			Null = 0,
			Sigmoid,
			FastSigmoid , // f(x) = x / (1 + |x|) ==> Derivative : f'(x) = f(x) * (1 - f(x))
			ReLU			 // f(x) = max(0, x) ==> Derivateive: f'(x) = { 1 if x > 0 && 0 if x <= 0 } 

		};
	public:
		NeuronActivation() = default;
		void SetActivationFunction(ActivationFunction activationFunction);
		ActivationFunction GetActivationFunction() const { return s_ActivationFunction; }
		static double Activate(double value, ActivationFunction activationFunction);
		static double Derivate(double value, ActivationFunction activationFunction);
		static const std::vector<std::wstring> GetAllActivationFunctions()
		{
			std::vector<std::wstring> activationFunctions;

			activationFunctions.push_back(L"Sigmoid");
			activationFunctions.push_back(L"FastSigmoid");
			activationFunctions.push_back(L"ReLU");

			return activationFunctions;
		}

		static const std::string& GetActivationFunctionString(ActivationFunction activationFunction)
		{
			static const std::string sigmoid = "Sigmoid";
			static const std::string fastSigmoid = "FastSigmoid";
			static const std::string relu = "ReLU";

			switch (activationFunction)
			{
			case Sigmoid: return sigmoid;
			case FastSigmoid: return fastSigmoid;
			case ReLU: return relu;
			}
		}
	private:
		static ActivationFunction s_ActivationFunction;

	};


}