#include "natural_numbers.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> random_vector(int, int); // size, seed
void print_vector(vector<int> &);
void save_vector(const vector<int> &, const char* file);
bool gt(int a, int b) { return (a > b); }
bool gteq(int a, int b) { return (a >= b); }
bool lt(int a, int b) { return (a < b); }
bool lteq(int a, int b) { return (a <= b); }
bool eq(int a, int b) { return (a == b); }
void heapify(vector<int> &, N, N, N, bool (*)(int, int));
void heapify(vector<int> &, N, N, W, bool (*)(int, int));
void build_heap(vector<int> &, N, N, N, bool (*)(int, int));
void swap(vector<int> &, N, N);
void swap(vector<int> &, W, W);
int count(const vector<int> &, N, N, bool (*)(int, int), int);
// vector v, range (N, N], relation fn, compare value
void winnow_heap(vector<int> &, N, N, N, bool (*)(int, int));


int main() {
   

    vector<int> v, c, s;
while(1) {
    cout << "vector power: ";
    int power, seed;
    cin >> power;
    cout << "  seed value: ";
    cin >> seed;

    s = v = random_vector(1 << power, seed);

    N n = v.size();
    N q1, m, q3;
    m = n >> 1;
    q1 = m >> 1;
    q3 = m + ((n-m) >> 1);
    cout << "(0, " << q1 << ", " << m << ", " << q3 << ", ";
    cout << n << "]\n";

    sort(s.begin(), s.end(), gt);
    int med = s[s.size()/2];
    cout << "median = " << med << endl;

    build_heap(v, n.null(), m, n, lt);
    int lt_in_middle = count(v, q1, q3, lt, med);
    int gt_in_middle = count(v, q1, q3, gt, med);
    int eq_in_middle = count(v, q1, q3, eq, med);
    cout << "eq in (q1, q3]: " << eq_in_middle << endl;
    cout << "lt in (q1, q3]: " << lt_in_middle << endl;
    cout << "gt in (q1, q3]: " << gt_in_middle << endl;

    winnow_heap(v, n.null(), m, n, lt);
    int winnow_med = v[s.size()/2];
    cout << "Estimated median: " << winnow_med << endl;
    int lt_est = count(v, n.null(), n, lt, winnow_med);
    int eq_est = count(v, n.null(), n, eq, winnow_med);
    int gt_est = count(v, n.null(), n, gt, winnow_med);
    cout << "lt est: " << lt_est << "\neq est: " << eq_est;
    cout << "\ngt est: " << gt_est << endl;
    
    save_vector(v, "partial.csv");
}
}

void winnow_heap(vector<int> &v, N a, N m, N b, bool (*rel)(int, int)) {
//cout << "winnow_heap(" << a << ", " << m << ", " << b << ")\n";
    static int winnow_count;
    winnow_count++;
    N n = v.size();
    a.set_width(log2(n));
    b.set_width(log2(n));
    m.set_width(log2(n));
    N q1, q3;
    build_heap(v, a, m, b, rel); 
    char fname[100] = "XXpartial.csv";
    fname[0] = '0'+winnow_count / 10;
    fname[1] = '0'+winnow_count % 10;
    save_vector(v, fname);
    if((b-a) <= 4)
        return;
    q1 = a + ((m - a) >> 1);
    q3 = m + ((b-m) >> 1);
    winnow_heap(v, q1, m, q3, rel);
}

int count(const vector<int> &v, N a, N b, bool (*rel)(int, int), int med) {
    int count = 0;
    N n = v.size();
    a.set_width(log2(n));
    b.set_width(log2(n));
    while(a != b) {
        ++a;
        if(rel(v[a.geometric_index()], med))
            count++;
    }
    return count;
}

void build_heap(vector<int> &v, N a, N m, N b, bool(*rel)(int, int)) {
//cout << "build_heap(" << a << ", " << m << ", " << b << ")\n";
    N n = v.size();
    a.set_width(log2(n));
    b.set_width(log2(n));
    N i, j;
    n = b - a;
    i.set_width(log2(n));
    j.set_width(log2(n));
    i = n >> 1;
    j = i + 1;
//cout << "n=" << n << ", i=" << i << ", j=" << j << endl;
    while(i != i.null()) {
//cout << "i=" << i << ", j=" << j << endl;
        heapify(v, a, b, i, rel);
        heapify(v, a, b, j.geometric_index(), rel);
        --i, ++j;
    }
}

