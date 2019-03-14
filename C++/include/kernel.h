#include <boost/preprocessor/tuple/size.hpp>

#define __KERNEL_AVAILABLE__    \
(                               \
  CPU_DualTVL1_dense            \
, CPU_Farneback_dense           \
, CUDA_DualTVL1_dense           \
, CUDA_Farneback_dense          \
, CUDA_PyrLK_dense              \
, CUDA_Brox_dense               \
)


//#define __KERNEL_AVAILABLE__ (CUDA_DualTVL1_dense,CPU_DualTVL1_dense)
//#define __KERNEL_AVAILABLE__ (CPU_Farneback_dense)
/*
#define __KERNEL_AVAILABLE__    \
(                               \
  CPU_DualTVL1_dense            \
, CPU_Farneback_dense           \
, CUDA_Brox_dense               \
, CUDA_DualTVL1_dense           \
, CUDA_Farneback_dense          \
, CUDA_PyrLK_dense              \
)
*/

#define USER_BOOST_PP_TUPLE

#define __KERNEL_AVAILABLE_N__ BOOST_PP_TUPLE_SIZE(__KERNEL_AVAILABLE__)

#if !BOOST_PP_IS_ITERATING

 #ifndef FILE_H_
 #define FILE_H_

 #include <boost/preprocessor/iteration/iterate.hpp>
 #include <boost/preprocessor/arithmetic/sub.hpp>

 #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PP_SUB(__KERNEL_AVAILABLE_N__,1), "kernel-iter.h"))
 #include BOOST_PP_ITERATE()

 #endif

#endif
