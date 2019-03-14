#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

//efine __KITER__TUPLE (CUDA_DualTVL1_dense,CPU_DualTVL1_dense)

#define __KITER__INC_(_) BOOST_PP_STRINGIZE(specialization/_.h)
#define __KITER__INC(_) __KITER__INC_(_)
//efine __KITER__Q      BOOST_PP_TUPLE_ELEM(2, BOOST_PP_ITERATION(), __KITER__TUPLE)
#define __KITER__Q      BOOST_PP_TUPLE_ELEM(__KERNEL_AVAILABLE_N__, BOOST_PP_ITERATION(), __KERNEL_AVAILABLE__)

#include __KITER__INC(__KITER__Q)
#include "kernel-template.cpp"
#include __KITER__INC(__KITER__Q)
