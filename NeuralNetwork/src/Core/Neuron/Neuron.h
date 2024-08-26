#pragma once
#include "nnpch.h"
#include <Core/ActivationFunction/ActivationFunction.h>


namespace NNCore {



	class Neuron
	{
	public:
		Neuron(double inputValue, ActivationFunction activationFunction);

		~Neuron() = default;

		void Activate(ActivationFunction activationFunction);
		void Derive(ActivationFunction activationFunction);

		double GetInputValue() const { return m_InputValue; }
		double GetActivatedValue() const { return m_ActivatedValue; }
		double GetDerivatedValue() const { return m_DerivatedValue; }

	private:
		double m_InputValue; // This value will be set by the user
		double m_ActivatedValue; // Will be calculated after 
		double m_DerivatedValue; // Derivative of the activation value

	};
}
