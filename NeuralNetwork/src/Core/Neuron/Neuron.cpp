#pragma once
#include "nnpch.h"
#include "Neuron.h"

namespace NNCore {


	Neuron::Neuron(double inputValue, ActivationFunction activationFunction)
		: m_InputValue(inputValue), m_ActivatedValue(0.0), m_DerivatedValue(0.0)
	{
		Activate(activationFunction);
		Derive(activationFunction);
	}

	void Neuron::Activate(ActivationFunction activationFunction)
	{
		switch (activationFunction)
		{
		case ActivationFunction::FastSigmoid:
			m_ActivatedValue = m_InputValue / (1 + abs(m_InputValue));
			break;
		case ActivationFunction::ReLU:
			m_ActivatedValue = m_InputValue > 0 ? m_InputValue : 0;
			break;
		default:
			m_ActivatedValue = 0;
			break;
		}
	}
	void Neuron::Derive(ActivationFunction activationFunction)
	{
		switch (activationFunction)
		{
		case ActivationFunction::FastSigmoid:
			m_DerivatedValue = m_ActivatedValue * (1 - m_ActivatedValue);
			break;
		case ActivationFunction::ReLU:
			m_DerivatedValue = m_ActivatedValue > 0 ? 1 : 0;
			break;
		default:
			m_DerivatedValue = 0;
			break;
		}
	}
}
