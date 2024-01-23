#include <catch2/catch_test_macros.hpp>
#include "netstack.h"

TEST_CASE("Initialize and cleanup networking stack", "[nsSetup][nsCleanup]") {
    // Test nsSetup and nsCleanup on Windows
    REQUIRE(nsSetup() == 0); // Check if initialization is successful
    nsCleanup(); // Clean up resources
    // Add more checks if needed for successful cleanup
}

TEST_CASE("Check if a socket is valid", "[nsIsValidSocket]") {
    // Test nsIsValidSocket on Windows
    SECTION("Valid socket") {
        SOCKET validSocket = socket(AF_INET, SOCK_STREAM, 0);
        REQUIRE(nsIsValidSocket(validSocket) == true);
        nsCloseSocket(validSocket);
    }

    SECTION("Invalid socket") {
        SOCKET invalidSocket = SOCKET_ERROR;
        REQUIRE(nsIsValidSocket(invalidSocket) == false);
    }
}

TEST_CASE("Get the last socket error code", "[nsSocketError]") {
    // Test nsSocketError on Windows
    // Assuming an error occurred before running the test
    socket(0, 0, 0);

    REQUIRE(nsSocketError() != 0);
}