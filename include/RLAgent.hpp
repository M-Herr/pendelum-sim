#pragma once
#include <vector>
#include <random>

#include <NeuralNetwork.hpp>

class RLAgent 
{
    public:
    RLAgent() = default;
    ~RLAgent() = default;

    RLAgent(const std::vector<size_t>& layer_sizes, double alpha, double gamma, double epsilon);
    

    Eigen::VectorXd selectAction(const Eigen::VectorXd& state);
    void Update(const Eigen::VectorXd& state, const Eigen::VectorXd& action, double reward, const Eigen::VectorXd& next_state);

    NeuralNetwork& GetNetwork() { return neural_network; }
    std::vector<Eigen::VectorXd> GetActivations() {return last_activation; }
    
    private:
    std::vector<Eigen::VectorXd> last_activation;
    NeuralNetwork neural_network;
    double alpha{0.0};
    double gamma{0.0};
    double epsilon{0.0};

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;

};