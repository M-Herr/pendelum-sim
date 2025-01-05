#include "RLAgent.hpp"


RLAgent::RLAgent(const std::vector<size_t>& layer_sizes, double alpha, double gamma, double epsilon)
{

    neural_network = std::move(NeuralNetwork(layer_sizes));
}

Eigen::VectorXd RLAgent::selectAction(const Eigen::VectorXd& state)
{
    std::vector<Eigen::VectorXd> activations;

    if (distribution(generator) < epsilon) {
        return Eigen::VectorXd::Random(neural_network.Forward(state, activations).size()); // Random action
    }

    Eigen::VectorXd actions = neural_network.Forward(state, activations);

    if(last_activation.size() != activations.size())
    {
        last_activation.resize(activations.size());
    }
    
    for(size_t i = 0; i < activations.size(); i++)
    {
        last_activation[i] = activations[i];
    }
    

    return actions;// Predict action
}

void RLAgent::Update(const Eigen::VectorXd& state, const Eigen::VectorXd& action, double reward, const Eigen::VectorXd& next_state)
{
    std::vector<Eigen::VectorXd> activations;
    Eigen::VectorXd predicted_action = neural_network.Forward(state, activations);

    std::vector<Eigen::VectorXd> next_activations;
    Eigen::VectorXd next_action = neural_network.Forward(next_state, next_activations);

    double target = reward + gamma * next_action.maxCoeff();
    Eigen::VectorXd output_error = predicted_action;
    output_error[0] = target - predicted_action[0];
    

    std::vector<Eigen::MatrixXd> weight_grads(neural_network.GetWeights().size());
    std::vector<Eigen::VectorXd> bias_grads(neural_network.GetBiases().size());


    Eigen::VectorXd delta = output_error;
    for (int i = neural_network.GetWeights().size() - 1; i >= 0; --i) {
        bias_grads[i] = delta;
        weight_grads[i] = delta * activations[i].transpose();

        if (i > 0) {
            delta = (neural_network.GetWeights()[i].transpose() * delta).array() * (1.0 - activations[i].array().square());
        }
    }

    neural_network.UpdateWeights(weight_grads, bias_grads, alpha);
}