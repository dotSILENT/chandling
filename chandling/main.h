#pragma once
#include "raknet\RakClientInterface.h"
#include <string>

typedef bool(__thiscall*Send_t)(RakClientInterface* pRakClient, RakNet::BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel);
typedef Packet*(__thiscall*Receive_t)(RakClientInterface* pRakClient);
//typedef bool(__thiscall*HandleRPC_t)(void* pRakPeer, const char *data, int length, PlayerID playerId);
//typedef void(__thiscall*AddToChat_t)(void* pChatInput, unsigned color, const char* text);


//#define MO_CHAT_POINTER 0x21A0E4
typedef    unsigned char    BYTE;

namespace SAMP
{

	class samp
	{
	public:
		static Send_t pSend;
		static Receive_t pReceive;
		//static HandleRPC_t pHandleRPC;
		//static AddToChat_t pAddToChat;
		static RakClientInterface* ppRakClient;

		samp();
		~samp();

		void SetUp();

		static bool __thiscall Send(RakClientInterface* pRakClient, RakNet::BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel);
		static Packet* __thiscall Receive(RakClientInterface* pRakClient); //ignore __thiscall error
																		   //static bool __thiscall HandleRPC(void* pRakPeer, const char *data, int length, PlayerID playerId); //ignore __thiscall error
		//static int samp::HandleRPCPacket(const char *data, int length, PlayerID playerId);

		void InstallVTHooks(RakClientInterface* pRakClient);

		//void AddToChat(const char* text, int color);
		//void AddToChat(int color, const char* format, ...);
		//void AddToLog(char *fmt, ...);
		unsigned m_samp;

		void static AddToLog(char *fmt, ...);

	};

};
#pragma once