void heapify(vector<int> &v, N a, N b, W i, bool (*rel)(int, int)) {
//cout << "W_heapify((" << a << ", " << b << "], " << i << ")\n";
    N n = v.size();
    a.set_width(log2(n));
    b.set_width(log2(n));
    W base = b.geometric_index();
    W end = (a == n ? n.toW() : a.geometric_index());
    W rchild = i << 1;
    W lchild = rchild + 1;
//cout << "base="<< base << ", i=" << i << ", rchild=" << rchild;
//cout << ", lchild=" << lchild << ", end=" << end << endl;
    if(end > lchild+base) {
        if(rel(v[lchild+base], v[rchild+base])) {
            if(rel(v[i+base], v[rchild+base])) {
                swap(v, i+base, rchild+base);
                heapify(v, a, b, rchild, rel);
            }
        }
        else {
            if(rel(v[i+base], v[lchild+base])) {
                swap(v, base+i, base+lchild);
                heapify(v, a, b, lchild, rel);
            }
        }
    }
    else if(end == lchild+base) {
        if(rel(v[i+base], v[rchild+base])) {
            swap(v, base+i, base+rchild);
        }
    }
}

void heapify(vector<int> &v, N a, N b, N i, bool (*rel)(int, int)) {
    N n = v.size();
    a.set_width(log2(n));
    b.set_width(log2(n));
    n = b-a;
    N rchild, lchild;
    i.set_width(log2(n));
    rchild.set_width(log2(n));
    lchild.set_width(log2(n));
    rchild = i << 1;
    lchild = rchild - 1;
    if(i < rchild) {
        if(rel(v[(a+rchild).geometric_index()], v[(a+lchild).geometric_index()])) {
            if(rel(v[(a+rchild).geometric_index()], v[(a+i).geometric_index()])) {
                swap(v, a+rchild, a+i);
                heapify(v, a, b, rchild, rel);
            }
        }
        else {
            if(rel(v[(a+lchild).geometric_index()], v[(a+i).geometric_index()])) {
                swap(v, a+lchild, a+i);     
                heapify(v, a, b, lchild, rel);  
            }
        }
    }
    else if(i < lchild) {
        if(rel(v[(a+lchild).geometric_index()], v[(a+i).geometric_index()])) {
            swap(v, a+lchild, a+i);
        }
    }
}

void swap(vector<int> &v, N a, N b) {
    int tmp = v[a.geometric_index()];
    v[a.geometric_index()] = v[b.geometric_index()];
    v[b.geometric_index()] = tmp;
}

void swap(vector<int> &v, W a, W b) {
    int tmp = v[a];
    v[a] = v[b];
    v[b] = tmp;
}

vector<int> random_vector(int size, int seed) {
    srand(seed);
    vector<int> v;
    for(int i=1; i<=size; i++)
        v.push_back(rand() % (size / 2));
    return move(v);
}

void print_vector(vector<int> &v) {
    cout << "{";
    N n = v.size();
    N i = 0;
    i.set_width(log2(n));
    do ++i, cout << setw(log10floor(v.size())) << i << ", ";
    while(i != n);
    cout << "}\n{";
    i = N(0);
    do {
        ++i;
        cout << setw(log10floor(v.size()));
        cout << v[i.geometric_index()] << ", ";
    } while(i != n);
    cout << "}\n{";
    i = N(0);
    do {
        ++i;
        cout << setw(log10floor(v.size())) << i.null()-i << ", ";
    } while(i != n);
    cout << "}\n";
}

void save_vector(const vector<int> &v, const char *filename) {
    FILE* fp = fopen(filename, "w");
    for(int i=v.size()-1; i>=0; i--)
        fprintf(fp, "%d\t%d\n", v.size()-i, v[i]);
    fclose(fp);
}
