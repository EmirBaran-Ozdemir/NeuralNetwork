#include "nnvpch.h"
#include "Core/NeuralNetwork.h"
#include "Core/NeuronActivation/NeuronActivation.h"
#include "NNVisualizer/Visualizer.h"
#include <windows.h>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

// Utility function to parse comma-separated values from a string
std::vector<double> parseValues(const std::string& input) {
    std::vector<double> values;
    std::stringstream ss(input);
    std::string token;

    while(std::getline(ss, token, ',')) {
        values.push_back(std::stod(token));
    }

    return values;
}

// Utility function to parse integer values for topology
std::vector<int> parseTopology(const std::string& input) {
    std::vector<int> values;
    std::stringstream ss(input);
    std::string token;

    while(std::getline(ss, token, ',')) {
        values.push_back(std::stoi(token));
    }

    return values;
}

// Function to parse command-line arguments
void parseCommandLineArgs(int argc, char* argv[],
    std::vector<double>& inputValues,
    std::vector<double>& outputValues,
    std::vector<int>& topology,
    NNCore::NeuronActivation::ActivationFunction& activationFunction) {

    bool inputProvided = false;
    bool outputProvided = false;
    bool topologyProvided = false;
    bool activationFunctionProvided = false;

    for(int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg.find("-input:") == 0) {
            std::string inputStr = arg.substr(7);
            inputValues = parseValues(inputStr);
            inputProvided = true;
        }
        else if(arg.find("-output:") == 0) {
            std::string outputStr = arg.substr(8);
            outputValues = parseValues(outputStr);
            outputProvided = true;
        }
        else if(arg.find("-topology:") == 0) {
            std::string topologyStr = arg.substr(10);
            topology = parseTopology(topologyStr);
            topologyProvided = true;
        }
        else if(arg.find("-activationfunction:") == 0) {
            std::string activationStr = arg.substr(20);
            if(activationStr == "sigmoid") {
                activationFunction = NNCore::NeuronActivation::ActivationFunction::Sigmoid;
            }
            else if(activationStr == "relu") {
                activationFunction = NNCore::NeuronActivation::ActivationFunction::ReLU;
            }
            else {
                throw std::invalid_argument("Unknown activation function: " + activationStr);
            }
            activationFunctionProvided = true;
        }
    }

    if(!inputProvided || !outputProvided || !topologyProvided || !activationFunctionProvided) {
        throw std::invalid_argument("Missing required command-line arguments.");
    }
}

// Entry point for a Windows application
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    try {
        // Convert command line parameters to argc/argv
        int argc = 0;
        char* argv[256];
        char* cmdLine = GetCommandLineA();
        char* token = strtok(cmdLine, " ");
        while(token) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        std::vector<double> inputValues;
        std::vector<double> outputValues;
        std::vector<int> topology;
        NNCore::NeuronActivation::ActivationFunction activationFunction;

        // Parse command-line arguments
        parseCommandLineArgs(argc, argv, inputValues, outputValues, topology, activationFunction);

        auto myNN = std::make_unique<NNCore::NeuralNetwork>(topology, activationFunction);
        myNN->Train(inputValues, outputValues, 1); // Train the network

        std::cout << static_cast<std::string>(*myNN->GetWeights()[0].get()) << std::endl;

        for(auto& layer : myNN->GetLayers()) {
            auto& neurons = layer.get()->GetNeurons();
            for(auto& neuron : neurons) {
                std::cout << neuron->GetActivatedValue() << std::endl;
            }
            std::cout << std::endl;
        }

        // Visualizer setup
        if(SUCCEEDED(CoInitialize(NULL))) {
            {
                NNVisualizer::Visualizer app;

                if(SUCCEEDED(app.Initialize())) {
                    app.SetNN(std::move(myNN));
                    app.RunMessageLoop();
                }
            }
            CoUninitialize();
        }
    }
    catch(std::invalid_argument& err) {
        std::cerr << err.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
