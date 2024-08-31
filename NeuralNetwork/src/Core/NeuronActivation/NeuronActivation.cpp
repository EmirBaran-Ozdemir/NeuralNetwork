#pragma once
#include "nnpch.h"
#include "NeuronActivation.h"

namespace NNCore {

	NeuronActivation::ActivationFunction NeuronActivation::s_ActivationFunction; // Default or initial value
	NeuronActivation::NeuronActivation(ActivationFunction activationFunction)
	{
		NeuronActivation::s_ActivationFunction = activationFunction;
	}
}