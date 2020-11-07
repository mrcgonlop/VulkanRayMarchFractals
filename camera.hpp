#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Camera
{
public:
	glm::vec3 position = { 1.5f, 1.5f, 1.5f };
	glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
	float rotation_speed = glm::pi<float>();
	float move_speed = 10.f;

	//float fov;
	//glm::vec2 near;
	//glm::vec2 far;

	glm::mat4 getViewMatrix() const
	{
		// return glm::inverse(glm::translate(glm::toMat4(rotation), position));
		return glm::transpose(glm::toMat4(rotation)) * glm::translate(glm::mat4(1.0f), -position); // equal to inv(TR) 
	}
};

// for test use
struct TestSceneConfiguration
{
	//std::string model_file;
	float scale;
	glm::vec3 min_light_pos;
	glm::vec3 max_light_pos;
	float light_radius;
	int light_num;
	glm::vec3 camera_position;
	glm::quat camera_rotation;
};