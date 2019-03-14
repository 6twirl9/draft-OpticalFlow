#ifndef __IMPL__CUDA_Brox_dense__
#define __IMPL__CUDA_Brox_dense__

#define __H2D_CONVERSION__ 1
#define __KERNEL__         CUDA_Brox_dense
#define __SIGNATURE__      double,double,double,int,int,int
#define __IMPLEMENTATION__ cv::cuda::BroxOpticalFlow
#define __KER_MAT__        cv::cuda::GpuMat
#define __DEFAULT__        0.197,50.0,0.8,5,150,10
#define __VAR__            "alpha","gamma","scale_factor","inner_iterations","outer_iterations","solver_iterations"

#else
#undef  __IMPL__CUDA_Brox_dense__

#undef  __VAR__
#undef  __DEFAULT__
#undef  __KER_MAT__
#undef  __IMPLEMENTATION__
#undef  __SIGNATURE__
#undef  __KERNEL__
#undef  __H2D_CONVERSION__

#endif
