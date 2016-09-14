#include "../inc/roaring4vs.h"
#include "croaring/roaring.h"

#include <stdarg.h>

#include <algorithm>
#include <new>
#include <stdexcept>

using namespace roaring;

/**
* Create an empty bitmap
*/
Roaring4VS::Roaring4VS() : roaring(NULL) {
    roaring = roaring_bitmap_create();
    if (roaring == NULL) {
        throw std::runtime_error("failed memory alloc in constructor");
    }
}

/**
* Construct a bitmap from a list of integer values.
*/
Roaring4VS::Roaring4VS(size_t n, const uint32_t *data) {
    roaring = roaring_bitmap_of_ptr(n, data);
    if (roaring == NULL) {
        throw std::runtime_error("failed memory alloc in constructor");
    }
}

/**
* Copy constructor
*/
Roaring4VS::Roaring4VS(const Roaring4VS &r) : roaring(NULL) {
    roaring = roaring_bitmap_copy((roaring_bitmap_t*)r.roaring);
    if (roaring == NULL) {
        throw std::runtime_error("failed memory alloc in constructor");
    }
}

/**
* Construct a roaring object from the C struct.
*
* Passing a NULL point is unsafe.
*/
Roaring4VS::Roaring4VS(void *s) : roaring(s) {}

/**
* Construct a bitmap from a list of integer values.
*/
Roaring4VS Roaring4VS::bitmapOf(size_t n, ...) {
    Roaring4VS ans;
    va_list vl;
    va_start(vl, n);
    for (size_t i = 0; i < n; i++) {
        ans.add(va_arg(vl, uint32_t));
    }
    va_end(vl);
    return ans;
}


/**
* Add value x
*
*/
void Roaring4VS::add(uint32_t x) { roaring_bitmap_add((roaring_bitmap_t*)roaring, x); }

/**
* Add value n_args from pointer vals
*
*/
void Roaring4VS::addMany(size_t n_args, const uint32_t *vals) {
    roaring_bitmap_add_many((roaring_bitmap_t*)roaring, n_args, vals);
}

/**
* Remove value x
*
*/
void Roaring4VS::remove(uint32_t x) { roaring_bitmap_remove((roaring_bitmap_t*)roaring, x); }

/**
* Check if value x is present
*/
bool Roaring4VS::contains(uint32_t x) const {
    return roaring_bitmap_contains((roaring_bitmap_t*)roaring, x);
}

/**
* Destructor
*/
Roaring4VS::~Roaring4VS() { roaring_bitmap_free((roaring_bitmap_t*)roaring); }

/**
* Copies the content of the provided bitmap, and
* discard the current content.
*/
Roaring4VS &Roaring4VS::operator=(const Roaring4VS &r) {
    roaring_bitmap_free((roaring_bitmap_t*)roaring);
    roaring = roaring_bitmap_copy((roaring_bitmap_t*)roaring);
    if (roaring == NULL) {
        throw std::runtime_error("failed memory alloc in assignement");
    }
    return *this;
}

/**
* Compute the intersection between the current bitmap and the provided
* bitmap,
* writing the result in the current bitmap. The provided bitmap is not
* modified.
*/
Roaring4VS &Roaring4VS::operator&=(const Roaring4VS &r) {
    roaring_bitmap_and_inplace((roaring_bitmap_t*)roaring, (roaring_bitmap_t*)r.roaring);
    return *this;
}

/**
* Compute the union between the current bitmap and the provided bitmap,
* writing the result in the current bitmap. The provided bitmap is not
* modified.
*
* See also the fastunion function to aggregate many bitmaps more quickly.
*/
Roaring4VS &Roaring4VS::operator|=(const Roaring4VS &r) {
    roaring_bitmap_or_inplace((roaring_bitmap_t*)roaring, (roaring_bitmap_t*)r.roaring);
    return *this;
}

