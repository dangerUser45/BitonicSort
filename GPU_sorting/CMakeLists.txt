find_package(OpenCL REQUIRED)

add_executable(gpu_driver
    src/driver.cpp
    src/utils.cpp
)

target_include_directories(gpu_driver PRIVATE include )

target_compile_definitions(gpu_driver PRIVATE
    CL_HPP_ENABLE_EXCEPTIONS
    CL_HPP_TARGET_OPENCL_VERSION=300
    CL_TARGET_OPENCL_VERSION=300

    GPU_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(gpu_driver PRIVATE
    OpenCL::OpenCL
    project_warnings
)
