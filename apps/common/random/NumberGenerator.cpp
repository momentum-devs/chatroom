#include "NumberGenerator.h"

namespace common::random
{
std::random_device NumberGenerator::randomDevice;

std::mt19937 NumberGenerator::pseudoRandomGenerator(NumberGenerator::randomDevice());
}
