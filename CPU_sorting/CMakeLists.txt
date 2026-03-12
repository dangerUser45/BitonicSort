add_executable(cpu_driver
    src/driver.cpp
)

target_include_directories(cpu_driver PRIVATE include )
target_link_libraries(cpu_driver PRIVATE project_warnings)
