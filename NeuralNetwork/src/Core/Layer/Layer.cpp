#include "nnpch.h"
#include "Layer.h"

namespace NNCore {
    Layer::Layer(size_t size, ActivationFunction activationFunction)
        : m_Size(size), m_Neurons() 
    {
        for (size_t i = 0; i < m_Size; i++)
        {
            m_Neurons.emplace_back(std::make_unique<Neuron>(0.00, activationFunction));
        }
    }

    void Layer::SetLayer(std::vector<std::unique_ptr<Neuron>>&& neurons) 
    {
        m_Neurons = std::move(neurons);
    }

    void Layer::SetNeuron(int index, std::unique_ptr<Neuron>&& neuron) 
    {
        if (index >= 0 && index < m_Neurons.size()) 
        {
            m_Neurons[index] = std::move(neuron);
        }
        else 
        {
            throw std::out_of_range("Index is out of range in SetNeuron.");
        }
    }


}
