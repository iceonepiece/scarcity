#include "UniqueID.h"
#include <random>
#include <unordered_map>

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

UniqueID::UniqueID()
	: m_ID(s_uniformDistribution(s_engine))
{
}

UniqueID::UniqueID(uint64_t id)
	: m_ID(id)
{
}