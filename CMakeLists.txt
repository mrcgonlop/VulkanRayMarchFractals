cmake_minimum_required(VERSION 3.16.3)
project(VulkanRayMarchFractals)

set(CMAKE_CXX_STANDARD 17)

# file(COPY shaders/frag.spv DESTINATION shaders)
# file(COPY shaders/vert.spv DESTINATION shaders)
# file(COPY shaders/comp.spv DESTINATION shaders)
link_libraries(glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi)

add_executable(EasyVulkan main.cpp VulkanWindowApp.hpp 
  VulkanMaster.hpp camera.hpp vulkancomponents/SwapChainUtils.hpp
  vulkancomponents/Renderer.hpp)