
cmake_minimum_required(VERSION 3.10)

project(rocket_modeling)
set (SRC "./src")
set (INCLUDES "./include")
set (GLAD_INCLUDE "./include/glad/include")
set (GLFW_INCLUDE  "./GLFW/include")
set (GLFW "./GLFW/src")
set (MATHS "./Maths")


set (COMMON_GLFW ${GLFW}/context.c ${GLFW}/window.c ${GLFW}/init.c ${GLFW}/input.c ${GLFW}/vulkan.c ${GLFW}/egl_context.c ${GLFW}/osmesa_context.c ${GLFW}/monitor.c)
set (WIN32_GLFW ${GLFW}/win32_thread.c ${GLFW}/win32_window.c ${GLFW}/win32_monitor.c ${GLFW}/win32_init.c ${GLFW}/win32_time.c ${GLFW}/win32_joystick.c ${GLFW}/wgl_context.c)
set (X11_GLFW  ${GLFW}/posix_thread.c  ${GLFW}/x11_window.c  ${GLFW}/x11_monitor.c ${GLFW}/x11_init.c  ${GLFW}/posix_time.c  ${GLFW}/linux_joystick.c  ${GLFW}/glx_context.c  ${GLFW}/xkb_unicode.c)

include_directories(${GLFW_INCLUDE} ${GLAD_INCLUDE}) 
if(WIN32)
	add_executable(rocket ${SRC}/main.cpp ${SRC}/renderer.cpp ${SRC}/cube.cpp ${SRC}/skybox.cpp ${SRC}/helpers.cpp ${SRC}/model.cpp ${SRC}/mesh.cpp ${INCLUDES}/glad/src/glad.c ${COMMON_GLFW} ${WIN32_GLFW} )
	target_link_libraries(rocket gdi32 assimp kernel32 user32)
	set (CMAKE_CXX_FLAGS "-O3 -std=c++17")
	add_compile_definitions(_GLFW_WIN32)
endif (WIN32)

if (UNIX)
        add_executable(rocket  ${SRC}/main.cpp ${SRC}/renderer.cpp ${SRC}/cube.cpp ${SRC}/skybox.cpp ${SRC}/helpers.cpp ${SRC}/model.cpp ${SRC}/mesh.cpp ${INCLUDES}/glad/src/glad.c ${COMMON_GLFW} ${X11_GLFW})
	target_link_libraries(rocket pthread dl X11 m assimp)
	set (CMAKE_CXX_FLAGS "-std=c++17")
	add_compile_definitions(_GLFW_X11)
endif (UNIX)






