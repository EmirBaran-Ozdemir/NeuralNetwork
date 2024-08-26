#include "nnpch.h"

#include "Core/Neuron/Neuron.h"
#include <Core/NeuralNetwork.h>


int main(int argc, char* argv[])
{
	NNCore::NeuralNetwork::SetActivationFunction(NNCore::ActivationFunction::FastSigmoid);
	NNCore::ActivationFunction activationFunction = NNCore::NeuralNetwork::GetActivationFunction();

	std::vector<int>* topology = new std::vector<int>();
	topology->push_back(4);
	topology->push_back(3);
	topology->push_back(5);
	topology->push_back(8);
	NNCore::NeuralNetwork* myNN = new NNCore::NeuralNetwork(*topology);
	myNN->Initialize({ 0.5,0.2,0.4,0.1 },true);
	myNN->Run();
	std::cout << "Hello, World!" << std::endl;
	return 0;
}
