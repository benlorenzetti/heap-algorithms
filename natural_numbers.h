#include <string>
#include <cassert>

/* Defines two complementary, portable unsigned integer classes,
 * whole numbers W = {0, 1, 2..., N-1} and natural numbers
 * N = {1, 2, 3..., N}, where N = 2^width.
 *
 * Both are represented in a datum of specified width in bits,
 * and the Natural representation is the 2's complement of the
 * usual Whole, unsigned integers. For width = 4:
 *
 *         (0)1001 =  Whole 9  =    + (8 + 1)
 *         (1)0111 = Natural 9 = 16 - (4 + 2 + 1)
*/
typedef unsigned int W;
class N;

N operator<<(N, W);
N operator>>(N, W);
N operator+(N, N);
N operator+(N, W);
N operator-(N, N);
N operator-(N, W);
N operator*(N, W);
N operator*(N, N);
bool operator<=(N, N);
bool operator<=(N, W);
bool operator<(N, N);
bool operator!=(N, N);
bool operator==(N, N);
N operator++(N&);
N operator--(N&);
W log2(N);
W log2(W);
W log10floor(W);

class N {
    public:
    W datum;
    W width;
    N();
    N(W x);
    void operator =(const N&);
    void set_width(W); // set max Natural number, N = 2^width
    W wmask() const;   // returns a wmask 00...111 for the sig. bits 
    W nmask() const;   // in N=2^width. (nmask = ~wmask)
    W toW() const;     // domain(toW) = [1...N-1] = range(toW)
    std::string print() const;
    W geometric_index() const;
    N null() const;
};

N N::null() const {
    N zero;
    zero.set_width(width);
    zero = 0;
    return zero;
}

void N::operator=(const N &n) {
    datum = n.datum;
    this->set_width(width);
}

W N::geometric_index() const {
    return (datum & this->wmask());
}

bool operator!=(N n1, N n2) {
    return (n1.datum != n2.datum);
}

bool operator==(N n1, N n2) {
    return (n1.datum == n2.datum);
}

N operator++(N &n) {
    n = n + 1;
    return n;
}

N operator--(N &n) {
    n = n-1;
    return n;
}

W log2(N n) {
    W power_of_2 = 0;
    W terminate = -1;
    while(n.datum != terminate) {
        n.datum = n.datum >> 1;
        power_of_2++;
        n.datum |= n.nmask() | (1 << (n.width - 1));
    }
    return power_of_2;
}

W log2(W w) {
    W power_of_2 = 0;
    while(w)
        w >>= 1, power_of_2++;
    return power_of_2;
}
W log10floor(W w) {
    W power_of_10 = 0;
    while(w) {
        w /= 10;
        ++power_of_10;
    }
    return power_of_10;
}


bool operator<(N n1, N n2) {
    if(n1.datum > n2.datum)
        return true;
    else
        return false;
}

bool operator<=(N n1, N n2) {
    if(n1.datum >= n2.datum)
        return true;
    else
        return false;
}

bool operator>=(N n1, N n2) {
    return (n1.datum <= n2.datum);
}

bool operator<=(N n1, W w2) {
    if(n1.toW() <= w2)
        return true;
    else
        return false;
}

N operator<<(N n, W w) {
    n.datum <<= w;
    n.datum |= n.nmask();
    return n;
}

N operator>>(N n, W w) {
    n.datum >>= w;
    n.width -= w;
    n.datum |= n.nmask();
    n.width += w;
    return n;
}

N operator+(N n1, N n2) {
    n1.datum += n2.datum;
    n1.datum |= n1.nmask();
    return n1;
}

N operator+(N n, W w) {
    n.datum -= w;
    n.datum |= n.nmask();
    return n;
}

N operator-(N n1, N n2) {
    return n1 + n2.datum;
}

N operator-(N n, W w) {
    N n2(w);
    return n - n2;
}

N operator*(N n, W w) {
    n.datum *= w;
    n.datum |= n.nmask();
    return n;
}

N operator*(N n1, N n2) {
    n1.datum *= n2.toW();
    n1.datum |= n1.nmask();
    return n1;
}

N::N() {
    width = 8 * sizeof(W);
}

N::N(W x) {
    width = 8 * sizeof(W);
    datum = -x;
}

std::ostream &operator<<(std::ostream &os, N const &n) {
    return os << n.print();
}

std::string N::print() const {
    if(!datum)
        return std::string("N");
    return std::to_string(toW());
}

void N::set_width(W new_width) {
    datum |= this->nmask();
    width = new_width;
}

W N::wmask() const {
    W mask;
    W rot_dist = width % (8*sizeof(W));
    if(rot_dist)
        mask = 1 << rot_dist;
    else
        mask = 0;
    return --mask;
}

W N::nmask() const {
    return ~(this->wmask());
}

W N::toW() const {
    assert(datum); // the value 2^8^sizeof(W) cannot fit in W
    W val;
    val = datum | this->nmask();;
    return -val;
}
