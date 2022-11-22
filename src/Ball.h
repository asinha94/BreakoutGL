#include <tuple>
#include "GameObject.h"

namespace Bout {

	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	typedef std::tuple<bool, Direction, glm::vec2> Collision;

	class BallObject : public GameObject
	{
	public:
		float Radius;
		bool  Stuck;

		BallObject();
		BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

		bool CheckCollisionAABB(GameObject& brick);
		Collision CheckCollisionCircle(GameObject& brick);

		glm::vec2 Move(float dr, unsigned int window_width);
		void Reset(glm::vec2 position, glm::vec2 velocity);
	};
}