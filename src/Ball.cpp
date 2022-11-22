#include <tuple>
#include "Ball.h"


namespace Bout {

    BallObject::BallObject()
        : GameObject(), Radius(12.5f), Stuck(true) { }

    BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
        : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

    glm::vec2 BallObject::Move(float dt, unsigned int window_width)
    {
        // if not stuck to player board
        if (!this->Stuck)
        {
            // move the ball
            this->Position += this->Velocity * dt;
            // then check if outside window bounds and if so, reverse velocity and restore at correct position
            if (this->Position.x <= 0.0f)
            {
                this->Velocity.x = -this->Velocity.x;
                this->Position.x = 0.0f;
            }
            else if (this->Position.x + this->Size.x >= window_width)
            {
                this->Velocity.x = -this->Velocity.x;
                this->Position.x = window_width - this->Size.x;
            }
            if (this->Position.y <= 0.0f)
            {
                this->Velocity.y = -this->Velocity.y;
                this->Position.y = 0.0f;
            }
        }
        return this->Position;
    }

    // resets the ball to initial Stuck Position (if ball is outside window bounds)
    void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
    {
        this->Position = position;
        this->Velocity = velocity;
        this->Stuck = true;

    }

    bool BallObject::CheckCollisionAABB(GameObject& brick)
    {
        // Test the overlap of both boxes at the same imt
        bool collisionX = this->Position.x + this->Size.x >= brick.Position.x
            && brick.Position.x + brick.Size.x >= this->Position.x;

        bool collisionY = this->Position.y + this->Size.y >= brick.Position.y
            && brick.Position.y + brick.Size.y >= this->Position.y;

        return collisionX && collisionY;
    }

    Direction VectorDirection(glm::vec2 target)
    {
        glm::vec2 compass[] = {
            glm::vec2(0.0f, 1.0f),	// up
            glm::vec2(1.0f, 0.0f),	// right
            glm::vec2(0.0f, -1.0f),	// down
            glm::vec2(-1.0f, 0.0f)	// left
        };

        int best_match = -1;
        float max = 0.f;
        for (int i = 0; i < 4; ++i) {
            float dot_product = glm::dot(glm::normalize(target), compass[i]);
            if (dot_product > max) {
                max = dot_product;
                best_match = i;
            }
        }

        return (Direction)best_match;
    }

    Collision BallObject::CheckCollisionCircle(GameObject& brick)
    {
        // Get center of circle
        auto center = this->Position + this->Radius;
        
        // Get half extents to clamp position
        auto aabb_half_extents = brick.Size / 2.f;
        auto aabb_center = brick.Position + aabb_half_extents;

        // Get difference vector from center of 2 objects
        auto difference = center - aabb_center;
        auto clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
        
        // Find the closest point based on the clamped vector from center
        auto closest = aabb_center + clamped;
        difference = closest - center;
        
        
        if (glm::length(difference) < this->Radius)
            // We return the difference so we can move the ball back to that position
            // So objects dont merge into each other
            return std::make_tuple(true, VectorDirection(difference), difference);
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));;
    }


}