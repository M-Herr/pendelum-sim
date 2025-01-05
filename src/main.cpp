#include <iostream>
#include <string>
#include <bit>
#include <cmath>
#include <chrono>
#include <thread>
#include <csignal>
#include <tuple>

#include "type_traits.hpp"


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "InvertedPendelum.hpp"
#include "PIDController.hpp"
#include "NeuralNetwork.hpp"
#include "RLAgent.hpp"
#include "Simulation.hpp"
#include "FunctionJob.hpp"
#include "PIDTuningJob.hpp"
#include "RungeKutta4.hpp"

#include <SFML/Graphics.hpp>
#include <eigen3/Eigen/Dense>
#include <vector>

#include "VariableRegistry.hpp"
#include "SimVar.hpp"

double AngleError(double setpoint, double current)
{
    double raw_diff = setpoint - current;
    // Wrap the difference into [-180, 180)
    while (raw_diff <= -M_PI) raw_diff += 2.0*M_PI;
    while (raw_diff >  M_PI)  raw_diff -= 2.0*M_PI;
    return raw_diff;
}

void DisplayInfo(sf::RenderWindow& window, std::shared_ptr<InvertedPendelum> pendulum, std::shared_ptr<PIDController> pid) {
    

    sf::Font font;
    if (!font.openFromFile("/home/mherr/repos/BasicProjectCPP/resources/fonts/Arial.ttf"))
    {
        // error...
    }

    // Create text elements
    sf::Text angleText(font);
    angleText.setFont(font);
    angleText.setCharacterSize(20);
    angleText.setFillColor(sf::Color::Black);
    angleText.setPosition({10.f, 10.f});
    
    sf::Text torqueText(font);
    torqueText.setFont(font);
    torqueText.setCharacterSize(20);
    torqueText.setFillColor(sf::Color::Black);
    torqueText.setPosition({10.f, 40.f});


    sf::Text pidText(font);
    pidText.setFont(font);
    pidText.setCharacterSize(20);
    pidText.setFillColor(sf::Color::Black);
    pidText.setPosition({10.f, 80.f});

    // Format the information to display
    std::ostringstream angleStream;
    angleStream << "Theta: " << pendulum->GetAngle()*180.0/M_PI << " degree";
    angleText.setString(angleStream.str());
    
    // Format the information to display
    std::ostringstream torqueStream;
    torqueStream << "Torque: " << pendulum->GetTorque() << " N*m";
    torqueText.setString(torqueStream.str());

    std::ostringstream pidStream;
    pidStream << "Kp: " << pid->GetKp() << " Ki: " << pid->GetKi() << " Kd: " << pid->GetKd();
    pidText.setString(pidStream.str());

    // Draw the text elements
    window.draw(angleText);
    window.draw(torqueText);
    window.draw(pidText);
}


