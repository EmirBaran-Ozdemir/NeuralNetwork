#pragma 
#include "nnvpch.h"
#include "Core/NeuralNetwork.h"
#include "Core/NeuronActivation/NeuronActivation.h"
#include "NNVisualizer/Visualizer.h"
#include <windows.h>

// Entry point for a Windows application
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	try {

		Utils::Random::Init();
		NNCore::NeuronActivation* neuronActivation = new NNCore::NeuronActivation();
		neuronActivation->SetActivationFunction(NNCore::NeuronActivation::ActivationFunction::FastSigmoid);
		NNCore::NeuronActivation::ActivationFunction activationFunction = neuronActivation->GetActivationFunction();

		std::vector<double> inputValues = { 0.5,0.2,0.4,0.1 };
		std::vector<double> outputValues = { 0.00,0.00,0.00,0.00,1.00 };
		std::vector<int> topology = { static_cast<int>(inputValues.size()), 3, 32, static_cast<int>(outputValues.size()) };
		auto myNN = std::make_unique<NNCore::NeuralNetwork>(topology, activationFunction);
		myNN->Train(inputValues, outputValues, 1); //todo Change this to step 
		std::cout << static_cast<std::string>(*myNN->GetWeights()[0].get()) << std::endl;

		for(auto& layer : myNN->GetLayers())
		{
			auto& neurons = layer.get()->GetNeurons();
			for(auto& neuron : neurons)
			{
				std::cout << neuron->GetActivatedValue() << std::endl;
			}
			std::cout << std::endl;
		}


	if(SUCCEEDED(CoInitialize(NULL)))
	{
		{
			NNVisualizer::Visualizer app;

			if(SUCCEEDED(app.Initialize()))
			{
				app.SetNN(std::move(myNN));

				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}
	}

	catch(std::invalid_argument& err)
	{
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
