#pragma once
#include "nnpch.h"
#include "Core/NeuronActivation/NeuronActivation.h"


namespace NNCore {

	class Neuron
	{
	public:
		Neuron(double inputValue, NeuronActivation::ActivationFunction activationFunction);

		~Neuron() = default;


		double GetBaseValue() const { return m_InputValue; }
		double GetActivatedValue() const { return m_ActivatedValue; }
		double GetDerivatedValue() const { return m_DerivatedValue; }

	private:
		void Activate();
		void Derive();
	
	private:
		double m_InputValue; // This value will be set by the user
		double m_ActivatedValue; // Will be calculated after 
		double m_DerivatedValue; // Derivative of the activation value
		NeuronActivation::ActivationFunction m_ActivationFunction;
	};
}
