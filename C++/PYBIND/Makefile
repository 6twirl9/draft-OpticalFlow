#
# Requred:
# 
# - CUDA enabled OpenCV
# - c++17 compliant compiler
# - PyBind
#

CXX := g++-7.4.0

CXXFLAGS := -std=c++17 -fPIC

DEP_INC := $(shell find include -type f)

INCFLAGS := \
 -I/opt/conda/include/python3.6m \
 -I/opt/conda/lib/python3.6/site-packages/numpy/core/include \
 \
 -I/project/at073-elderfall/opt/opencv/include \
 -I/project/at073-elderfall/opt/source/opencv-3.4.4/BUILD/modules/python_bindings_generator \
 -I/project/at073-elderfall/opt/source/opencv-3.4.4/modules/python/src2 \
 \
 -I/project/at073-elderfall/opt/boost/include \
 -Iinclude \
 -I. \
 -I/project/at073-elderfall/opt/pybind11/include $(shell python -m pybind11 --includes)

LDFLAGS := \
 -Wl,-rpath=/project/at073-elderfall/opt/opencv-3.4.4/lib,--no-as-needed $(shell pkg-config --libs opencv)

.PHONY: default

default: _opticalflow.so

%.o: %.cpp $(DEP_INC)
	@echo "COMPILE $<"
	@$(CXX) -o $@ -c $(CXXFLAGS) $(INCFLAGS) $<

_opticalflow.so: _opticalflow.o
	@echo "LINK    $@"
	@$(CXX) -o $@ -shared $^ $(LDFLAGS)

t-opticalflow: t-opticalflow.o
	@echo "LINK    $@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	-rm -f *.o *.so
