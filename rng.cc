#include "rng.h"
#include <random>

RNG::RNG(unsigned int seed): seed{seed} {
	rng = std::mt19937{ seed };
	srand(seed);
}

int RNG::rand() {
	std::uniform_int_distribution<int> dist(0, RAND_MAX);
	return dist(rng);
}