# CppRoaring4VS

[CRoaring](https://github.com/RoaringBitmap/CRoaring) is a C11-compliant library providing [compressed Roaring bitmaps](http://roaringbitmap.org). CppRoaring4VS provides a
C++ wrapper for Microsoft Visual Studio 2015.

CRoaring: https://github.com/RoaringBitmap/CRoaring


# Objective

The primary goal of the CRoaringVS is to provide a static library that can be used in Microsoft Visual Studio 2015 compiled projects.


# Requirements

- Microsoft Visual Studio 2015 Community Edition update 3 or better.
- Within Visual Studion, have "Clang with Microsoft CodeGen" installed (found under Cross Platform Mobile Development / Visual C++ Mobile Development in the Modify features list).

# Usage

Load ``msv/CRoaringVSLib.sln`` into Microsoft Visual Studio. The project should then build both the library and small test program that you can run.

# Known shortcomings
- Microsoft Visual Studio 2015 does not support Intel intrinsics thus we have to disable several optimizations (such as AVX2 support). A bug has been submitted:  https://connect.microsoft.com/VisualStudio/feedback/details/3102596/clang-c2-intel-intrinsics-support-for-clang

# Project structure
- Sources:
The original amalgamated CRoaring source files can be found in the directory src/croaring.
The c++ header file for this library in the directory inc.
The c++ implementation file for this library in the directory src.
The source file for the test is in the directory tst.
- Project files:
The Microsoft Visual Studio 2015 solution file is available in the directory msv.
This solution contains the following 2 projects each in its own subdirectory: CRoaringVSLib (to build the library), CRoaringVSTest (to test the CRoaringVSLib library).
- Library files
The static .lib files are in the lib directory.

# Relevant Tips when working with C99/C11-standard compliant code in Visual Studio

- Consider disabling Intellisense since it can't parse standard compliant C code: Tools -> Options -> Text Editor -> C/C++ -> Advanced, and set Disable Error Reporting to True.
-  Once you have a clang CodeGen project, you can go under Configuration Properties, C/C++, Language, C Language Standard and choose between C89, C99, C11, C99 GNU, C11 GNU.
