add_executable(basic basic.cpp)

target_link_libraries(basic PRIVATE netstack)

target_compile_features(basic PRIVATE cxx_std_17)