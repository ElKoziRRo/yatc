//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "networkmessage.h"
#include "encryption.h"

#if defined(WIN32) || defined(_MSC_VER)

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>

#define socketret_t int
#define opt_t char
#define optlen_t int
#ifndef CONNECT_WOULD_BLOCK
#define CONNECT_WOULD_BLOCK WSAEWOULDBLOCK
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

#else

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>

extern int h_errno;

#define SOCKET int
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SD_BOTH SHUT_RDWR

#define socketret_t ssize_t
#define opt_t int
#define optlen_t socklen_t
#define CONNECT_WOULD_BLOCK EINPROGRESS
#define EWOULDBLOCK EAGAIN

#endif

#include <list>

class Connection;
class ProtocolGame;

#include "enum.h"
#include "protocolconfig.h"

#define RAISE_PROTOCOL_WARNING(message) \
setErrorDesc(message"."); \
Notifications::onProtocolError(false); \
return true;

#define RAISE_PROTOCOL_ERROR(message) \
setErrorDesc(message"."); \
Notifications::onProtocolError(true); \
return false;

#define MSG_READ_U8(name) \
uint8_t name; \
if(!msg.getU8(name)){ \
	RAISE_PROTOCOL_ERROR("Error reading U8: ""name") \
}

#define MSG_READ_U16(name) \
uint16_t name; \
if(!msg.getU16(name)){ \
	RAISE_PROTOCOL_ERROR("Error reading U16: ""name") \
}

#define MSG_INSPECT_U16(name) \
uint16_t name; \
if(!msg.inspectU16(name)){ \
	RAISE_PROTOCOL_ERROR("Error reading U16: ""name") \
}

#define MSG_READ_U32(name) \
uint32_t name; \
if(!msg.getU32(name)){ \
	RAISE_PROTOCOL_ERROR("Error reading U32: ""name") \
}

#define MSG_READ_STRING(name) \
std::string name; \
if(!msg.getString(name)){ \
	RAISE_PROTOCOL_ERROR("Error reading String: ""name") \
}

#define MSG_READ_POSITION(name) \
Position name; \
if(!msg.getPosition(name)){ \
	RAISE_PROTOCOL_ERROR("Error reading Position: ""name") \
}

class Protocol
{
	public:
		Protocol() : m_connection(NULL),m_currentMsg(NULL),m_currentMsgN(0) {}
		virtual ~Protocol() {}

		virtual void onConnect() = 0;
		virtual bool onRecv(NetworkMessage& msg) = 0;

		void setConnection(Connection* con) { m_connection = con; }

		std::string getErrorDesc(){ return getProtocolName() + std::string(": ") + m_errorMessage;}
		const std::list<uint8_t>& getLastServerCmd() { return m_lastServerCmd; }

		const NetworkMessage* getCurrentMsg(){ return m_currentMsg;}
		uint32_t getCurrentMsgN(){ return m_currentMsgN;}

		void usesAccountName(bool doUseAccName) { m_usesaccountname = doUseAccName; }
		bool doesUseAccountName() const { return m_usesaccountname; }

		virtual const char* getProtocolName() = 0;

	protected:
		void setErrorDesc(const std::string& message){ setErrorDesc(message.c_str());}
		void setErrorDesc(const char* message){ m_errorMessage = message;}
		void addServerCmd(uint8_t type);

		Connection* m_connection;
		NetworkMessage* m_currentMsg;
		uint32_t m_currentMsgN;

		bool m_usesaccountname;

	private:
		std::string m_errorMessage;
		std::list<uint8_t> m_lastServerCmd;
};

class Connection
{
	public:
		virtual ~Connection();

		enum STATE
		{
			STATE_INIT,
			STATE_CONNECTING,
			STATE_CONNECTED,
			STATE_CLOSED,
			STATE_ERROR
		};

		enum ConnectionError{
			ERROR_CANNOT_RESOLVE_HOST = 1,
			ERROR_WRONG_HOST_ADDR_TYPE,
			ERROR_CANNOT_CREATE_SOCKET,
			ERROR_CANNOT_SET_NOBLOCKING_SOCKET,
			ERROR_CANNOT_CONNECT,
			ERROR_CONNECT_TIMEOUT,
			ERROR_SELECT_FAIL_CONNECTED,
			ERROR_SELECT_FAIL_CONNECTING,
			ERROR_UNSUCCESSFUL_CONNECTION,
			ERROR_GETSOCKTOPT_FAIL,
			ERROR_UNEXPECTED_SELECT_RETURN_VALUE,
			ERROR_CANNOT_GET_PENDING_SIZE,
			ERROR_RECV_FAIL,
			ERROR_UNEXPECTED_RECV_ERROR,
			ERROR_DECRYPT_FAIL,
			ERROR_WRONG_MSG_SIZE,
			ERROR_SEND_FAIL,
			ERROR_PROTOCOL_ONRECV,
			ERROR_CLOSED_SOCKET,
			ERROR_TOO_BIG_MESSAGE
		};
		static const char* getErrorDesc(int message);

		virtual void executeNetwork();

		virtual void closeConnection();
		STATE getState(){ return m_state; }
		int getSocketError();

		virtual void sendMessage(NetworkMessage& msg);

		bool getChecksumState() const { return m_checksumEnable; }
		void setCryptoState(bool state){ m_cryptoEnable = state;}
	    void setChecksumState(bool state){ m_checksumEnable = state;}

		void setKey(char* key, int size){
			if(m_crypto){
				m_crypto->setKey(key, size);
			}
		}

		uint32_t getSent() {return m_sentBytes;}
		uint32_t getRecv() {return m_recvBytes;}
		uint32_t getTraffic() {return m_sentBytes+m_recvBytes;}


		Protocol* getProtocol(){ return m_protocol;}

	protected:
		Connection(const std::string& host, uint16_t port, Encryption* crypto, Protocol* protocol);
		friend class ProtocolConfig;

		//functions
		void callCallback(int error);
		virtual unsigned long getPendingInput();
		virtual int internalRead(unsigned int n, bool all);
		void closeConnectionError(int error);
		virtual void checkSocketReadState();

		//
		Encryption* m_crypto;
		Protocol* m_protocol;
		NetworkMessage m_inputMessage;

		//Remote host info
		std::string m_host;
		uint32_t m_ip;
		uint16_t m_port;

		//Traffic measurement
		uint32_t m_sentBytes;
		uint32_t m_recvBytes;

        //For recording a replay
		FILE* m_recordfile;
		uint32_t m_recordstart;

		//internal connection state
		bool m_cryptoEnable;
	    bool m_checksumEnable;

		enum READSTATE{
			READING_SIZE,
			READING_CHECKSUM,
			READING_MESSAGE
		};

		STATE m_state;
		READSTATE m_readState;
		uint16_t m_msgSize;

		//
		SOCKET m_socket;
		uint32_t m_ticks;
};

#endif
