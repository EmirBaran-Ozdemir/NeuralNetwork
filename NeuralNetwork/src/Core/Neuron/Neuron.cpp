#pragma once
#include "nnpch.h"
#include "Neuron.h"
#include "Core/NeuronActivation/NeuronActivation.h"
namespace NNCore {


	Neuron::Neuron(double inputValue, NeuronActivation::ActivationFunction activationFunction)
		: m_InputValue(inputValue), m_ActivatedValue(0.0), m_DerivatedValue(0.0), m_ActivationFunction(activationFunction)
	{
		Activate();
		Derive();
	}

	void Neuron::Activate()
	{
		switch (m_ActivationFunction)
		{
		case NeuronActivation::ActivationFunction::FastSigmoid:
			m_ActivatedValue = m_InputValue / (1 + abs(m_InputValue));
			break;
		case NeuronActivation::ActivationFunction::ReLU:
			m_ActivatedValue = m_InputValue > 0 ? m_InputValue : 0;
			break;
		default:
			m_ActivatedValue = 0;
			break;
		}
	}
	void Neuron::Derive()
	{
		switch (m_ActivationFunction)
		{
		case NeuronActivation::ActivationFunction::FastSigmoid:
			m_DerivatedValue = m_ActivatedValue * (1 - m_ActivatedValue);
			break;
		case NeuronActivation::ActivationFunction::ReLU:
			m_DerivatedValue = m_ActivatedValue > 0 ? 1 : 0;
			break;
		default:
			m_DerivatedValue = 0;
			break;
		}
	}
}
