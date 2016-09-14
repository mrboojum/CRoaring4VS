# CppRoaring4VS
C++ Roaring bitmaps for microsoft Visual Studio 2015 compiled with clang.


# Introduction

A C++ wrapper around the CRoaring project to enable compilation of CRoaring with Microsoft Visual Studio 2015 with clang.

[CRoaring]: https://github.com/RoaringBitmap/CRoaring


# Objective

The primary goal of the CRoaringVS is to provide a static library that can be used in Microsoft Visual Studio 2015 compiled projects. 


# Requirements

- The library requires Microsoft Visual Studio 2015 update 3 with clang installed. It is only tested on Windows 7.


# Known shortcomings
- Doesn't work yet due to an unresolved bug resulting in the test failing with error message:
The Block at 0x.... was allocated by aligned routines, use _aligned_free().
- Microsoft Visual Studio 2015 is not fully supporting intel intrinsics. A bug has been reported [here:] (https://connect.microsoft.com/VisualStudio/feedback/details/3102596/clang-c2-intel-intrinsics-support-for-clang)

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




