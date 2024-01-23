#ifndef CPP_ADDRESS_HPP
#define CPP_ADDRESS_HPP

#include "netstack.h"

namespace netstack
{
	/**
	 * @brief Possible address families for a Socket or Address.
	 */
	enum class AddressFamily
	{
		// UNSPEC = AF_UNSPEC,			///< Unspecified.
		// UNIX = AF_UNIX,				///< local to host (pipes, portals).
		INET = AF_INET,				///< internetwork addresses also known as IPv4, typically used with TCP or UPD protocalls.
		// IMPLINK = AF_IMPLINK,		///< 
		// PUP = AF_PUP,				///< 
		// CHAOS = AF_CHAOS,			///< 
		// IPX = AF_IPX,				///< IPX address family.
		// NS = AF_NS,					///< 
		// ISO = AF_ISO,				///< 
		// OSI = AF_OSI,				///< 
		// ECMA = AF_ECMA,				///< 
		// DATAKIT = AF_DATAKIT,		///< 
		// CCITT = AF_CCITT,			///< 
		// SNA = AF_SNA,				///< 
		// DECnet = AF_DECnet,			///< 
		// DLI = AF_DLI,				///< 
		// LAT = AF_LAT,				///< 
		// HYLINK = AF_HYLINK,			///< 
		// APPLETALK = AF_APPLETALK,	///< AppleTalk.
		// NETBIOS = AF_NETBIOS,		///< NetBIOS address family.
		// VOICEVIEW = AF_VOICEVIEW,	///< 
		// FIREFOX = AF_FIREFOX,		///< 
		// UNKNOWN1 = AF_UNKNOWN1,		///< 
		// BAN = AF_BAN,				///< 
		// ATM = AF_ATM,				///< 
		INET6 = AF_INET6,			///< internetwork addresses also known as IPv6, typically used with TCP or UPD protocalls.
		// CLUSTER = AF_CLUSTER,		///< 
		// IRDA = AF_IRDA,				///< 
		// NETDES = AF_NETDES,			///< 
		// MAX = AF_MAX				///< 
	};

	/**
	 * @brief An encapsulation for a socket address.
	 * 
	 * Can store either INET (IPv4) or INET6 (IPv6) addresses.
	 */
	class Address 
	{
	private:
        bool state_;
		sockaddr_storage address_;	///< The socket address.
		socklen_t length_;			///< The length of the socket address.

	public:
		/**
		 * @brief Constructs an address. Which represents the destination or source of a socket.
		 *
		 * @param {const AddressFamily} family - The address family to use for the socket (e.g. INET, INET6).
		 * @param {const char*} ip - The IP address to use (e.g. "127.0.0.1")
		 * @param {const unsigned short} port - The port number to use (e.g 8080, 3000)
		 */
		Address(const AddressFamily family, const char* ip, const unsigned short port) : state_(true)
		{
			address_ = {};
			sockaddr* sockAddr = (sockaddr*)&address_;
			sockAddr->sa_family = (int)family;

			switch (family)
			{
			case AddressFamily::INET:
			{
				sockaddr_in* sin = (sockaddr_in*)sockAddr;
				const int status = inet_pton(sockAddr->sa_family, ip, &sin->sin_addr);
				
				if (status == 0 || status == -1) {
                    state_ = false;
                    break;
                }
				
				sin->sin_port = htons(port);
				break;
			}

			case AddressFamily::INET6:
			{
				sockaddr_in6* sin6 = (struct sockaddr_in6*)sockAddr;
				const int status =  inet_pton(sockAddr->sa_family, ip, &sin6->sin6_addr);
				
				if (status == 0 || status == -1) {
                    state_ = false;
                    break;
                }

				sin6->sin6_port = htons(port);
				break;
			}

			default:
				state_ = false;
				break;
			}

			length_ = sizeof(address_);
		}

		/**
		 * @brief Constructs an empty address object, use the parameterized constructor to make useable addresses.
		 */
		Address()
		{
			address_ = {};
			length_ = {};
		}

        operator bool() const 
        {
            return state_;
        }

		/**
		 * @brief Converts the Address object to a sockaddr pointer.
		 *
		 * @return {sockaddr*} A sockaddr pointer.
		 */
		operator sockaddr* () const
		{
			return name();
		}

		/**
		 * @brief Returns a pointer to the socket address.
		 *
		 * @return {sockaddr*} A sockaddr pointer.
		 */
		sockaddr* name() const
		{
			return (struct sockaddr*)&address_;
		}

		/**
		 * @brief Converts the Address object to a pointer to an socklen_t representing the size of the socket address.
		 *
		 * @return {socklen_t*} A pointer to an socklen_t representing the size of the socket address.
		 */
		operator socklen_t* ()
		{
			return &length_;
		}
		
		/**
		 * @brief Returns a pointer to an socklen_t representing the size of the socket address.
		 *
		 * @return {socklen_t*} A pointer to an socklen_t representing the size of the socket address.
		 */
		socklen_t* ptr()
		{
			return &length_;
		}

		/**
		 * @brief Converts the Address object to an int representing the size of the socket address.
		 *
		 * @return {int} The size of the socket address.
		 */
		operator socklen_t() const
		{
			return size();
		}

		/**
		 * @brief Returns the size of the socket address.
		 *
		 * @return {socklen_t} The size of the socket address.
		 */
		socklen_t size() const
		{
			return length_;
		}

		// TODO
		void GetAddressInfo()
		{
			// getaddrinfo();
		}
	};
} // namespace netstack

#endif // CPP_ADDRESS_HPP