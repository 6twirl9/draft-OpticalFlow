
CXX := g++-7.4.0
SRC := t-opticalflow.cpp

CXXFLAGS := -std=c++17

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
 -I.

LDFLAGS := \
 -Wl,-rpath=/project/at073-elderfall/opt/opencv-3.4.4/lib,--no-as-needed $(shell pkg-config --libs opencv)

.PHONY: default

default: t-opticalflow

t-opticalflow.o:%.o: %.cpp $(DEP_INC)
	@echo "COMPILE $<"
	@$(CXX) -o $@ -c $(CXXFLAGS) $(INCFLAGS) $<

t-opticalflow: t-opticalflow.o
	@echo "LINK    $@"
	@$(CXX) -o $@    $(CXXFLAGS) $(INCFLAGS) $< $(LDFLAGS)

clean:
	-rm -f *.o t-opticalflow
