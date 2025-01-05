#include "NeuralNetwork.hpp"

#include <random>

NeuralNetwork::NeuralNetwork(const std::vector<size_t>& layer_sizes)
: layer_sizes(layer_sizes)
{
  InitializeWeights(); 
}

NeuralNetwork::NeuralNetwork(NeuralNetwork&& other) noexcept
: 
weights(std::move(other.weights)),
biases(std::move(other.biases)),
layer_sizes(std::move(other.layer_sizes)) 
{

}

NeuralNetwork& NeuralNetwork::operator=(NeuralNetwork&& other) noexcept
 {
    if (this != &other) {
        weights = std::move(other.weights);
        biases = std::move(other.biases);
        layer_sizes = std::move(other.layer_sizes);
    }
    return *this;
}


NeuralNetwork& NeuralNetwork::operator=(const NeuralNetwork& other) {
    if (this != &other) {
        layer_sizes = other.layer_sizes;

        // Deep copy of weights and biases
        weights.clear();
        biases.clear();
        for (const auto& weight : other.weights) {
            weights.push_back(weight);
        }
        for (const auto& bias : other.biases) {
            biases.push_back(bias);
        }
    }
    return *this;
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork& other)
    : layer_sizes(other.layer_sizes) {
    // Deep copy of weights and biases
    for (const auto& weight : other.weights) {
        weights.push_back(weight);
    }
    for (const auto& bias : other.biases) {
        biases.push_back(bias);
    }
}


Eigen::VectorXd NeuralNetwork::Forward(const Eigen::VectorXd& input, std::vector<Eigen::VectorXd>& activations)
{
    Eigen::VectorXd activation = input;
    activations.clear();
    activations.push_back(activation);

    for (size_t i = 0; i < weights.size(); ++i) {
        activation = (weights[i] * activation + biases[i]).array().tanh();
        activations.push_back(activation);
    }
    
    return activation;
}

void NeuralNetwork::UpdateWeights(const std::vector<Eigen::MatrixXd>& weight_grads,
                       const std::vector<Eigen::VectorXd>& bias_grads,
                       double alpha)
{
    for (size_t i = 0; i < weights.size(); ++i) {
            weights[i] -= alpha * weight_grads[i];
            biases[i] -= alpha * bias_grads[i];
    }
}


void NeuralNetwork::InitializeWeights()
{
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (size_t i = 0; i < layer_sizes.size() - 1; ++i) {
        int input_size = layer_sizes[i];
        int output_size = layer_sizes[i + 1];

        weights.emplace_back(Eigen::MatrixXd::NullaryExpr(output_size, input_size, [&]() { return dist(rng); }));
        biases.emplace_back(Eigen::VectorXd::NullaryExpr(output_size, [&]() { return dist(rng); }));
    }       
}
