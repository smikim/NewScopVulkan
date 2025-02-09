# CMakeList.txt: Scop에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

# 지원되는 경우 MSVC 컴파일러에 대해 핫 다시 로드 사용하도록 설정합니다.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")
 
set(NAME Scop)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()
 
project(${NAME} VERSION 0.23.0)
 
# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()
 
 
# 2. Set GLFW_PATH in .env.cmake to target specific glfw
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2022") # 2.1 Update lib-vc2019 to use same version as your visual studio
  elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    message(STATUS "USING MINGW")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64") # 2.1 make sure matches glfw mingw subdirectory
  endif()
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
    message(FATAL_ERROR "Could not find glfw library!")
else()
    message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()
 
include_directories(external)
 
# If TINYOBJ_PATH not specified in .env.cmake, try fetching from git repo
if (NOT TINYOBJ_PATH)
  message(STATUS "TINYOBJ_PATH not specified in .env.cmake, using external/tinyobjloader")
  set(TINYOBJ_PATH external/tinyobjloader)
endif()
 
set(APP_FILES
    # Headers Files    
    Scop_App/App/Scop.hpp
    Scop_App/App/ScopObject.hpp
    Scop_App/App/HumanGLObject.hpp
    # Source Files
    Scop_App/App/Scop.cpp
    Scop_App/App/ScopObject.cpp
    Scop_App/App/HumanGLObject.cpp
)

set(COMPONENT_FILES
    # Headers Files    
    Scop_App/Component/Component.hpp
    Scop_App/Component/Transform.hpp
    Scop_App/Component/Animation.hpp
    # Source Files
    Scop_App/Component/Component.cpp
    Scop_App/Component/Transform.cpp
    Scop_App/Component/Animation.cpp
)
set(GRAPHICS_FILES
    # Headers Files    
    Scop_App/Graphics/BasicPSO.hpp
    # Source Files
    Scop_App/Graphics/BasicPSO.cpp    
)

set(INCLUDES_FILES 
    # Headers Files    
    Scop_App/Includes/Common/IVulkanModel.hpp
    Scop_App/Includes/Common/Vertex.hpp
    Scop_App/Includes/Common/ShaderData.hpp
    Scop_App/Includes/helpers.hpp
    Scop_App/Includes/KeyboardMovementController.hpp
    Scop_App/Includes/Types.hpp
    Scop_App/Includes/VulkanBuffer.hpp
    Scop_App/Includes/VulkanCommandBuffer.hpp
    Scop_App/Includes/VulkanDebug.hpp
    Scop_App/Includes/VulkanDescriptor.hpp
    Scop_App/Includes/VulkanDevice.hpp
    Scop_App/Includes/VulkanInitializers.hpp
    Scop_App/Includes/VulkanInstance.hpp
    Scop_App/Includes/VulkanModel.hpp
    Scop_App/Includes/VulkanPhysicalDevice.hpp
    Scop_App/Includes/VulkanPipeline.hpp
    Scop_App/Includes/VulkanPipelineState.hpp
    Scop_App/Includes/VulkanQueue.hpp
    Scop_App/Includes/VulkanRenderer.hpp
    Scop_App/Includes/VulkanSwapChain.hpp
    Scop_App/Includes/VulkanTexture.hpp
    Scop_App/Includes/VulkanTools.hpp
    Scop_App/Includes/VulkanUtils.hpp
    Scop_App/Includes/VulkanAnimationModel.hpp
    Scop_App/Includes/Input.hpp
)

set(LIBRAYRY_FILES
    # Headers Files  
    Scop_App/Library/Math/math.hpp
    Scop_App/Library/Math/Matrix.hpp
    Scop_App/Library/Math/Vector.hpp
    Scop_App/Library/BmpLoader.hpp
    Scop_App/Library/ObjMeshLoader.hpp
    # Source Files
    Scop_App/Library/Math/math.cpp
    Scop_App/Library/Math/Matrix.cpp
    Scop_App/Library/Math/Vector.cpp
    Scop_App/Library/BmpLoader.cpp
    Scop_App/Library/ObjMeshLoader.cpp
)

