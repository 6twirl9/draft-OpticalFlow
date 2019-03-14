#ifndef __IMPL__CUDA_DualTVL1_dense__
#define __IMPL__CUDA_DualTVL1_dense__

#define __H2D_CONVERSION__ 1
#define __KERNEL__         CUDA_DualTVL1_dense
#define __SIGNATURE__      double,double,double,int,int,double,int,double,double,bool
#define __IMPLEMENTATION__ cv::cuda::OpticalFlowDual_TVL1
#define __KER_MAT__        cv::cuda::GpuMat
#define __DEFAULT__        0.25,0.15,0.3,5,5,0.01,300,0.8,0.0,false
#define __VAR__            "tau","lambda","theta","nscales","warps","epsilon","iterations","scaleStep","gamma","useInitialFlow"

#else
#undef  __IMPL__CUDA_DualTVL1_dense__

#undef  __VAR__
#undef  __DEFAULT__
#undef  __KER_MAT__
#undef  __IMPLEMENTATION__
#undef  __SIGNATURE__
#undef  __KERNEL__
#undef  __H2D_CONVERSION__

#endif
