// === #include...
#include <iostream>
#include <fstream>
#include <cstring>
///

#define __EXPORT__(module) _##module
using namespace std ;

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#define BUILD_PYTHON

#include "include/contrib/misc.h"
#include "include/opticalflow.h"

namespace py = pybind11 ;
using namespace pybind11::literals ;

// === Rudolves

uint8_t BufProtoFormat2OpenCVType(const string s,int ch)
{
 uint8_t r = CV_8UC1 ;
// #define rep(_0,_1) case _0: return CV_##_1##C(ch) ;
 #define rep(_0,_1) case _0: r = CV_##_1##C(ch) ; break ;
 switch(s[0])
 {
  rep('b', 8S) rep('B', 8U)
  rep('h',16S) rep('H',16U)
  rep('i',32S) //p('I',32U)
  //p('q',64S) rep('Q',64U)
  rep('f',32F) rep('d',64F)

  default: break ;
 }
 #undef  rep
 cout << s << " : " << ch << " : " <<  int(r) << endl ;

return r ;
}

auto BufProtoFormat2OpenCVType(py::buffer_info& info)
{
auto s = info.shape ;
int  c = 1 ;

 if(s.size() == 3) c = s[2] ;

auto cv_type = BufProtoFormat2OpenCVType(info.format,c) ;

return cv_type ;

//return [&s,&cv_type](auto ptr) { return cv::Mat(s[0],s[1],cv_type,ptr) ; } ;
}


using __VarCV_TYPE__ = variant<int8_t,uint8_t,int16_t,uint16_t,int32_t,float,double> ;

__VarCV_TYPE__ VarCV_TYPE(uint8_t cv_type)
{
__VarCV_TYPE__ r = uint8_t(0) ;

 uint8_t depth = cv_type & CV_MAT_DEPTH_MASK;
 uint8_t ch = 1 + (cv_type >> CV_CN_SHIFT);

 #define rep(_0,_1,_2,_3) if( cv_type == CV_##_1##_2##C(ch) ) { r = _3##int##_1##_t(0) ; cout << "*** " << #_1 #_2 << ch << endl ; }
  rep('b', 8,S, ) rep('B', 8,U,u)
  rep('h',16,S, ) rep('H',16,U,u)
  rep('i',32,S, ) //p('I',32,U,u)
  //p('q',64,S, ) rep('Q',64,U,u)
 #undef  rep

 #define rep(_0,_1,_2) if( cv_type == CV_##_1##C(ch)) { r = _2(0) ; cout << "*** " << #_2 << endl ; }
  rep('f',32F,float) rep('d',64F,double)
 #undef  rep

return r ;
}

//template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
//template<class... Ts> overload(Ts...) -> overload<Ts...>;

template<typename T, typename... Ss>
auto py_array_t(T* _p, Ss... Args)
{
// py::capsule free_when_done(_p, [](void *_q) { delete[] reinterpret_cast<T *>(_q); });
 return py::array_t<T>({int(Args)...},_p /*, free_when_done*/) ;
}

///

#define DFLT(_k,_v) py::arg(#_k) = _v

PYBIND11_MODULE(__EXPORT__(opticalflow), m)
{
 py::class_<OpticalFlow>(m,"OpticalFlow",py::buffer_protocol(),py::dynamic_attr())

 .def(py::init<>())

 .def("info",&OpticalFlow::info_,DFLT(all,false))
 .def("load",&OpticalFlow::load,DFLT(implementation,""),DFLT(verbose,false))

#define rep(device) \
 .def(#device,&OpticalFlow::device,DFLT(_,""))

 rep(device)
 rep(algorithm)
 rep(feature)
 rep(implementation)

#undef  rep

 .def       // === kernel
 (
   "kernel"
 , [](OpticalFlow& self, py::dict opt, bool verbose)
   {
    return self.kernel(OpticalFlow::DICT(opt),verbose) ;
   }
 , DFLT(opt,py::dict())
 , DFLT(verbose,false)
 )
///
 .def       // === __call__ image0, image1
 (
   "__call__"
 , [](OpticalFlow& self, py::buffer b0, py::buffer b1)
   {
    py::buffer_info bi[2] ;

     bi[0] = b0.request() ;
     bi[1] = b1.request() ;

    auto s = bi[0].shape ;

    auto cv_type = BufProtoFormat2OpenCVType(bi[0]) ;

    vector<VarMat> f(2) ;

     for(int i = 0 ; i < 2 ; i++ )
     {
      f[i] = self.toKerMat(cv::Mat(s[0],s[1],cv_type,bi[i].ptr)) ;
     }

    double elapsed ;

    auto hold = self(f[0],f[1],elapsed) ;

    py::list collect ;

    std::visit
    (
     overload
     {
      [&collect,&hold](auto _ )
      {
       int ch = 1 + (hold.type() >> CV_CN_SHIFT) ;
       auto s = hold.size ;
       collect.append(py_array_t(hold.ptr<decltype(_)>(0),s[0],s[1],ch)) ;
      }
     }
    , VarCV_TYPE(hold.type())
    );

    py::list ret ;

    ret.append(collect) ;
    ret.append(elapsed) ;

   return ret ;
   }
 )
///
 .def       // === __call__ list of images
 (
   "__call__"
 , [](OpticalFlow& self, py::list bs, bool wrap = false)
   {
    // === numpy array -> cv::Mat

    vector<py::buffer_info> info ;

    for(auto _ : bs)

     info.push_back(py::cast<py::buffer>(_).request()) ;

    auto s = info[0].shape ;

    auto cv_type = BufProtoFormat2OpenCVType(info[0]) ;

    cv::Mat::setDefaultAllocator(cv::cuda::HostMem::getAllocator (cv::cuda::HostMem::AllocType::PAGE_LOCKED));

  //vector<VarMat> f(info.size()) ;
    vector<cv::Mat> f(info.size()) ;

     for(int i = 0 ; i < info.size() ; i++ )
     {
//    f[i] = self.toKerMat(cv::Mat(s[0],s[1],cv_type,info[i].ptr)) ;
      f[i] = cv::Mat(s[0],s[1],cv_type,info[i].ptr) ;
     }

    if(wrap) f.push_back(f[0]) ;

///

    double elapsed ;

    py::list collect ;

    for(auto hold : self(f,elapsed))

     std::visit
     (
      overload
      {
       [&collect,&hold](auto _ )
       {
        int ch = 1 + (hold.type() >> CV_CN_SHIFT) ;
        auto s = hold.size ;
        collect.append(py_array_t(hold.ptr<decltype(_)>(0),s[0],s[1],ch)) ;
       }
      }
     , VarCV_TYPE(hold.type())
     );

    py::list ret ;

    ret.append(collect) ;
    ret.append(elapsed) ;

   return ret ;
   }
 , DFLT(bs,py::list())
 , DFLT(wrap,false)
 )
///

 ;
}

