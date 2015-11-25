#!/usr/bin/env bash

astyle --style=allman --recursive -p -n *.cpp *.h

find . -name "Makefile" -delete
find . -name "CMakeCache.txt" -delete
find . -name "CMakeFiles" -type d -exec rm -r {} \;
find . -name "cmake_install.cmake" -delete