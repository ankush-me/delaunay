#ifndef __UTILS_MISC__
#define __UTILS_MISC__


/** Gives the positive modulus. */
template<typename V>
V mod(const V& a, const V& b);

/** Tests if i is in [s,e]%N. */
bool in_range(const int i, const int s, const int e, const int N);

#endif
