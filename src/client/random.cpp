#include "stdafx.h"
#include "random.h"

namespace Global
{
Random::Random()
	: _engine(_device())
{

}

Random::~Random()
{
}

bool Random::Probability(const float num)
{
	return (num > 1.f ? true : (num < 0.f ? false : std::bernoulli_distribution(num)(_engine)));
}
}