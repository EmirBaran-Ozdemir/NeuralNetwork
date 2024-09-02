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
	private:
		static ActivationFunction s_ActivationFunction;

	};


}