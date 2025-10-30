#ifndef __RANDOM__
#define __RANDOM__
#include <random>
#include <algorithm>

using namespace std;

class RNG {
private:
	unsigned int seed;
	std::mt19937 rng;
public:
	RNG(unsigned int seed);
	int rand();

	template <typename RNGIter>
	void shuffle(RNGIter first, RNGIter last) {
		std::shuffle(first, last, rng);
	}
};

#endif