/**
* Compute the symmetric union between the current bitmap and the provided
* bitmap,
* writing the result in the current bitmap. The provided bitmap is not
* modified.
*/
Roaring4VS &Roaring4VS::operator^=(const Roaring4VS &r) {
    roaring_bitmap_xor_inplace((roaring_bitmap_t*)roaring, (roaring_bitmap_t*)r.roaring);
    return *this;
}

/**
* Exchange the content of this bitmap with another.
*/
void Roaring4VS::swap(Roaring4VS &r) { std::swap(r.roaring, roaring); }

/**
* Get the cardinality of the bitmap (number of elements).
*/
uint64_t Roaring4VS::cardinality() const {
    return roaring_bitmap_get_cardinality((roaring_bitmap_t*)roaring);
}

/**
* Returns true if the bitmap is empty (cardinality is zero).
*/
bool Roaring4VS::isEmpty() const { return roaring_bitmap_is_empty((roaring_bitmap_t*)roaring); }

/**
* Convert the bitmap to an array. Write the output to "ans",
* caller is responsible to ensure that there is enough memory
* allocated
* (e.g., ans = new uint32[mybitmap.cardinality()];)
*/
void Roaring4VS::toUint32Array(uint32_t *ans) const {
    roaring_bitmap_to_uint32_array((roaring_bitmap_t*)roaring, ans);
}

/**
* Return true if the two bitmaps contain the same elements.
*/
bool Roaring4VS::operator==(const Roaring4VS &r) const {
    return roaring_bitmap_equals((roaring_bitmap_t*)roaring, (roaring_bitmap_t*)r.roaring);
}

/**
* compute the negation of the roaring bitmap within a specified interval.
* areas outside the range are passed through unchanged.
*/
void Roaring4VS::flip(uint64_t range_start, uint64_t range_end) {
    roaring_bitmap_flip_inplace((roaring_bitmap_t*)roaring, range_start, range_end);
}

/**
*  Remove run-length encoding even when it is more space efficient
*  return whether a change was applied
*/
bool Roaring4VS::removeRunCompression() {
    return roaring_bitmap_remove_run_compression((roaring_bitmap_t*)roaring);
}

/** convert array and bitmap containers to run containers when it is more
* efficient;
* also convert from run containers when more space efficient.  Returns
* true if the result has at least one run container.
*/
bool Roaring4VS::runOptimize() { return roaring_bitmap_run_optimize((roaring_bitmap_t*)roaring); }

/**
* Iterate over the bitmap elements. The function iterator is called once
* for
*  all the values with ptr (can be NULL) as the second parameter of each
* call.
*
*  roaring_iterator is simply a pointer to a function that returns void,
*  and takes (uint32_t,void*) as inputs.
*/
void Roaring4VS::iterate(RoaringIterator iterator, void *ptr) const {
    roaring_iterate((roaring_bitmap_t*)roaring, iterator, ptr);
}

/**
* If the size of the roaring bitmap is strictly greater than rank, then
this
function returns true and set element to the element of given rank.
Otherwise, it returns false.
*/
bool Roaring4VS::select(uint32_t rank, uint32_t *element) const {
    return roaring_bitmap_select((roaring_bitmap_t*)roaring, rank, element);
}

/**
* write a bitmap to a char buffer. This is meant to be compatible with
* the
* Java and Go versions. Returns how many bytes were written which should be
* getSizeInBytes().
*
* Setting the portable flag to false enable a custom format that
* can save space compared to the portable format (e.g., for very
* sparse bitmaps).
*/
size_t Roaring4VS::write(char *buf, bool portable) const {
    if (portable)
        return roaring_bitmap_portable_serialize((roaring_bitmap_t*)roaring, buf);
    else
        return roaring_bitmap_serialize((roaring_bitmap_t*)roaring, buf);
}

