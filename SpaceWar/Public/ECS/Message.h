#pragma once
using MessageType = unsigned int;
struct TwoFloats { float m_x; float m_y; };
struct FourFloats { float x; float y; float w; float h; };

// TDOD: Extend messages types
struct Message
{
	Message(const MessageType& Type) : 
		mType(Type)
	{}
	Message()
	{}

	MessageType mType;
	int mSender{-1};
	int mReceiver{-1};

	union
	{
		FourFloats m_4f;
		TwoFloats m_2f;
		float m_float;
		int m_int;
		bool m_bool;
	};
};