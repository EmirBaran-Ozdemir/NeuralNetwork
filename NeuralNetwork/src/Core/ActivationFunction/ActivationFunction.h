#pragma once
#include "nnpch.h"

namespace NNCore {

	enum class ActivationFunction {

		// Non linear
		FastSigmoid = 0, // f(x) = x / (1 - |x|) ==> Derivative : f'(x) = f(x) / (1 - f(x))
		ReLU			 // f(x) = max(0, x) ==> Derivateive: f'(x) = { 1 if x > 0 && 0 if x <= 0 } 
	};

}