/**
* read a bitmap from a serialized version. This is meant to be compatible
* with
* the
* Java and Go versions.
*
* Setting the portable flag to false enable a custom format that
* can save space compared to the portable format (e.g., for very
* sparse bitmaps).
*/
Roaring4VS Roaring4VS::read(const char *buf, bool portable) {
    Roaring4VS ans(NULL);
    if (portable)
        ans.roaring = roaring_bitmap_portable_deserialize(buf);
    else
        ans.roaring = roaring_bitmap_deserialize(buf);
    if (ans.roaring == NULL) {
        throw std::runtime_error("failed memory alloc while reading");
    }
    return ans;
}

/**
* How many bytes are required to serialize this bitmap (meant to be
* compatible
* with Java and Go versions)
*
* Setting the portable flag to false enable a custom format that
* can save space compared to the portable format (e.g., for very
* sparse bitmaps).
*/
size_t Roaring4VS::getSizeInBytes(bool portable) const {
    if (portable)
        return roaring_bitmap_portable_size_in_bytes((roaring_bitmap_t*)roaring);
    else
        return roaring_bitmap_size_in_bytes((roaring_bitmap_t*)roaring);
}

/**
* Computes the intersection between two bitmaps and returns new bitmap.
* The current bitmap and the provided bitmap are unchanged.
*/
Roaring4VS Roaring4VS::operator&(const Roaring4VS &o) const {
    roaring_bitmap_t *r = roaring_bitmap_and((roaring_bitmap_t*)roaring, (roaring_bitmap_t*)o.roaring);
    if (r == NULL) {
        throw std::runtime_error("failed materalization in and");
    }
    return Roaring4VS(r);
}

/**
* Computes the union between two bitmaps and returns new bitmap.
* The current bitmap and the provided bitmap are unchanged.
*/
Roaring4VS Roaring4VS::operator|(const Roaring4VS &o) const {
    roaring_bitmap_t *r = roaring_bitmap_or((roaring_bitmap_t*)roaring, (roaring_bitmap_t*)o.roaring);
    if (r == NULL) {
        throw std::runtime_error("failed materalization in or");
    }
    return Roaring4VS(r);
}

/**
* Computes the symmetric union between two bitmaps and returns new bitmap.
* The current bitmap and the provided bitmap are unchanged.
*/
Roaring4VS Roaring4VS::operator^(const Roaring4VS &o) const {
    roaring_bitmap_t *r = roaring_bitmap_xor((roaring_bitmap_t*)roaring, (roaring_bitmap_t*)o.roaring);
    if (r == NULL) {
        throw std::runtime_error("failed materalization in xor");
    }
    return Roaring4VS(r);
}

/**
* Whether or not we apply copy and write.
*/
void Roaring4VS::setCopyOnWrite(bool val) { ((roaring_bitmap_t*)roaring)->copy_on_write = val; }

/**
* Print the content of the bitmap
*/
void Roaring4VS::printf() { roaring_bitmap_printf((roaring_bitmap_t*)roaring); }

/**
* Whether or not copy and write is active.
*/
bool Roaring4VS::getCopyOnWrite() const { return ((roaring_bitmap_t*)roaring)->copy_on_write; }

/**
* computes the logical or (union) between "n" bitmaps (referenced by a
* pointer).
*/
Roaring4VS Roaring4VS::fastunion(size_t n, const Roaring4VS **inputs) {
    const roaring_bitmap_t **x =
        (const roaring_bitmap_t **)malloc(n * sizeof(roaring_bitmap_t *));
    if (x == NULL) {
        throw std::runtime_error("failed memory alloc in fastunion");
    }
    for (size_t k = 0; k < n; ++k) x[k] = (roaring_bitmap_t*)inputs[k]->roaring;

    Roaring4VS ans(NULL);
    ans.roaring = roaring_bitmap_or_many(n, x);
    if (ans.roaring == NULL) {
        throw std::runtime_error("failed memory alloc in fastunion");
    }
    free(x);
    return ans;
}
