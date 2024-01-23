#ifndef CPP_SOCKET_HPP
#define CPP_SOCKET_HPP

#include <string>
#include <vector>
#include <memory>
#include <climits>

#include "netstack.h"
#include "address.hpp"

namespace netstack
{
    /**
	 * @brief Possible socket types.
	 */
	enum class SocketType
	{
		STREAM = SOCK_STREAM,		///< Stream socket, typically used with TCP.
		DATAGRAM = SOCK_DGRAM,		///< Datagram socket, typically used with UDP.
		// RAW = SOCK_RAW,				///< Raw socket.
		// RDM = SOCK_RDM,				///< Reliably-delivered message socket.
		// SEQPACKET = SOCK_SEQPACKET,	///< Sequential packet socket.
	};

	/**
	 * @brief Possible socket protocol options.
	 */
	enum class SocketProtocol
	{
		// IP = IPPROTO_IP,
		// ICMP = IPPROTO_ICMP,	///< Internet Control Message Protocol.
		// IGMP = IPPROTO_IGMP,	///< Internet Group Management Protocol.
		// GGP = IPPROTO_GGP,
		TCP = IPPROTO_TCP,		///< Transmission Control Protocol.
		// PUP = IPPROTO_PUP,
		UDP = IPPROTO_UDP,		///< User Datagram Protocol.
		// IDP = IPPROTO_IDP,
		// ND = IPPROTO_ND,
		// RAW = IPPROTO_RAW,
		// MAX = IPPROTO_MAX
	};

	/**
	 * @brief Possible receive flags options.
	 */
	enum class ReceiveFlags
	{
		NONE = 0,					///< No flags, default behviour.
		OOB = MSG_OOB,				///< Process out-of-band data.
		PEEK = MSG_PEEK,			///< Peek at incoming message.
		WAITALL = 3,				///< Wait for full message to arrive.
	};

	/**
	 * @brief Possible send flags.
	 */
	enum class SendFlags
	{
		NONE = 0,					///< No flags, default behviour.
		DONTROUTE = MSG_DONTROUTE,	///< Send without using routing tables.
		OOB = MSG_OOB,				///< Send out-of-band data.
	};

	/**
	 * @brief Possible shutdown flags.
	 */
	enum class ShutdownFlags
	{
		RECEIVE = 0,	///< Shutdown the receive function of the socket.
		SEND = 1,		///< Shutdown the send function of the socket.
		BOTH = 2,		///< Shutdown both the send and receive functions of the socket.
	};
    
    /**
	 * @brief A Wrapper class for a Windows socket.
	 * 
	 * @extends Handler
	 */
	class Socket
	{
		friend class SocketOption;
	private:

	protected:
		SOCKET _socket;	///< SOCKET handle.

	public:
		Socket() = delete;

		/**
		 * @brief Creates a socket.
		 * 
		 * @param {AddressFamily} af - The address family to use for the socket (e.g. INET, INET6).
		 * @param {SocketType} type - The type of socket to create (e.g. STREAM, DATAGRAM).
		 * @param {SocketProtocol} protocol - The protocol to use with the socket (e.g. TCP, UDP).
		 */
		Socket(const AddressFamily af, const SocketType type, const SocketProtocol protocol) : Socket((int)af, (int)type, (int)protocol) {}

		/**
		 * @brief Creates a socket.
		 * 
		 * @param {int} af - The address family to use for the socket (e.g. AF_INET, AF_INET6).
		 * @param {int} type - The type of socket to create (e.g. SOCK_STREAM, SOCK_DGRAM).
		 * @param {int} protocol - The protocol to use with the socket (e.g. IPPROTO_TCP, IPPROTO_UDP).
		 */
		Socket(const int af, const int type, const int protocol)
		{
			_socket = socket(af, type, protocol);
		}

		/**
		 * @brief Creates a new socket using the specified SOCKET handle.
		 * 
		 * @param {SOCKET} socket - The SOCKET handle to use.
		 */
		Socket(const SOCKET socket)
		{
			_socket = socket;
		}

		/**
		* @brief Receives data from the socket and stores it in the specified buffer.
		* 
		* @param {std::string&} buffer - The buffer to store the received data.
		* @param {ReceiveFlags} flags - The flags to use to modifiy the operation. Defaults to NONE if not specified.
		* @return {int} The number of bytes received.
		*/
		int Receive(std::string& buffer, const ReceiveFlags flags = ReceiveFlags::NONE)
		{
			std::vector<char> what(CHAR_MAX);
			int received = 0;
			do
			{
				received = Receive(&what[0], what.size(), (int)flags);

				buffer.append(what.cbegin(), what.cend());
			} while (received == CHAR_MAX);

			return received;
		}

		/**
		 * @brief Receives data from the socket and stores it in the specified buffer.
		 * 
		 * @param {char*} buffer - The buffer to store the received data.
		 * @param {int} length - The length of the buffer.
		 * @param {int} flags - The flags to use to modifiy the operation. Defaults to 0 if not specified.
		 * @return {int} The number of bytes received.
		 */
		int Receive(char* buffer, const int length, const int flags = 0)
		{
			const int status = recv(_socket, buffer, length, flags);

			return status;
		}

