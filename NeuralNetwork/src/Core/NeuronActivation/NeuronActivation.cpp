#pragma once
#include "NeuronActivation.h"

namespace NNCore {

	NN_API NeuronActivation::ActivationFunction NeuronActivation::s_ActivationFunction = NeuronActivation::ActivationFunction::Null; // Default or initial value

	void NeuronActivation::SetActivationFunction(NeuronActivation::ActivationFunction activationFunction)
	{
		s_ActivationFunction = activationFunction;

	}
	double NeuronActivation::Activate(double value, NeuronActivation::ActivationFunction activationFunction)
	{
		switch(activationFunction)
		{
		case NeuronActivation::ActivationFunction::Null:
			THROW_ERROR("Activation function is not defined");
			return -1;
		case NeuronActivation::ActivationFunction::FastSigmoid:
			return value / (1 + abs(value));
		case NeuronActivation::ActivationFunction::Sigmoid:
			return 1.0 / (1.0 + exp(-value));
		case NeuronActivation::ActivationFunction::ReLU:
			return value > 0 ? value : 0;
		default:
			THROW_ERROR("Activation function is not defined");
			return -1;
		}
	}

	double NeuronActivation::Derivate(double value, NeuronActivation::ActivationFunction activationFunction) {
		switch(activationFunction)
		{
		case NeuronActivation::ActivationFunction::Null:
			THROW_ERROR("Activation function is not defined");
			return -1;
		case NeuronActivation::ActivationFunction::Sigmoid:
			return value * (1 - value);
		case NeuronActivation::ActivationFunction::ReLU:
			return value > 0 ? 1.0 : 0.0;
		case NeuronActivation::ActivationFunction::FastSigmoid:
			return 1.0 / std::pow((1.0 + std::abs(value)), 2);
		default:
			THROW_ERROR("Activation function is not defined");
			return -1;
		}
	}
}