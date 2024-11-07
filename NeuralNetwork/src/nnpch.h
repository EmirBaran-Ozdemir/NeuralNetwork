#pragma once


#ifdef _WIN32
#ifdef BUILDING_NEURALNETWORK
#define NN_API __declspec(dllexport)
#else
#define NN_API __declspec(dllimport)
#endif
#else
#define NN_API
#endif


#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <ostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>
#include <atomic>
#include <mutex>
#include <condition_variable>

// Core
#include "Core/Utils/Utils.h"