		/**
		 * @brief Receives data from the socket and stores it in the specified buffer.
		 * 
		 * @param {char*} buffer - The buffer to store the received data.
		 * @param {int} length - The length of the buffer.
		 * @param {int} flags - The flags to use to modify the operation. Defaults to 0 if not specified.
		 * @param {sockaddr*} from - The pointer that contains the source address and port of the sender. Defaults to nullptr if not needed.
		 * @param {socklen_t*} fromLength - The pointer that contains the length of the sockaddr structure. Defaults to nullptr if not needed.
		 * @return {int} The number of bytes received.
		 */
		int ReceiveFrom(char* buffer, const size_t length, const int flags = 0, sockaddr* from = nullptr, socklen_t* fromLength = nullptr)
		{
			const int status = recvfrom(_socket, buffer, length, flags, from, fromLength);

			return status;
		}

		/**
		 * @brief Receives data from the socket and stores it in the specified buffer.
		 * 
		 * @param {std::string&} buffer - The buffer to store the received data.
		 * @param {ReceiveFlags} flags - The flags to use to modify the operation. Defaults to 0 if not specified.
		 * @param {Address&} fromAddress - The address that contains the source address and port of the sender. Defaults to an empty address.
		 * @return {int} The number of bytes received.
		 */
		int ReceiveFrom(std::string& buffer, const ReceiveFlags flags = ReceiveFlags::NONE, Address* fromAddress = nullptr)
		{
			std::unique_ptr<char[]> charBuffer = std::make_unique<char[]>(buffer.size());

			const int result = ReceiveFrom(charBuffer.get(), buffer.size(), (int)flags,  fromAddress == nullptr ? nullptr : fromAddress->name(), fromAddress == nullptr ? 0 : fromAddress->size());

			buffer.assign(charBuffer.get());

			return result;
		}

		/**
		 * @brief Sends the specified data over the socket.
		 * 
		 * @param {const char*} buffer - The buffer of data to send.
		 * @param {int} length - The length of the buffer.
		 * @param {int} flags - The flags to use for the send operation. Defaults to 0 if not specified.
		 * @throws {SocketSendException} - If an error occurs when sending the message.
		 * @return {int} The number of bytes sent.
		 */
		int Send(const char* buffer, const int length, const int flags = 0)
		{
			const int status = send(_socket, buffer, length, (int)flags);
			
			return status;
		}

		/**
		 * @brief Sends the specified data over the socket.
		 * 
		 * @param {const std::string&} buffer - The buffer of data to send.
		 * @param {SendFlags} flags - The flags to use for the send operation. Defaults to NONE if not specified.
		 * @return {int} The number of bytes sent.
		 */
		int Send(const std::string& buffer, const SendFlags flags = SendFlags::NONE)
		{
			return Send(buffer.c_str(), buffer.length(), (int)flags);
		}

		/**
		 * @brief Sends data from the buffer via the socket to an address. 
		 * 
		 * @param {const char*} buffer - The buffer of data to send.
		 * @param {int} length - The length of the buffer to be sent.
		 * @param {int} flags - Optional flags to be passed to the sendto function. Defaults to 0 if not specified.
		 * @param {sockaddr*} to - A pointer to a sockaddr structure containing the destination address.
		 * @param {int} toLength - The length of the destination address.
		 * @return {int} The number of bytes sent.
		*/
		int SendTo(const char* buffer, const int length, const int flags = 0, sockaddr* to = nullptr, socklen_t toLength = 0)
		{
			const int status = sendto(_socket, buffer, length, flags, to, toLength);

			return status;
		}

		/**
		 * @brief Sends data from the buffer via the socket to an address.
		 * 
		 * @param {const string&} buffer - The buffer of data to send.
		 * @param {SendFlags} flags - Optional flags to be passed to the sendto function. Defaults to NONE if not specified.
		 * @param {Address&} address - The address of the remote host.
		 * @return {int} The number of bytes sent.
		*/
		int SendTo(const std::string& buffer, const SendFlags flags = SendFlags::NONE, Address* address = nullptr)
		{
			return SendTo(buffer.c_str(), buffer.size(), (int)flags, address == nullptr? nullptr : address->name(), address == nullptr? 0 : address->size());
		}

		/**
		 * @brief Ends communication on this socket.
		 *
		 * @param {ShutdownFlags} flag - The flag indicating the direction in which to shut down the socket. Defaults to BOTH directions if not specified.
		 * @returns {bool} - .
		 */
		bool Shutdown(const ShutdownFlags flag = ShutdownFlags::BOTH)
		{
			const int status = shutdown(_socket, (int)flag);

            return status != SOCKET_ERROR;
		}

		/**
		 * @brief Destroys the instance of the socket by closing the underlying socket.
		 */
		~Socket()
		{
			nsCloseSocket(_socket);
		}
    };
} // namespace netstack

#endif // CPP_SOCKET_HPP