#pragma once
#include <stdint.h>

using Bitset = uint32_t;
class Bitmask {
public:
	Bitmask() : bits(0) {}
	Bitmask(const Bitset& Bits) : bits(Bits) {}

	Bitset GetMask() const { return bits; }
	void SetMask(const Bitset& value) { bits = value; }

	bool Matches(const Bitmask& Bits,
		const Bitset& Relevant = 0)const
	{
		return(Relevant ?
			((Bits.GetMask() & Relevant) == (bits & Relevant)) :
			(Bits.GetMask() == bits));
	}

	bool test(const unsigned int& pos)const {
		return ((bits & (1 << pos)) != 0);
	}
	void set(const unsigned int& pos) {
		bits |= 1 << pos;
	}
	void TurnOnBits(const Bitset& Bits) {
		bits |= Bits;
	}
	void ClearBit(const unsigned int& pos) {
		bits &= ~(1 << pos);
	}
	void ToggleBit(const unsigned int& pos) {
		bits ^= 1 << pos;
	}
	void reset() { bits = 0; }

private:

	Bitset bits;
};