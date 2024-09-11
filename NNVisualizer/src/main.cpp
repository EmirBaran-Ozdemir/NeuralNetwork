#include "nnvpch.h"
#include "Core/NeuralNetwork.h"
#include "Core/NeuronActivation/NeuronActivation.h"
#include "NNVisualizer/Visualizer.h"

void parseCommandLineArgs(int argc, char* argv[],
    std::vector<double>& inputValues,
    std::vector<double>& outputValues,
    std::vector<int>& topology,
    NNCore::NeuronActivation::ActivationFunction& activationFunction);

// Entry point for a Windows application
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    try {
        int argc = 0;
        std::vector<std::string> args;
        char* cmdLine = GetCommandLineA();
        std::string cmdLineStr = cmdLine;

        std::istringstream iss(cmdLineStr);
        std::string arg;
        while(iss >> std::quoted(arg)) {
            args.push_back(arg);
        }

        argc = args.size();
        char* argv[256];
        for(int i = 0; i < argc; ++i) {
            argv[i] = const_cast<char*>(args[i].c_str());
        }

        std::vector<double> inputValues;
        std::vector<double> outputValues;
        std::vector<int> topology;
        NNCore::NeuronActivation::ActivationFunction activationFunction;

        parseCommandLineArgs(argc, argv, inputValues, outputValues, topology, activationFunction);

        //! Main program loop 

        auto myNN = std::make_unique<NNCore::NeuralNetwork>(topology, activationFunction, inputValues, outputValues);
        myNN->Train(1);

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



std::vector<double> parseValues(const std::string& input) {
    std::vector<double> values;
    std::stringstream ss(input);
    std::string token;

    while(std::getline(ss, token, ',')) {
        // Trim whitespace from token
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        values.push_back(std::stod(token));
    }

    return values;
}

std::vector<int> parseTopology(const std::string& input) {
    std::vector<int> values;
    std::stringstream ss(input);
    std::string token;

    while(std::getline(ss, token, ',')) {
        // Trim whitespace from token
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        values.push_back(std::stoi(token));
    }

    return values;
}

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
            else if(activationStr == "fast_sigmoid") {
                activationFunction = NNCore::NeuronActivation::ActivationFunction::FastSigmoid;
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

