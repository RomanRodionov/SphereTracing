cmake_minimum_required(VERSION 3.5.0)
project(SphereTracing VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
option(USE_VULKAN "Enable GPU implementation via Vulkan" ON)

find_package(OpenMP)

include(CTest)
enable_testing()

if(USE_VULKAN)

  find_package(Vulkan)

  if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
  elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
  elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
  endif()

  add_subdirectory(external/volk)

  add_compile_definitions(USE_VOLK)
  add_compile_definitions(USE_VULKAN)

  include_directories(${Vulkan_INCLUDE_DIR})

  include_directories("external/LiteMath"
                      "external/vkutils"
                      "external/volk")

  link_directories("../volk")
else()
  include_directories("external/LiteMath")
endif()

include_directories(".")
if(WIN32)
  link_directories(${ADDITIONAL_LIBRARY_DIRS})
endif()

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes") # get rid of annoying gcc warnings (unknown attributes)
  set(PLATFORM_DEPENDEPNT_LIBS ${Vulkan_LIBRARY} dl)
endif()

set(SOURCES
    public_camera.cpp
    public_scene.cpp
    sphere_tracing.cpp
    main.cpp
)
set(GPU_SOURCES
    sphere_tracing_gpu.cpp
    sphere_tracing_gpu_ds.cpp
    sphere_tracing_gpu_init.cpp

    external/vkutils/vk_utils.cpp
    external/vkutils/vk_copy.cpp
    external/vkutils/vk_context.cpp
    external/vkutils/vk_alloc_simple.cpp
    external/vkutils/vk_buffers.cpp
    external/vkutils/vk_images.cpp
    external/vkutils/vk_pipeline.cpp
    external/vkutils/vk_descriptor_sets.cpp
)

if(USE_VULKAN)
  add_executable(Run ${SOURCES} ${GPU_SOURCES})

  set_target_properties(Run PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  target_link_libraries(Run LINK_PUBLIC OpenMP::OpenMP_CXX volk "${PLATFORM_DEPENDEPNT_LIBS}")

else()
  add_executable(Run ${SOURCES})

  set_target_properties(Run PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  target_link_libraries(Run LINK_PUBLIC OpenMP::OpenMP_CXX)
endif()
