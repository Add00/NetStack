#ifndef C_NETSTACK_H
#define C_NETSTACK_H

#include <stdbool.h>

// Prevents name mangling when compiled in c++
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_WIN32)
    // Required Windows libraries
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

#else
    // Required POSIX/Unix libraries
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>
#endif

#ifndef SOCKET 
    typedef int SOCKET;
#endif

// Linkage type for library functions (can be extern, static, or inline).
#define NS_LINKAGE extern

#if defined (NS_MACRO_MODE)
    #if defined(_WIN32)
        #define nsIsValidSocket(socket) ((socket) != INVALID_SOCKET)
        #define nsCloseSocket(socket) closesocket(socket)
        #define nsSocketError() (WSAGetLastError())
    #else
        #define nsIsValidSocket(socket) ((socket) >= 0)
        #define nsCloseSocket(socket) close(socket)
        #define nsSocketError() (errno)
    #endif

#else
    /// @brief Initialize the networking stack (Windows-specific).
    /// This function initializes the networking stack on Windows using WSAStartup.
    /// It should be called before any other networking functions on Windows.
    /// 
    /// @return 0 on success, or an error code indicating the failure.
    NS_LINKAGE int nsSetup()
    {
        #if defined(_WIN32)
            WSADATA d;
            return WSAStartup(MAKEWORD(2, 2), &d);
        #endif

        return 0;
    }

    /// @brief Free resources used by the networking stack (Windows-specific).
    /// This function cleans up resources used by the networking stack on Windows.
    /// It should be called when networking operations are no longer needed.
    NS_LINKAGE void nsCleanup()
    {
        #if defined(_WIN32)
            WSACleanup();
        #endif
    }

    /// @brief Check if a socket is valid.
    /// This function checks if the given socket is valid for further use.
    /// 
    /// @param socket The socket to check.
    /// @return true if the socket is valid, false otherwise.
    NS_LINKAGE bool nsIsValidSocket(SOCKET socket)
    {       
        #if defined(_WIN32)
            return socket != INVALID_SOCKET;
        #else
            return socket >= 0;
        #endif
    }

    /// @brief Close a socket.
    /// This function closes the given socket, releasing any resources associated with it.
    /// 
    /// @param socket The socket to close.
    NS_LINKAGE void nsCloseSocket(SOCKET socket)
    {
        #if defined(_WIN32)
            closesocket(socket);
        #else
            close(socket);
        #endif
    }


    /// @brief Get the last socket error code.
    /// This function retrieves the last socket error code, which can be used for error handling.
    /// On Windows, it uses WSAGetLastError(); on non-Windows systems, it uses errno.
    /// 
    /// @return The last socket error code.
    NS_LINKAGE int nsSocketError()
    {
        #if defined(_WIN32)
            return WSAGetLastError();
        #else
            return errno;
        #endif
    }
#endif // C_NETSTACK_MACRO_MODE

#if defined(__cplusplus)
}
#endif

#endif // C_NETSTACK_H