add_executable(test_netstack_c netstack_c.cpp)
target_compile_features(test_netstack_c PRIVATE cxx_std_17)
target_link_libraries(test_netstack_c PRIVATE netstack Catch2::Catch2WithMain)

add_test(NAME test-netstack_c COMMAND test_netstack_c)