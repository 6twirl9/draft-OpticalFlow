#ifndef __IMPL__CUDA_Farneback_dense__
#define __IMPL__CUDA_Farneback_dense__

#define __H2D_CONVERSION__ 1
#define __KERNEL__         CUDA_Farneback_dense
#define __SIGNATURE__      int,double,bool,int,int,int,double,int
#define __IMPLEMENTATION__ cv::cuda::FarnebackOpticalFlow
#define __KER_MAT__        cv::cuda::GpuMat
#define __DEFAULT__        5,0.5,false,13,10,5,1.1,0
#define __VAR__            "numLevels","pyrScale","fastPyramids","winSize","numIters","polyN","polySigma","flags"

#else
#undef  __IMPL__CUDA_Farneback_dense__

#undef  __VAR__
#undef  __DEFAULT__
#undef  __KER_MAT__
#undef  __IMPLEMENTATION__
#undef  __SIGNATURE__
#undef  __KERNEL__
#undef  __H2D_CONVERSION__

#endif
