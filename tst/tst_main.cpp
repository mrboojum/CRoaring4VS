/**
* The purpose of this test is to check that we can call CRoaringVS from C++ in Microsoft Visual Studio 2015 update 3 without clang.
*/

#include "../inc/roaring4vs.h"

#include <cassert>
#include <iostream>

using namespace roaring;

void test_example_cpp(bool copy_on_write) {
    // create a new empty bitmap
    Roaring4VS r1;
    r1.setCopyOnWrite(copy_on_write);
    // then we can add values
    for (uint32_t i = 100; i < 1000; i++) {
        r1.add(i);
    }

    // check whether a value is contained
    assert(r1.contains(500));

    // compute how many bits there are:
    uint64_t cardinality = r1.cardinality();
    std::cout << "Cardinality = " << cardinality << std::endl;

    // if your bitmaps have long runs, you can compress them by calling
    // run_optimize
    size_t size = r1.getSizeInBytes();
    r1.runOptimize();
    size_t compact_size = r1.getSizeInBytes();

    std::cout << "size before run optimize " << size << " bytes, and after "
        << compact_size << " bytes." << std::endl;

    // create a new bitmap with varargs
    Roaring4VS r2 = Roaring4VS::bitmapOf(5, 1, 2, 3, 5, 6);

    r2.printf();
    printf("\n");

    // we can also create a bitmap from a pointer to 32-bit integers
    const uint32_t values[] = { 2, 3, 4 };
    Roaring4VS r3(3, values);
    r3.setCopyOnWrite(copy_on_write);

    // we can also go in reverse and go from arrays to bitmaps
    uint64_t card1 = r1.cardinality();
    uint32_t *arr1 = new uint32_t[card1];
    assert(arr1 != NULL);
    r1.toUint32Array(arr1);
    Roaring4VS r1f(card1, arr1);
    delete[] arr1;

    // bitmaps shall be equal
    assert(r1 == r1f);

    // we can copy and compare bitmaps
    Roaring4VS z(r3);
    z.setCopyOnWrite(copy_on_write);
    assert(r3 == z);

    // we can compute union two-by-two
    Roaring4VS r1_2_3 = r1 | r2;
    r1_2_3.setCopyOnWrite(copy_on_write);
    r1_2_3 |= r3;

    // we can compute a big union
    const Roaring4VS *allmybitmaps[] = { &r1, &r2, &r3 };
    Roaring4VS bigunion = Roaring4VS::fastunion(3, allmybitmaps);
    assert(r1_2_3 == bigunion);

    // we can compute intersection two-by-two
    Roaring4VS i1_2 = r1 & r2;

    // we can write a bitmap to a pointer and recover it later

    size_t expectedsize = r1.getSizeInBytes();
    char *serializedbytes = new char[expectedsize];
    r1.write(serializedbytes);
    Roaring4VS t = Roaring4VS::read(serializedbytes);
    assert(r1 == t);
    delete[] serializedbytes;
}


int main(int ac, const char *const *av) {
    test_example_cpp(true);
    test_example_cpp(false);

    return EXIT_SUCCESS;
}