void VisualizeNeuralNetwork(
    sf::RenderWindow& window,
    const std::vector<Eigen::VectorXd>& activations,
    const std::vector<Eigen::MatrixXd>* weights = nullptr // optional
)
{
    // Clear window to a background color (light gray here).
    window.clear(sf::Color(220, 220, 220));

    if (activations.empty())
        return;

    // ----------------------------------------------------------------
    //  1) Collect basic info
    // ----------------------------------------------------------------
    float width  = static_cast<float>(window.getSize().x);
    float height = static_cast<float>(window.getSize().y);

    size_t numLayers = activations.size();

    // Number of columns = numLayers + 1 (for spacing)
    float layerSpacing = width / static_cast<float>(numLayers + 1);

    // Base radius for each neuron
    const float baseRadius = 15.f;

    // The *unscaled* vertical spacing we *want* between neurons
    // (If your largest layer is too big, we’ll scale this down)
    float neuronVerticalSpacing = 50.f;
// ----------------------------------------------------------------
    //  2) Find the LARGEST layer size
    // ----------------------------------------------------------------
    size_t maxNeuronsInLayer = 0;
    for (const auto& layer : activations) {
        maxNeuronsInLayer = std::max(maxNeuronsInLayer, (size_t)layer.size());
    }

    // If there's at least 1 neuron, compute how tall that largest layer would be
    // ignoring the radius for simplicity, just spacing * (count-1)
    float desiredMaxHeight = (maxNeuronsInLayer - 1) * neuronVerticalSpacing;

    // ----------------------------------------------------------------
    //  3) Apply a vertical scale if needed
    // ----------------------------------------------------------------
    // We’ll add a margin so we don’t fill the screen top-to-bottom
    float verticalMargin = 50.f;
    float maxDrawableHeight = height - verticalMargin; // some top/bottom margin

    float scale = 1.f;
    if (desiredMaxHeight > maxDrawableHeight && maxDrawableHeight > 0.f) {
        scale = maxDrawableHeight / desiredMaxHeight;
    }

    // So the *actual* vertical spacing is scaled
    float scaledSpacing = neuronVerticalSpacing * scale;

    // Optional: we could also scale the radius if we want smaller circles
    // float scaledRadius = baseRadius * scale;
    // But if you prefer to keep circles a consistent size, leave radius alone.

    // ----------------------------------------------------------------
    //  4) Compute positions for each neuron, applying our scale
    // ----------------------------------------------------------------
    std::vector<std::vector<sf::Vector2f>> neuronPositions(numLayers);

    for (size_t layerIndex = 0; layerIndex < numLayers; ++layerIndex)
    {
        size_t neuronCount = activations[layerIndex].size();
        if (neuronCount == 0) { 
            continue; 
        }

        // The total height of this layer (in “scaledSpacing” increments)
        float totalLayerHeight = (neuronCount - 1) * scaledSpacing;
        // Where the top neuron goes so the layer is centered vertically
        float startY = (height - totalLayerHeight) / 2.0f;

        // Horizontal position: (layerIndex + 1) * layerSpacing
        float xPos = (layerIndex + 1) * layerSpacing;

        neuronPositions[layerIndex].resize(neuronCount);

        for (size_t neuronIndex = 0; neuronIndex < neuronCount; ++neuronIndex)
        {
            float yPos = startY + neuronIndex * scaledSpacing;
            neuronPositions[layerIndex][neuronIndex] = sf::Vector2f(xPos, yPos);
        }
    }


     // ----------------------------------------------------------------
    //  5) Draw lines (if weights provided)
    // ----------------------------------------------------------------
    if (weights && weights->size() == (numLayers - 1))
    {
        for (size_t l = 0; l < numLayers - 1; ++l)
        {
            const auto& wMat = (*weights)[l];
            // wMat.rows() = neurons in layer(l+1)
            // wMat.cols() = neurons in layer(l)
            for (int nextNeuron = 0; nextNeuron < wMat.rows(); ++nextNeuron)
            {
                for (int currNeuron = 0; currNeuron < wMat.cols(); ++currNeuron)
                {
                    // Start in layer l
                    if (currNeuron >= (int)neuronPositions[l].size()) {
                        continue;
                    }
                    // End in layer l+1
                    if (nextNeuron >= (int)neuronPositions[l+1].size()) {
                        continue;
                    }

                    sf::Vector2f startPos = neuronPositions[l][currNeuron];
                    sf::Vector2f endPos   = neuronPositions[l+1][nextNeuron];

                    // We can color by sign or magnitude
                    double wVal = wMat(nextNeuron, currNeuron);
                    int intensity = static_cast<int>(
                        std::clamp(std::fabs(wVal)*255.0, 0.0, 255.0)
                    );
                    sf::Color edgeColor = (wVal >= 0.0)
                        ? sf::Color(intensity, 0, 0)    // red for positive
                        : sf::Color(0, 0, intensity);   // blue for negative

                    std::array<sf::Vertex, 2> line = {
                        sf::Vertex{ startPos, edgeColor },
                        sf::Vertex{ endPos,   edgeColor }
                    };
                    window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
                }
            }
        }
    }

    // -----------------------------------------------------------
    // 3) Draw each neuron as a circle
    // -----------------------------------------------------------
    for (size_t layerIndex = 0; layerIndex < numLayers; ++layerIndex)
    {
        size_t neuronCount = activations[layerIndex].size();
        for (size_t neuronIndex = 0; neuronIndex < neuronCount; ++neuronIndex)
        {
            float activation = static_cast<float>(activations[layerIndex](neuronIndex));

            // Map activation [-1..+1] to [0..255] or use your own color scheme
            int colorValue = static_cast<int>((activation + 1.f) * 127.5f);
            colorValue = std::clamp(colorValue, 0, 255);

            // Optionally scale the circle radius by |activation|
            float radius = baseRadius;
            // radius *= 0.5f + 0.5f * std::fabs(activation);

            sf::CircleShape neuronShape(radius);
            // Center the circle on its position (so position is at the center, not top-left)
            neuronShape.setOrigin({radius, radius});
            neuronShape.setPosition(neuronPositions[layerIndex][neuronIndex]);
            neuronShape.setFillColor(sf::Color(colorValue, colorValue, colorValue));
            neuronShape.setOutlineColor(sf::Color::Black);
            neuronShape.setOutlineThickness(2.0f);

            window.draw(neuronShape);
        }
    }
}


