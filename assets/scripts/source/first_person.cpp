#include "lava_system.hpp"

#include <iostream>

class FirstPersonEntity : public Entity {
private:
    bool firstClick = true;
    bool drag = false;
    const float sensitivity = 100.0f;

    float yMomentum = 0.0f;
    const float gravity = -10.0f;
    const float speed = 10.0f;

    bool spaceFirstClick = true;

public:
    using Entity::Entity;

    void start() {
        
    }

    void update(float dt) {
        lv::TransformComponent& transformComponent = getComponent<lv::TransformComponent>();
        glm::vec3 direction = glm::toMat3(glm::quat(glm::radians(transformComponent.rotation))) * glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 movement = glm::vec3(0.0f);
        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_W) == LVND_STATE_PRESSED) {
            movement += direction * speed;
        }
        if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_A) == LVND_STATE_PRESSED) {
            movement += -glm::normalize(glm::cross(direction, up)) * speed;
        }
        if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_S) == LVND_STATE_PRESSED) {
            movement += -direction * speed;
        }
        if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_D) == LVND_STATE_PRESSED) {
            movement += glm::normalize(glm::cross(direction, up)) * speed;
        }

        if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_SPACE) == LVND_STATE_PRESSED) {
            if (spaceFirstClick) {
                spaceFirstClick = false;
                yMomentum += 10.0f;
            }
        } else if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_SPACE) == LVND_STATE_RELEASED) {
            spaceFirstClick = true;
        }

        //Gravity
        yMomentum += gravity * dt;
        movement.y = yMomentum;

        transformComponent.position += movement * dt;
        if (transformComponent.position.y < 1.0f) {
            transformComponent.position.y = 1.0f;
            yMomentum = 0.0f;
        }

        uint16_t width, height;
        lvndGetWindowSize((LvndWindow*)window, &width, &height);

        int32_t mouseX, mouseY;
        lvndGetCursorPosition((LvndWindow*)window, &mouseX, &mouseY);

        if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_ESCAPE) == LVND_STATE_PRESSED) {
            if (firstClick) {
                firstClick = false;
                drag = !drag;
                mouseX = (width * 0.5f);
                mouseY = (height * 0.5f);
                if (drag) {
                    lvndSetCursorState((LvndWindow*)window, LVND_CURSOR_STATE_HIDDEN);
                } else {
                    lvndSetCursorState((LvndWindow*)window, LVND_CURSOR_STATE_NORMAL);
                }
            }
        } else if (lvndGetKeyState((LvndWindow*)window, LVND_KEY_ESCAPE) == LVND_STATE_RELEASED) {
            // Makes sure the next time the camera looks around it doesn't jump
            firstClick = true;
        }
        
        if (drag) {
            lvndSetCursorPosition((LvndWindow*)window, (width * 0.5f), (height * 0.5f));
            float rotX = sensitivity * float(mouseY - (height * 0.5f)) / float(height);
            float rotY = sensitivity * float(mouseX - (width * 0.5f)) / float(width);

            transformComponent.rotation.x -= rotX;
			transformComponent.rotation.y -= rotY;
        }
    }
};

REGISTER_SCRIPT_CLASS(FirstPersonEntity);
