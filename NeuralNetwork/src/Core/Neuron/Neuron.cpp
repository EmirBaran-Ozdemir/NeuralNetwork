#pragma once
#include "nnpch.h"
#include "Neuron.h"
#include "Core/NeuronActivation/NeuronActivation.h"
namespace NNCore {

	Neuron::Neuron(double inputValue, NeuronActivation::ActivationFunction activationFunction)
		: m_InputValue(inputValue), m_ActivatedValue(0.0), m_DerivatedValue(0.0), m_ActivationFunction(activationFunction)
	{
		m_ActivatedValue = NeuronActivation::Activate(m_InputValue,m_ActivationFunction);
		m_DerivatedValue = NeuronActivation::Derivate(m_InputValue, m_ActivationFunction);
	}
}
