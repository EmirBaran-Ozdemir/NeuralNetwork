#include "nnvpch.h"
#include "Core/NeuralNetwork.h"
#include "Core/NeuronActivation/NeuronActivation.h"
#include "NNVisualizer/Visualizer.h"


int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	try {

		// Visualizer setup
		if (SUCCEEDED(CoInitialize(NULL))) {
			{
				NNVisualizer::Visualizer app;

				if (SUCCEEDED(app.Initialize())) {
					app.RunMessageLoop();
				}
			}
			CoUninitialize();
		}
	}
	catch (std::invalid_argument& err)
	{
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}