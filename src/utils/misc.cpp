
#include "misc.h"

/** Gives the positive modulus. */
template<typename V>
V mod(const V& a, const V& b) {
    return (a % b + b) % b;
}
template int mod(const int & a, const int & b);


/** Tests if i is in [s,e]%N. */
bool in_range(const int i, const int s, const int e, const int N) {
	if (s == e) {return i==s;}
	if (s<e) {return (s<=i && i<=e);}
	else if (s > e) {return ((s<=i && i< N)||(0<=i && i<= e));}
}