void animatePendulum(sf::RenderWindow& window, double angle)
{
    window.clear(sf::Color::Blue);

    // Convert angle to (x, y) coordinates
    double length = 200.0; // Length of the pendulum in pixels
    double originX = 400.0; // Center of the window
    double originY = 200.0;
    double x = originX + length * std::sin(angle);
    double y = originY + length * std::cos(angle);

    sf::Vertex pendelum_line[2];
    //pendelum_line.resize(2);
    pendelum_line[0].position = sf::Vector2f(originX,originY);
    pendelum_line[0].color = sf::Color::Black;
    pendelum_line[1].position = sf::Vector2f(x,y);
    pendelum_line[1].color = sf::Color::Black;

    window.draw(pendelum_line,2, sf::PrimitiveType::Lines);

    // Draw pendulum bob
    sf::CircleShape bob(10);
    bob.setFillColor(sf::Color::Red);
    bob.setPosition(sf::Vector2f(x-10, y-10));
    //bob.setPosition(x - 10, y - 10); // Center the circle
    
    window.draw(bob);

}



int main() 
{
    
    double frame_rate = 60.0;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Inverted Pendulum");
    
    sf::RenderWindow nn_window(sf::VideoMode(sf::Vector2u(800, 600)), "Neural Network");
    sf::FloatRect view_rect;
    view_rect.position = {0.f, 0.f};
    view_rect.size = {800.f, 600.f};
    sf::View view(view_rect);  // "world" coords

    nn_window.setView(view);

    PendelumOptions options;
    options.gravity_accel = 9.81;
    options.initial_angle = 0.1;
    options.initial_angular_velocity = 0.1;
    options.length = 1;
    options.mass = 100;

    std::shared_ptr<InvertedPendelum> inverted_pendelum = std::make_shared<InvertedPendelum>(options);
    
    Simulation simulation;
    //auto pid_controller = std::make_shared<PIDController>(82850.0, 1.0, 76628.0);
    auto pid_controller = std::make_shared<PIDController>(1, 1.0, 2.0);
    std::vector<size_t> layout = {5, 32, 32, 3};
    
    auto agent = std::make_shared<RLAgent>(layout, 0.001, 0.5, 0.9);

    double gain_update_rate = 5.0;
    double gains_time = 0.0;
    bool gains_updated = false;

    double time_delay = 5.0;
    double time = 0.0;
    double previous_error = 0.0;
    Eigen::VectorXd action;
    Eigen::VectorXd before_state(5);
    double setpoint = M_PI;
    std::vector<Eigen::VectorXd> activations;

    bool pid_tune_jobs = true;
    auto before_pendelum = std::make_shared<FunctionJob>(
        "BeforePendelum",
        [&inverted_pendelum, &pid_controller, &agent, &previous_error, &action, &before_state, &activations, &setpoint, &pid_tune_jobs, &gains_time, &gain_update_rate, &gains_updated](double deltaTime)
        {
            gains_time += deltaTime;
            if(pid_tune_jobs)
            {

                if(gains_time > gain_update_rate)
                {
                    double current_control_signal = pid_controller->GetOutput(setpoint, inverted_pendelum->GetAngle(), deltaTime);
                    
                    //std::cout << "PIDControl Signal: " << control_signal << "\n";
                
                    
                    before_state << pid_controller->GetError(), pid_controller->GetErrorIntegral(), pid_controller->GetErrorDerivative(), inverted_pendelum->GetAngle(), inverted_pendelum->GetAngularVelocity();
                    
                    // Select an action
                    action = agent->selectAction(before_state);

                    double Kp = std::clamp(action(0), -5.0, 5.0);
                    double Ki = std::clamp(action(1), -5.0, 5.0);
                    double Kd = std::clamp(action(2), -5.0, 5.0);

                    pid_controller->SetGains(Kp, Ki, Kd);

                    previous_error = pid_controller->GetError();
                    gains_time = 0.0;
                    gains_updated = true;
                }
            }

        }
    );

    bool start_update_timer = false;

    auto after_pendelum = std::make_shared<FunctionJob>(
        "AfterPendelum",
        [&inverted_pendelum, &pid_controller, &agent, &time, &time_delay, &previous_error, &action, &before_state, &setpoint, &pid_tune_jobs, &gains_updated, &start_update_timer](double deltaTime)
        {

            if(gains_updated)
            {
                start_update_timer = true;
                gains_updated = false;
            }

            if(pid_tune_jobs)
            {
                if(start_update_timer)
                {
                    time += deltaTime;

                    if(time > time_delay)
                    {
                        Eigen::VectorXd state(3);
                    
                
                        //double current_control_signal = pid_controller->GetOutput(setpoint, inverted_pendelum->GetAngle(), deltaTime);
                        double current_control_signal = std::clamp(pid_controller->GetOutput(setpoint, inverted_pendelum->GetAngle(), deltaTime), -100.0, 100.0);

                        Eigen::VectorXd next_state(5);
                        next_state << pid_controller->GetError(), pid_controller->GetErrorIntegral(), pid_controller->GetErrorDerivative(), inverted_pendelum->GetAngle(), inverted_pendelum->GetAngularVelocity();
                        
                        // Calculate reward (e.g., negative squared error)
                        double total_reward = 0.0;
                        double angle_error_reward = -std::pow(pid_controller->GetError(), 2);
                        double angle_velocity_reward  = 0;

                        if(inverted_pendelum->GetAngle()*M_PI/180.0 > 160 || inverted_pendelum->GetAngle()*M_PI/180.0 < -160)
                        {
                            angle_velocity_reward = -std::pow(inverted_pendelum->GetAngularVelocity(), 2);
                        }
                        
                        
                        
                        
                       

                        total_reward = (angle_error_reward + angle_velocity_reward);
                        std::cout << "Loss: " << total_reward << "\n";
                        // Update the RL agent
                        agent->Update(before_state, action, total_reward, next_state);

                        time = 0.0;

                        start_update_timer = false;
                    }
                }
            }
        }
    );


    auto pendelum_job = std::make_shared<FunctionJob>(
        "PendelumJob", 
        [&inverted_pendelum, &pid_controller, &agent, &setpoint](double deltaTime) { 
            pid_controller->Update(setpoint, inverted_pendelum->GetAngle(), deltaTime);
            double current_control_signal = pid_controller->GetOutput(setpoint, inverted_pendelum->GetAngle(), deltaTime);
            current_control_signal = std::clamp(current_control_signal, -800.0, 800.0);
            inverted_pendelum->CollectTorque(current_control_signal);
            //std::cout << "Angle: " << inverted_pendelum->GetAngle() << "\n";
        }
    );


    auto rk4_integrator = std::make_shared<RungeKutta4Integrator>();
    
    rk4_integrator->AddDerivativeFunction(
        [&inverted_pendelum](double time, Eigen::VectorXd& state) mutable -> Eigen::VectorXd
        {
            return inverted_pendelum->ComputeDeriv(time, state);
        },
        inverted_pendelum->GetStateRef()
        );


    auto integrator_job = std::make_shared<FunctionJob>(
    "RungeKuttaIntegratorJob",
    [&rk4_integrator, &inverted_pendelum](double deltaTime)
    {
        rk4_integrator->Integrate(deltaTime);
    }
    );

    auto post_integrator_job = std::make_shared<FunctionJob>(
        "Rk4_Post",
        [&rk4_integrator, &inverted_pendelum](double deltaTime)
        {
            inverted_pendelum->ApplyIntegration();
        }
    );
    
    integrator_job->AddRunAfterJob({"",post_integrator_job});
    integrator_job->SetJobRate(0.01);

    auto pid_before_pair = std::make_pair("PIDBefore", before_pendelum);
    auto pid_after_pair = std::make_pair("PIDAfter", after_pendelum);
    
    simulation.AddJob(integrator_job, SimulationJobTypes::Integrator);

    simulation.AddJob(before_pendelum, SimulationJobTypes::Scheduled);
    simulation.AddJob(pendelum_job, SimulationJobTypes::Scheduled);
    simulation.AddJob(after_pendelum, SimulationJobTypes::Scheduled);


    simulation.SetTimeScale(20.0);

   

    /**
    NeuralNetworkLayout layout;
    layout.hidden_sizes = {16, 24, 16};
    layout.input_size = 3;
    layout.num_hidden = 3;
    layout.output_size = 3;

    NeuralNetwork neural_network(layout);
     */
   
    
    window.setFramerateLimit(160);
    nn_window.setFramerateLimit(160);

    std::thread sim_thread([&]() {
        simulation.Run();
    });

    double last_network_update = 0.0;

    double network_update_interval = 5.0;

    bool first_pass = true;
    Eigen::VectorXd state;
    state.resize(2);
    state << inverted_pendelum->GetAngle(), inverted_pendelum->GetAngularVelocity();

    while(window.isOpen() && nn_window.isOpen())
    {   

    
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                simulation.Stop();
                sim_thread.join();
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
                    simulation.Stop();
                    sim_thread.join();
                }
                else if(keyPressed->scancode == sf::Keyboard::Scancode::A)
                {
                    if(pid_tune_jobs)
                    {
                        pid_tune_jobs = false;
                    }
                    else if(!pid_tune_jobs)
                    {
                        pid_tune_jobs = true;
                    }
                }
            }
        }

        while (const std::optional event = nn_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                simulation.Stop();
                sim_thread.join();
                window.close();
                nn_window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    simulation.Stop();
                    sim_thread.join();
                    window.close();
                    nn_window.close();
                }
                
            }
            else if (const auto* resizeEvent = event->getIf<sf::Event::Resized>())
            {
                
                float newWidth  = static_cast<float>(resizeEvent->size.x);
                float newHeight = static_cast<float>(resizeEvent->size.y);

                // 2) Reset the view so that our "logical" 800x600 space
                //    scales to the new window size
                view.setSize({800.f, 600.f}); 
                view.setCenter({400.f, 300.f});

                // Optionally keep aspect ratio or fill entire window
                // If you want to keep the same aspect ratio:
                // float aspectRatio = newWidth / newHeight;
                // view.setSize(600.f*aspectRatio, 600.f);

                // 3) Apply the new view
                nn_window.setView(view);

            }
        }
        
        animatePendulum(window, inverted_pendelum->GetAngle());
        DisplayInfo(window, inverted_pendelum, pid_controller);
        if(agent->GetActivations().size() > 0)
        {
           
            VisualizeNeuralNetwork(nn_window, agent->GetActivations(),&agent->GetNetwork().GetWeights() );
        }

        window.display();
        nn_window.display();
    }


    return EXIT_SUCCESS;
}