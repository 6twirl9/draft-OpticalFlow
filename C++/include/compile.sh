#1/bin/bash
set -u
file=$1
g++-7.4.0 $file -std=c++17 -E -I/project/at073-elderfall/opt/boost/include -I.

#g++ -shared optflow.o -o liboptflow.so -Wl,-rpath=/project/at073-elderfall/opt/opencv-3.4.4/lib,--no-as-needed `pkg-config --libs opencv`
