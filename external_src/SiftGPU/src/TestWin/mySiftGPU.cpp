////////////////////////////////////////////////////////////////////////////
//    File:        SimpleSIFT.cpp
//    Author:      Changchang Wu
//    Description : A simple example shows how to use SiftGPU and SiftMatchGPU
//
//
//
//    Copyright (c) 2007 University of North Carolina at Chapel Hill
//    All Rights Reserved
//
//    Permission to use, copy, modify and distribute this software and its
//    documentation for educational, research and non-profit purposes, without
//    fee, and without a written agreement is hereby granted, provided that the
//    above copyright notice and the following paragraph appear in all copies.
//    
//    The University of North Carolina at Chapel Hill make no representations
//    about the suitability of this software for any purpose. It is provided
//    'as is' without express or implied warranty. 
//
//    Please send BUG REPORTS to ccwu@cs.unc.edu
//
////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <vector>
#include <iostream>
using std::vector;
using std::iostream;


////////////////////////////////////////////////////////////////////////////
#if !defined(SIFTGPU_STATIC) && !defined(SIFTGPU_DLL_RUNTIME) 
// SIFTGPU_STATIC comes from compiler
#define SIFTGPU_DLL_RUNTIME
// Load at runtime if the above macro defined
// comment the macro above to use static linking
#endif

#ifdef _WIN32
    #ifdef SIFTGPU_DLL_RUNTIME
        #define WIN32_LEAN_AND_MEAN
        #include <windows.h>
        #define FREE_MYLIB FreeLibrary
        #define GET_MYPROC GetProcAddress
    #else
        //define this to get dll import definition for win32
        #define SIFTGPU_DLL
        #ifdef _DEBUG 
            #pragma comment(lib, "../../lib/siftgpu_d.lib")
        #else
            #pragma comment(lib, "../../lib/siftgpu.lib")
        #endif
    #endif
#else
    #ifdef SIFTGPU_DLL_RUNTIME
        #include <dlfcn.h>
        #define FREE_MYLIB dlclose
        #define GET_MYPROC dlsym
    #endif
#endif

#include "../SiftGPU/SiftGPU.h"

int main(int argc, char * argv[])
{
    //this will use overloaded new operators
    SiftGPU  *sift = new SiftGPU;
	sift->ParseParam(argc, argv);
	if(sift->CreateContextGL() != SiftGPU::SIFTGPU_FULL_SUPPORTED) return 0;
std::cout << "-" << sift->GetImageCount() << std::endl;
	 for (int i = 0; i < sift->GetImageCount(); i++) {
    if(sift->RunSIFT(i))
    {
    }
}
    delete sift;
    return 1;
}


