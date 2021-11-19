#pragma once
#include "Message.h"

class Observer{
public:
	virtual ~Observer(){}
	virtual void Notify(const Message& msg) = 0;
};