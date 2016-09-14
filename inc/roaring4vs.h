/*
A C++ wrapper around the CRoaring project to enable compilation of CRoaring with Microsoft Visual Studio 2015 with clang.
*/
#ifndef INCLUDE_ROARING4VS_H_
#define INCLUDE_ROARING4VS_H_

#include <cstdint>

namespace roaring {

    class Roaring4VS {
    public:
        /**
         * Create an empty bitmap
         */
        Roaring4VS();

        /**
         * Construct a bitmap from a list of integer values.
         */
        Roaring4VS(size_t n, const uint32_t *data);

        /**
         * Copy constructor
         */
        Roaring4VS(const Roaring4VS &r);

        /**
         * Construct a roaring object from the C struct.
         *
         * Passing a NULL point is unsafe.
         */
        Roaring4VS(void *s);

        /**
         * Construct a bitmap from a list of integer values.
         */
        static Roaring4VS bitmapOf(size_t n, ...);

        /**
         * Add value x
         *
         */
        void add(uint32_t x);

        /**
         * Add value n_args from pointer vals
         *
         */
        void addMany(size_t n_args, const uint32_t *vals);

        /**
         * Remove value x
         *
         */
        void remove(uint32_t x);

        /**
         * Check if value x is present
         */
        bool contains(uint32_t x) const;

        /**
         * Destructor
         */
        ~Roaring4VS();

        /**
         * Copies the content of the provided bitmap, and
         * discard the current content.
         */
        Roaring4VS &operator=(const Roaring4VS &r);

        /**
         * Compute the intersection between the current bitmap and the provided
         * bitmap,
         * writing the result in the current bitmap. The provided bitmap is not
         * modified.
         */
        Roaring4VS &operator&=(const Roaring4VS &r);

        /**
         * Compute the union between the current bitmap and the provided bitmap,
         * writing the result in the current bitmap. The provided bitmap is not
         * modified.
         *
         * See also the fastunion function to aggregate many bitmaps more quickly.
         */
        Roaring4VS &operator|=(const Roaring4VS &r);

        /**
         * Compute the symmetric union between the current bitmap and the provided
         * bitmap,
         * writing the result in the current bitmap. The provided bitmap is not
         * modified.
         */
        Roaring4VS &operator^=(const Roaring4VS &r);

        /**
         * Exchange the content of this bitmap with another.
         */
        void swap(Roaring4VS &r);

        /**
         * Get the cardinality of the bitmap (number of elements).
         */
        uint64_t cardinality() const;

        /**
        * Returns true if the bitmap is empty (cardinality is zero).
        */
        bool isEmpty() const;

        /**
         * Convert the bitmap to an array. Write the output to "ans",
         * caller is responsible to ensure that there is enough memory
         * allocated
         * (e.g., ans = new uint32[mybitmap.cardinality()];)
         */
        void toUint32Array(uint32_t *ans) const;

        /**
         * Return true if the two bitmaps contain the same elements.
         */
        bool operator==(const Roaring4VS &r) const;

        /**
         * compute the negation of the roaring bitmap within a specified interval.
         * areas outside the range are passed through unchanged.
         */
        void flip(uint64_t range_start, uint64_t range_end);

        /**
         *  Remove run-length encoding even when it is more space efficient
         *  return whether a change was applied
         */
        bool removeRunCompression();

        /** convert array and bitmap containers to run containers when it is more
         * efficient;
         * also convert from run containers when more space efficient.  Returns
         * true if the result has at least one run container.
         */
        bool runOptimize();

        /**
         * Iterate over the bitmap elements. The function iterator is called once
         * for
         *  all the values with ptr (can be NULL) as the second parameter of each
         * call.
         *
         *  roaring_iterator is simply a pointer to a function that returns void,
         *  and takes (uint32_t,void*) as inputs.
         */
         //void iterate(RoaringIterator iterator, void *ptr) const;

         /**
          * If the size of the roaring bitmap is strictly greater than rank, then
          this
            function returns true and set element to the element of given rank.
            Otherwise, it returns false.
          */
        bool select(uint32_t rank, uint32_t *element) const;

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
        size_t write(char *buf, bool portable = true) const;

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
        static Roaring4VS read(const char *buf, bool portable = true);

        /**
         * How many bytes are required to serialize this bitmap (meant to be
         * compatible
         * with Java and Go versions)
         *
         * Setting the portable flag to false enable a custom format that
         * can save space compared to the portable format (e.g., for very
         * sparse bitmaps).
         */
        size_t getSizeInBytes(bool portable = true) const;

        /**
         * Computes the intersection between two bitmaps and returns new bitmap.
         * The current bitmap and the provided bitmap are unchanged.
         */
        Roaring4VS operator&(const Roaring4VS &o) const;

        /**
         * Computes the union between two bitmaps and returns new bitmap.
         * The current bitmap and the provided bitmap are unchanged.
         */
        Roaring4VS operator|(const Roaring4VS &o) const;

        /**
         * Computes the symmetric union between two bitmaps and returns new bitmap.
         * The current bitmap and the provided bitmap are unchanged.
         */
        Roaring4VS operator^(const Roaring4VS &o) const;

        /**
         * Whether or not we apply copy and write.
         */
        void setCopyOnWrite(bool val);

        /**
         * Print the content of the bitmap
         */
        void printf();

        /**
         * Whether or not copy and write is active.
         */
        bool getCopyOnWrite() const;

        /**
         * computes the logical or (union) between "n" bitmaps (referenced by a
         * pointer).
         */
        static Roaring4VS fastunion(size_t n, const Roaring4VS **inputs);

        void *roaring;
    };

}

#endif /* INCLUDE_ROARING4VS_H_ */
