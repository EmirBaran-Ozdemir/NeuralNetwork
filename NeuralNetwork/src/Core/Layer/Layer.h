#include "nnpch.h"
#include "Core/Neuron/Neuron.h"


namespace NNCore {

	class Layer {
	public:
		Layer(size_t size, ActivationFunction activationFunction);
		size_t GetSize() const { return m_Size; }
		void SetLayer(std::vector<std::unique_ptr<Neuron>>&& neurons);
		void SetNeuron(int index, std::unique_ptr<Neuron>&& neuron);
		const std::vector<std::unique_ptr<Neuron>>& GetNeurons() const { return m_Neurons; }

	private:
		size_t m_Size;
		std::vector<std::unique_ptr<Neuron>> m_Neurons;
	};
}