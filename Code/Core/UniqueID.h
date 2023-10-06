#pragma once

#include <cstdint>

class UniqueID
{
public:
	UniqueID();
	UniqueID(uint64_t id);

	operator uint64_t() const { return m_ID; }
private:
	uint64_t m_ID;
};