#include "lava_system.hpp"

#include <iostream>

class MoveEntity : public Entity {
private:
    float timePassed = 0.0f;

public:
    using Entity::Entity;

    void start() {
        std::cout << "Starting move script" << std::endl;
    }

    void update(float dt) {
        timePassed += dt;
        if (hasComponent<TransformComponent>()) {
            getComponent<TransformComponent>().position.y = std::sin(timePassed);
        }
    }
};

REGISTER_SCRIPT_CLASS(MoveEntity);
