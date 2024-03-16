#!/bin/sh
glslangValidator -V kernel2D_draw.comp -o kernel2D_draw.comp.spv -DGLSL -I.. -I../kernel_slicer/TINYSTL -I/home/roman/Desktop/nair/SphereTracing/external/LiteMath 
