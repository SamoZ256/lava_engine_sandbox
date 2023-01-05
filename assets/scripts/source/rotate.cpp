#include "lava_system.hpp"

#include <iostream>

class RotateEntity : public Entity {
public:
    using Entity::Entity;

    void start() {
        std::cout << "Starting rotate script" << std::endl;
    }

    void update(float dt) {
        if (Input::getKeyState(this, LVND_KEY_M) == LVND_STATE_PRESSED) {
            if (hasComponent<TransformComponent>())
                getComponent<TransformComponent>().rotation.y += 60.0f * dt;
        }
    }
};

REGISTER_SCRIPT_CLASS(RotateEntity);
