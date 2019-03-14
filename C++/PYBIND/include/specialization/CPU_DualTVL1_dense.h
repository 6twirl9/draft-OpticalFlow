#ifndef __IMPL__CPU_DualTVL1_dense__
#define __IMPL__CPU_DualTVL1_dense__

#define __H2D_CONVERSION__ 0
#define __KERNEL__         CPU_DualTVL1_dense
#define __SIGNATURE__      double,double,double,int,int,double,int,int,double,double,int,bool
#define __IMPLEMENTATION__ cv::DualTVL1OpticalFlow
#define __KER_MAT__        cv::Mat
#define __DEFAULT__        0.25,0.15,0.3,5,5,0.01,30,10,0.8,0.0,5,false
#define __VAR__            "tau","lambda","theta","nscales","warps","epsilon","innnerIterations","outerIterations","scaleStep","gamma","medianFiltering","useInitialFlow"

#else
#undef  __IMPL__CPU_DualTVL1_dense__

#undef  __VAR__
#undef  __DEFAULT__
#undef  __KER_MAT__
#undef  __IMPLEMENTATION__
#undef  __SIGNATURE__
#undef  __KERNEL__
#undef  __H2D_CONVERSION__

#endif
