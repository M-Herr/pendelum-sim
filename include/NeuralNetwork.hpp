#pragma once

#include <vector>
#include <cstdlib>
#include <eigen3/Eigen/Eigen>
#include <memory>

struct NeuralNetworkLayout
{
    size_t input_size;
    size_t num_hidden;
    std::vector<size_t> hidden_sizes;
    size_t output_size;
};

class NeuralNetwork 
{
    public:
    NeuralNetwork() = default;
    ~NeuralNetwork() = default;

    explicit NeuralNetwork(const std::vector<size_t>& layer_sizes);

    NeuralNetwork(NeuralNetwork&& other) noexcept;
    NeuralNetwork(const NeuralNetwork& other);

    NeuralNetwork& operator=(const NeuralNetwork& other);
    NeuralNetwork& operator=(NeuralNetwork&& other) noexcept;

    Eigen::VectorXd Forward(const Eigen::VectorXd& input, std::vector<Eigen::VectorXd>& activations);

    void UpdateWeights(const std::vector<Eigen::MatrixXd>& weight_grads,
                       const std::vector<Eigen::VectorXd>& bias_grads,
                       double alpha);

    const std::vector<Eigen::MatrixXd>& GetWeights() const;
    const std::vector<Eigen::VectorXd>& GetBiases() const;


    private:
    std::vector<Eigen::MatrixXd> weights;
    std::vector<Eigen::VectorXd> biases;
    std::vector<size_t> layer_sizes;

    private:
    void InitializeWeights();

};

inline const std::vector<Eigen::MatrixXd>& NeuralNetwork::GetWeights() const
{
    return this->weights;
}

inline const std::vector<Eigen::VectorXd>& NeuralNetwork::GetBiases() const
{
    return this->biases;
}