set(MAIN_FILE
    # Source Files
    Scop_App/Main/main.cpp
)

set(SRC_FILES 
    # Source Files
    Scop_App/Sources/Common/Vertex.cpp
    Scop_App/Sources/KeyboardMovementController.cpp
    Scop_App/Sources/VulkanBuffer.cpp
    Scop_App/Sources/VulkanCommandBuffer.cpp
    Scop_App/Sources/VulkanDebug.cpp
    Scop_App/Sources/VulkanDevice.cpp
    Scop_App/Sources/VulkanDescriptor.cpp
    Scop_App/Sources/VulkanInstance.cpp
    Scop_App/Sources/VulkanModel.cpp
    Scop_App/Sources/VulkanPhysicalDevice.cpp
    Scop_App/Sources/VulkanPipeline.cpp
    Scop_App/Sources/VulkanPipelineState.cpp
    Scop_App/Sources/VulkanQueue.cpp
    Scop_App/Sources/VulkanRenderer.cpp
    Scop_App/Sources/VulkanSwapChain.cpp
    Scop_App/Sources/VulkanTexture.cpp
    Scop_App/Sources/VulkanTools.cpp
    Scop_App/Sources/VulkanUtils.cpp
    Scop_App/Sources/VulkanAnimationModel.cpp
    Scop_App/Sources/Input.cpp

)

set(WINDOW_FILES
    # Headers Files   
    Scop_App/Window/GlfwWindow.hpp
    # Source Files
    Scop_App/Window/GlfwWindow.cpp
)
set(PROJECT_FILES
    ${APP_FILES}
    ${COMPONENT_FILES}
    ${GRAPHICS_FILES}
    ${INCLUDES_FILES}
    ${LIBRAYRY_FILES}
    ${MAIN_FILE}
    ${SRC_FILES}
    ${WINDOW_FILES}
)
 
add_executable(${PROJECT_NAME} ${PROJECT_FILES})

# ?�행 ?�일??출력 ?�렉?�리�??�정?�니??
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# ?�브?�트 ?�일 �?기�? 중간 ?�일??출력 ?�렉?�리�??�정?�니??
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/obj"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/obj"
)
message(STATUS "RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_SOURCE_DIR}/bin")
message(STATUS "ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_SOURCE_DIR}/obj")
message(STATUS "LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_SOURCE_DIR}/obj")

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")


# ?�행 ?�일??출력 ?�렉?�리�??�정?�니??
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

# ?�브?�트 ?�일 �?기�? 중간 ?�일??출력 ?�렉?�리�??�정?�니??
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/obj")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/obj")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/obj")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
 
# 컴파일러 경고 옵션 추가
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
 
if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
 
  if (USE_MINGW)
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/include
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/lib
    )
  endif()
 
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_FILES}
    ${Vulkan_INCLUDE_DIRS}
    ${TINYOBJ_PATH}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_PATH}
    )
 
  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )
 
  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_FILES}
      ${TINYOBJ_PATH}
    )
    target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
endif()
 
 
############## Build SHADERS #######################
 
# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)

# ?�버�?메시지 출력
message(STATUS "GLSL_VALIDATOR: ${GLSL_VALIDATOR}")

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/Scop_App/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/Scop_App/shaders/*.vert"
)

# ?�버�?메시지 출력
message(STATUS "GLSL_SOURCE_FILES: ${GLSL_SOURCE_FILES}")

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME_WE)
  get_filename_component(FILE_EXT ${GLSL} EXT)
  set(SPIRV "${PROJECT_SOURCE_DIR}/Scop_App/shaders/${FILE_NAME}${FILE_EXT}.spv")

  # 디버그 메시지 출력
  message(STATUS "Compiling ${GLSL} to ${SPIRV}")
    
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL}
    COMMENT "Compiling ${GLSL} to ${SPIRV}"
  )
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

# ?�버�?메시지 출력
message(STATUS "SPIRV_BINARY_FILES: ${SPIRV_BINARY_FILES}")

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

# ?�용???�의 clean 명령 추�?
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/obj
)

message(STATUS "RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")