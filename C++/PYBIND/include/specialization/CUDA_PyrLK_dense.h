#ifndef __IMPL__CUDA_PyrLK_dense__
#define __IMPL__CUDA_PyrLK_dense__

#define __H2D_CONVERSION__ 1
#define __KERNEL__         CUDA_PyrLK_dense
#define __SIGNATURE__      cv::Size,int,int,bool
#define __IMPLEMENTATION__ cv::cuda::DensePyrLKOpticalFlow
#define __KER_MAT__        cv::cuda::GpuMat
#define __DEFAULT__        cv::Size(13,13),3,30,false
#define __VAR__            "winSize","maxLevel","iters","useInitialFlow"

#else
#undef  __IMPL__CUDA_PyrLK_dense__

#undef  __VAR__
#undef  __DEFAULT__
#undef  __KER_MAT__
#undef  __IMPLEMENTATION__
#undef  __SIGNATURE__
#undef  __KERNEL__
#undef  __H2D_CONVERSION__

#endif
