#include "VulkanTools.hpp"
#include "camera.hpp"
#include <chrono>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
constexpr float MIN_DELTA_TIME = 1.0f / 60.0f;
constexpr float SMALL_NUMBER = 1e-8f;

class MyVulkanWindowApp {
public:
    void run(){
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow *window;
    VulkanTools vulkanTools;

    bool lmb_down = false;
	bool rmb_down = false;
	glm::vec2 cursor_pos = { 0.0f, 0.0f };
	glm::vec2 prev_cursor_pos = { 0.0f, 0.0f };
    bool w_down = false; // todo use a hash map or something
	bool s_down = false;
	bool a_down = false;
	bool d_down = false;
	bool q_down = false;
	bool e_down = false;
	bool z_pressed = false;
    glm::vec2 framebuffer_size = { WIDTH, HEIGHT };
    bool framebufferResized = false;

    Camera camera;

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
		auto cursorpos_callback = [](GLFWwindow* window, double xPos, double yPos)
		{
			auto app = reinterpret_cast<MyVulkanWindowApp*>(glfwGetWindowUserPointer(window));
			app->onCursorPosChanged(xPos, yPos);
		};

		auto mousebutton_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			auto app = reinterpret_cast<MyVulkanWindowApp*>(glfwGetWindowUserPointer(window));
			app->onMouseButton(button, action, mods);
		};

		auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto app = reinterpret_cast<MyVulkanWindowApp*>(glfwGetWindowUserPointer(window));
			app->onKeyPress(key, scancode, action, mods);
		};
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetCursorPosCallback(window, cursorpos_callback);
		glfwSetMouseButtonCallback(window, mousebutton_callback);

		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		framebuffer_size = { w, h };

		glfwSetKeyCallback(window, key_callback);
    }

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<MyVulkanWindowApp *>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void initVulkan() {
        vulkanTools.setUp(window);
    }

    void mainLoop() {
        auto start_time = std::chrono::high_resolution_clock::now();
		auto previous = std::chrono::high_resolution_clock::now();
		decltype(previous) current;
		float delta_time;
        while (!glfwWindowShouldClose(window)) {
            current = std::chrono::high_resolution_clock::now();
			delta_time = std::chrono::duration<float>(current - previous).count();
            glfwPollEvents();
            if (delta_time >= MIN_DELTA_TIME) /**prevent underflow*/{
				tick(delta_time);
				previous = current;
			}
        }
    }

    void cleanup(){
        vulkanTools.cleanUp();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void onMouseButton(int button, int action, int mods){
		if (action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			cursor_pos = {x, y};
			prev_cursor_pos = {x, y};
			if (button == GLFW_MOUSE_BUTTON_LEFT){
				lmb_down = true;
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT){
				rmb_down = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
		else if (action == GLFW_RELEASE){
			if (button == GLFW_MOUSE_BUTTON_LEFT){
				lmb_down = false;
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT){
				rmb_down = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

    void onKeyPress(int key, int scancode, int action, int mods){
		if (action == GLFW_PRESS) {
			switch (key) {
				// todo use a hash map or something
			    case GLFW_KEY_W:
				    w_down = true;
					break;
			    case GLFW_KEY_S:
				    s_down = true;
					break;
				case GLFW_KEY_A:
					a_down = true;
					break;
				case GLFW_KEY_D:
					d_down = true;
					break;
				case GLFW_KEY_Q:
					q_down = true;
					break;
				case GLFW_KEY_E:
					e_down = true;
					break;
			}
		}
		else if (action == GLFW_RELEASE) {
			switch (key) {
			    case GLFW_KEY_W:
				    w_down = false;
					break;
			    case GLFW_KEY_S:
				    s_down = false;
					break;
				case GLFW_KEY_A:
					a_down = false;
					break;
				case GLFW_KEY_D:
					d_down = false;
					break;
				case GLFW_KEY_Q:
					q_down = false;
					break;
				case GLFW_KEY_E:
					e_down = false;
					break;
				case GLFW_KEY_Z:
					z_pressed = true;
			}
		}
	}

    void onCursorPosChanged(double xPos, double yPos) {
		//std::cout << xPos << "," << yPos << std::endl;
		if (!lmb_down && !rmb_down) {
			return;
		}
		else if (lmb_down) {
			//glm::vec2 tmp(xPos, yPos);
			//modelRotAngles += (tmp - cursorPos) * 0.01f;
			cursor_pos = { xPos, yPos };
		}
		else if (rmb_down) {
			glm::vec2 tmp(xPos, yPos);
			cursor_pos = { xPos, yPos };
		}
	}

    bool isNearlyEqual(float a, float b, float tolerance = SMALL_NUMBER){
		return glm::abs(a - b) <= tolerance;
	}

    glm::vec3 vec_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 vec_right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 vec_forward = glm::vec3(0.0f, 0.0f, -1.0f);

    void tick(float delta_time){
		if (rmb_down){
			if (isNearlyEqual(framebuffer_size.x, 0.f) || isNearlyEqual(framebuffer_size.x, 0.f)){
				return;
			}
			auto cursor_delta = (cursor_pos - prev_cursor_pos) / glm::min(framebuffer_size.x, framebuffer_size.y) * 2.0f;
			if (!isNearlyEqual(cursor_delta.x, 0)){
				camera.rotation = glm::angleAxis(camera.rotation_speed * -cursor_delta.x, vec_up) * camera.rotation; // world up
			}
			if (!isNearlyEqual(cursor_delta.y, 0)){
				camera.rotation = camera.rotation * glm::angleAxis(camera.rotation_speed * -cursor_delta.y, vec_right); // local right
			}
			camera.rotation = glm::normalize(camera.rotation);
			prev_cursor_pos = cursor_pos;
		}
		if (w_down) {
			// forward
			camera.position += camera.rotation  * vec_forward * camera.move_speed * delta_time;
		}
		if (s_down) {
			// back
			camera.position -= camera.rotation  * vec_forward * camera.move_speed * delta_time;
		}
		if (a_down){
			// left
			camera.position -= camera.rotation  * vec_right * camera.move_speed * delta_time;
		}
		if (d_down){
			// right
			camera.position += camera.rotation  * vec_right * camera.move_speed * delta_time;
		}
		if (q_down){
			// up
			camera.position -= vec_up * camera.move_speed * delta_time;
		}
		if (e_down){
			// down
			camera.position += vec_up * camera.move_speed * delta_time;
		}
	}

};