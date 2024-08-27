#include "nnpch.h"

#include "Core/Neuron/Neuron.h"
#include <Core/NeuralNetwork.h>
#include <Core/Utils/Matrix.h>

int main(int argc, char* argv[])
{
	Utils::Random::Init();
	NNCore::NeuralNetwork::SetActivationFunction(NNCore::ActivationFunction::FastSigmoid);
	NNCore::ActivationFunction activationFunction = NNCore::NeuralNetwork::GetActivationFunction();

	std::vector<int> topology = { 4,3,5,8 };
	NNCore::NeuralNetwork* myNN = new NNCore::NeuralNetwork(topology);
	myNN->Initialize({ 0.5,0.2,0.4,0.1 },true);
	myNN->Run();
	std::cout << static_cast<std::string>(*myNN->GetMatrices()[0].get()) << std::endl;
	auto& neuronsInit = myNN->GetLayers()[0].get()->GetNeurons();
	for (auto& neuron : neuronsInit)
	{
		std::cout << neuron->GetActivatedValue() << std::endl;
	}
	auto& neurons = myNN->GetLayers()[1].get()->GetNeurons();
	for (auto& neuron : neurons)
	{
		std::cout << neuron->GetActivatedValue() << std::endl;
	}
	return 0;
}
