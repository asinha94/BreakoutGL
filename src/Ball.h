
#include "GameObject.h"

namespace Bout {
	class BallObject : public GameObject
	{
	public:
		float Radius;
		bool  Stuck;

		BallObject();
		BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

		glm::vec2 Move(float dr, unsigned int window_width);
		void Reset(glm::vec2 position, glm::vec2 velocity);
	};
}