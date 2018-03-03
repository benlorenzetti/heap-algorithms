#include <stdlib.h>
#include <stdio.h>

int* rand_array(int, int); // size, seed
int* copy_array(const int*, int); // array, size
void print_array(const int*, int); // array, size
void save_array(const char*, int*, int); // filename, array, & size
int log2floor(int);
int log2ceil(int);
int rchild(int); // index i
int lchild(int);
int sibling(int);
int nparent(int, int); // index i, heap-width w
int wparent(int, int);
int b(int); // index i
int m(int); // size n
int h(const int, int); // heap depth d, index i
void swap(int*, int, int); // array, e1, e2
void max_heapify(int*, int*, int, int); // min pt, median point, index i, height h
void min_heapify(int*, int*, int, int); // med pt, max pt, index i, height h
int min_inject(const int, int*, int*, int, int);  // inject from max heap into min heap,
                                    // possibly swapping a smaller value into the max heap
                                    // heap depth d, med pt, max pt, index i, height h
                                    // return: -1 if swap occurs, 0 on termination
int max_inject(const int, int*, int*, int, int);

int main(int argc, char *argv[]) {
    // Initialize Test Array
    if(argc != 3) {
        printf("\tUsage:\n%s <array_size> <seed-num>\n", argv[0]);
        printf("\tE.g.\n%s 1000 123\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int array_size = atoi(argv[1]);
    int seed_number = atoi(argv[2]);
    int* array = rand_array(array_size, seed_number);    
    int* c1 = copy_array(array, array_size);
    
    int n = array_size;
    int w = b(-n); // number of bits needed to fully index heap
    int d = w - 1; // sign digit / whole heap depth
    int* max = array;
    int* med = array + m(n);
    int* min = array + n;   
    printf("min=%d, med=%d, max=%d\n", min, med, max);

    for(int i=(max-med); i; i++)
        min_heapify(med, max, i, h(d, i));

    for(int i=m(n)-1; i>=0; i--) {
        max_heapify(min, med, i, h(d, i));
        while(min_inject(d, med, max, i, h(d, i)))
            max_heapify(min, med, i, h(d, i));
    }

    save_array("unordered.csv", array, array_size);
    save_array("partordered.csv", array, array_size);    
    free(array);
}

int max_inject(const int d, int* min, int* med, int i, int height) {
    if(i >= 0) {
//        printf("max_inject() must inject from min heap.\n");
        exit(EXIT_FAILURE);
    }
    for(int l=0; l<=height; l++) {
        int parent = wparent(i, d-l);
//        printf("max_inject(%d, %d) looking at parent = %d\n", i, height, parent);
        if(min <= med + parent) {
//            printf("\tparent out of bounds, continue.\n");
            continue;
        }
        if(med[parent] > med[i]) {
//            printf("\tswaping med[%d]=%d <-> med[%d]=%d\n", parent, med[parent], i, med[i]);
            swap(med, parent, i);
            max_heapify(min, med, parent, h(d, parent));
            return -1;
        }
    }
    return 0;
}


int min_inject(const int d, int* med, int* max, int i, int height) {
    if(i < 0) {
//        printf("min_inject() must inject from max heap.\n");
        exit(EXIT_FAILURE);
    }
    for(int l=0; l<=height; l++) {
        int parent = nparent(i, d-l);
//        printf("min_inject(%d, %d) looking at parent = %d\n", i, height, parent);
        if(med + parent < max) {
//            printf("\tparent out of bounds, continue.\n");
            continue;
        }
        if(med[i] > med[parent]) {
//            printf("\tswaping med[%d]=%d <-> med[%d]=%d\n", i, med[i], parent, med[parent]);
            swap(med, i, parent);
            min_heapify(med, max, parent, h(d, parent));
            return -1;
        }
    }
    return 0;
}


void swap(int* array, int e1, int e2) {
    int tmp = array[e1];
    array[e1] = array[e2];
    array[e2] = tmp;
}

void min_heapify(int* med, int* max, int i, int h) {
    if(h == 0 || med+rchild(i) < max) {
        return;
    }
    if(max == med+lchild(i) || med[lchild(i)] <= med[rchild(i)]) {
        if(med[i] > med[lchild(i)]) {
            swap(med, lchild(i), i);
            min_heapify(med, max, lchild(i), --h);
        }
    }
    else {
        if(med[i] > med[rchild(i)]) {
            swap(med, rchild(i), i);
            min_heapify(med, max, rchild(i), --h);
        }
    }
}


void max_heapify(int* min, int* mheap, int i, int h) {
    if(h == 0 || min < mheap+lchild(i)) {
        return;
    }
    if(min == mheap+lchild(i) || mheap[rchild(i)] >= mheap[lchild(i)]) {
        if(mheap[rchild(i)] > mheap[i]) {
            swap(mheap, rchild(i), i);
            max_heapify(min, mheap, rchild(i), --h);
        }
    }
    else {
        if(mheap[lchild(i)] > mheap[i]) {
            swap(mheap, lchild(i), i);
            max_heapify(min, mheap, lchild(i), --h);
        }
    }
}

int b(int i) {
    if(!i)
        return 1;
    else if(i > 0)
        return log2floor(i);
    else
        return log2ceil(-i);
}

int m(int n) {
    if(n & 1)
        return n/2 + 1;
    return n/2;
}

int h(const int d, int i) {
    return d - b(i);
}

int rchild(int i) { return (i * 2); }
int lchild(int i) { return (i << 1) + 1; }
int sibling(int i) { return i ^ 1; }
int nparent(int i, int d) {
    int mask = 1 << d;
    mask = ~(mask-1);
    return mask | (i >> 1);
}
int wparent(int i, int d) {
    int mask = 1 << d;
    mask -= 1;
    return mask & (i >> 1);
}

int log2ceil(int size) {
    int height = 0;
    --size;
    do ++height;
    while(size >>= 1);
    return height;
}

int log2floor(int size) {
    int height = 0;
    do ++height;
    while(size >>= 1);
    return height;
}

void print_array(const int* array, int size) {
    printf("{\n");
    for(int i=size-1; i>=0; i--)
        printf("%2d, ", i-m(size));
    printf("}{\n");
    for(int i=size-1; i>=0; i--)
        printf("%2d, ", array[i]);
    printf("\n}\n");
}

void save_array(const char* filename, int* array, int size) {  
    FILE *outfile = fopen(filename, "w");
    for(int i=size-1; i; i--)
        fprintf(outfile, "%d\t%d\t\n", i-m(size), array[i]);
    fclose(outfile);
    printf("View results of %s with:\n", filename);
    printf("gnuplot> plot '%s' with boxes\n", filename);
}

int* copy_array(const int* array, int size) {
    int* copy = (int*) malloc(size * sizeof(int));
    for(int i=0; i<size; i++)
        copy[i] = array[i];
    return copy;
}

int* rand_array(int size, int seed) {
    int* array = (int*) malloc(size * sizeof(int));
    srand(seed);
    for(int i=0; i<size; i++)
            array[i] = rand() % (size/4);
    return array;
}


