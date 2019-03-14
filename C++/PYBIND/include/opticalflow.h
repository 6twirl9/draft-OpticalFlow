// === #include...
#include <iostream>
#include <ostream>
#include <iomanip>

#include <tuple>
#include <vector>
#include <array>
#include <list>
#include <set>
#include <deque>

#include <algorithm>
#include <variant>
#include <unordered_map>
#include <functional>
#include <utility>
#include <memory>
#include <type_traits>
#include <initializer_list>

#include <stdint.h>

#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cudaoptflow.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/cudaarithm.hpp>

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
///

#include "include/contrib/boost.h"
#include "include/contrib/cv2.h"
#include "include/UGLY.h"
#include "include/kernel.h"

using namespace std ;

struct OpticalFlow
{
// === Misc.

using THIS = OpticalFlow ;
using DICT = DICT_DEFAULT_t ;

struct IMPLEMENTATION ;
using IMPL = OpticalFlow::IMPLEMENTATION ;

// Why is this not in a database?
struct IMPLEMENTATION // ===
{
using IMPL = IMPLEMENTATION ;

// === Exception
struct NotImplemented : public exception
{
 const string msg ;

 virtual const char * what() const throw ()
 {
  return msg.c_str() ;
 }

 NotImplemented() :                                msg("No valid implementation found!") {}
 NotImplemented(string implementation) :           msg("Implementation " + implementation + " not found!") {}
 NotImplemented(string device, string algorithm, string feature = "dense" ) : msg("No implementation found for algorithm " + algorithm + " (" + feature + ") targetting " + device + ".") {}
 
} ;
///

struct GET  // ===
{
 static auto _device        (auto& _) { return get<0>(_) ; }
 static auto _algorithm     (auto& _) { return get<1>(_) ; }
 static auto _feature       (auto& _) { return get<2>(_) ; }
 static auto _implementation(auto& _) { return get<3>(_) ; }
} ;
///
struct SET  // ===
{
 static void _device        (auto& _, auto v) { get<0>(_) = v ; }
 static void _algorithm     (auto& _, auto v) { get<1>(_) = v ; }
 static void _feature       (auto& _, auto v) { get<2>(_) = v ; }
 static void _implementation(auto& _, auto v) { get<3>(_) = v ; }
} ;
///

typedef vector<tuple<string,string,string,string,string>> TABLE ;
using T = TABLE ;

static TABLE t ;

static auto available(string implementation)             // ===
{
 auto pred1 = [] (auto _, auto _0) { return ( GET::_implementation(_) == _0 ) ; } ;
 TABLE collect ;
 for( auto it = IMPL::t.begin() ; it != IMPL::t.end() ; it++ )
 {
  if( pred1(*it,implementation) )
   collect.push_back(*it)
  ;
 }

 if( collect.size() == 0 )

  throw NotImplemented(implementation) ;

 return collect ;
}
///
static auto available(string device, string algorithm, string feature = "dense")   // ===
{
 auto pred3 = [](auto _, auto _0, auto _1, auto _2) { return ( GET::_device(_) == _0 ) and ( GET::_algorithm(_) == _1 ) and ( GET::_feature(_) == _2 ) ; } ;
 TABLE collect ;
 for( auto it = IMPLEMENTATION::t.begin() ; it != IMPLEMENTATION::t.end() ; it++ )
 {
  if( pred3(*it,device,algorithm,feature) )
   collect.push_back(*it)
  ;
 }

 if( collect.size() == 0 )

  throw NotImplemented(device,algorithm,feature) ;

 return collect ;
}
///

} ;

///

DICT_t<string> state ;

template<typename Sig>
auto slice(auto dict,auto key,bool verbose = false)
{
 return for_each_in_tuple4<Sig>(key, [&dict](const auto &key )->auto { return dict[key] ; },verbose);
}

#define rep(mode,device)                \
THIS& device(string _ = "")             \
{                                       \
 auto Q = state ;                       \
                                        \
 if( _ != "" )                          \
                                        \
  Q[#device] = _ ;                      \
                                        \
 if( hasValidImplementation(Q,mode) )   \
                                        \
  state = Q ;                           \
                                        \
 return *this ;                         \
}

rep(0,device)
rep(0,algorithm)
rep(0,feature)
rep(1,implementation)

string _implementation() { return state._string("implementation") ; }
string _implementation_alias() { return state._string("alias") ; }

#undef rep

typedef DICT_t<USER_BOOST_PP_STRIP_BRACE(__KERNEL_AVAILABLE__)> KERNEL_t ;
KERNEL_t Ker ;

typedef KERNEL_t::VALUE_t V ;
template<typename T> static V GenKer() { return V(T()) ; }
unordered_map<string,function<V()>> KerGen ;

///

OpticalFlow()   // ===
{
 // Ensure we start with a valid state

 state = { { "device", "CUDA" }, { "algorithm", "DualTVL1" } , { "feature", "dense" }, { "implementation", "" }, { "alias", "" } } ;

 auto Q = state ;

 if( hasValidImplementation(Q) )

  state = Q ;

 #define rep(r, data, elem) KerGen[BOOST_PP_STRINGIZE(elem)] = GenKer<elem> ;

 BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

 #undef  rep
}
///

// Instantiate implementation on demand

bool load(string implementation = "", bool verbose = false)  // ===
{
 bool found = false ;
 string impl = implementation ;

 if( impl == "" )

  impl = _implementation_alias() ;

 auto it = Ker.begin() ;
 for( ; it != Ker.end() ; it ++ ) { if(verbose) cout << "SEARCH " << it->first << " / " << impl << endl ; if(it->first == impl) break ; }

 found = not ( it == Ker.end() ) ;

 bool OK = false ;

 if( not found )
 {
 #define rep(r, data, s) rep_(s,BOOST_PP_STRINGIZE(s))
 #define rep_(s,s_str) if( impl == s_str ) { Ker[s_str] = get<s>(KerGen[s_str]()) ; OK = true ; }

 BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

 #undef  rep_
 #undef  rep

  if(verbose)
  {
   if(OK) cout << "Loaded implementation " << impl << endl ;
   else   cout << "Implementation " << impl << " NOT loaded :(" << endl ;
  }
 }
 else
 {
  OK = true ;
  if(verbose)
  {
   cout << "Implementation " << impl << " already loaded!" << endl ;
  }
 }

 return OK ;
}
///

// Dispatcher to methods in actual optical flow implementations

THIS& kernel(DICT opt = DICT(),bool verbose=false)  // ===
{
 if( not hasValidImplementation(state) )

  return *this ;

 auto impl = _implementation_alias() ;

 if( load(impl) )
 {
 #define rep(r, data, s) rep_(s,BOOST_PP_STRINGIZE(s))
 #define rep_(s,s_str) if( impl == s_str ) { get<s>(Ker[s_str]).kernel(opt,verbose) ; }

 BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

 #undef  rep_
 #undef  rep
 }

 return *this ;
}
///
template<typename ... Ts>
auto operator()(Ts&& ... args)  // ===
{
 if( hasValidImplementation(state) )
 {

 auto impl = _implementation_alias() ;

  if( load(impl) )
  {
  #define rep(r, data, s) rep_(s,BOOST_PP_STRINGIZE(s))
  #define rep_(s,s_str) if( impl == s_str ) { cout << ":" << impl <<endl ;  return CALL(get<s>(Ker[s_str]),forward<Ts>(args) ... ) ; }

  BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

  #undef  rep_
  #undef  rep
  }
 }

}
///
template<typename ... Ts>
auto toKerMat(Ts&& ... args)  // ===
{
 if( hasValidImplementation(state) )
 {
 auto impl = _implementation_alias() ;

 if( load(impl) )
 {
 #define rep(r, data, s) rep_(s,BOOST_PP_STRINGIZE(s))
 #define rep_(s,s_str) if( impl == s_str ) { return get<s>(Ker[s_str]).toKerMat(forward<Ts>(args) ... ) ; }
 

 BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

 #undef  rep_
 #undef  rep
 }
 }

}
///
template<typename ... Ts>
auto toKerMat_(Ts&& ... args)  // ===
{
 auto impl = _implementation_alias() ;

 #define rep(r, data, s) rep_(s,BOOST_PP_STRINGIZE(s))
 #define rep_(s,s_str) if( impl == s_str ) { return get<s>(Ker[s_str]).toKerMat(forward<Ts>(args) ... ) ; }

 BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

 #undef  rep_
 #undef  rep
}
///

// Do nothing
void operator()(void) { }

THIS& info(bool all=false)    // ===
{
 typedef decltype(gen_tuple_type<string,5>()) T3S ;
 cout << "----------------------------------------------------------------" << endl ;
 slice<T3S>(state,T3S("device","algorithm","feature","implementation","alias"),true);

 auto cur_impl = _implementation_alias() ;

 auto it = Ker.begin() ;
 for( ; it != Ker.end() ; it ++ )
 {
 auto impl = it->first ;

 if( not all and impl != cur_impl ) continue ;

 #define rep(r, data, s) rep_(s,BOOST_PP_STRINGIZE(s))
 #define rep_(s,s_str) if( impl == s_str ) { get<s>(Ker[s_str]).info() ; }

 BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

 #undef  rep_
 #undef  rep

 cout << flush ;
 }
 
 return *this ;
}
///
auto info_(bool all=false)    // ===
{
namespace py = pybind11 ;
py::dict dict, dict_impl, dict_detail ;

for(auto item : state)
{
 std::visit
 (
   overload
   {
    [&dict_impl,&item](auto _) { dict_impl[py::str(item.first)] = _ ; }
   }
 , item.second
 ) ;
}

 auto cur_impl = _implementation_alias() ;

 auto it = Ker.begin() ;
 for( ; it != Ker.end() ; it ++ )
 {
 auto impl = it->first ;

 if( not all and impl != cur_impl ) continue ;

 #define rep(r, data, s) rep_(s,BOOST_PP_STRINGIZE(s))
 #define rep_(s,s_str) if( impl == s_str ) { dict_detail[py::str(impl)] = get<s>(Ker[s_str]).info_() ; }

 BOOST_PP_SEQ_FOR_EACH(rep, _, BOOST_PP_TUPLE_TO_SEQ(__KERNEL_AVAILABLE__))

 #undef  rep_
 #undef  rep

 }

dict["current"] = dict_impl ;
dict["detail"] = dict_detail ;
 
return dict ;
}
///

bool hasValidImplementation(auto& state, int mode = 0)  // ===
{
 bool OK = false ;

 if( mode == 0 )    // ===
 {
  typedef decltype(gen_tuple_type<string,3>()) T3S ;
  auto t = tuple2iter<vector>(slice<T3S>(state,T3S("device","algorithm","feature"),false));

  try
  {
   auto r = tuple2iter<vector>(IMPL::available(t[0],t[1],t[2])[0]) ;

   state["implementation"] = r[3] ;
   state["alias"]          = r[4] ;

   OK = true ;
  }
  catch( IMPL::NotImplemented& e)
  {
   cout << " *** " << e.what() << endl ;
   cout << "implementation not set" << endl ;
  }
 }
///
 if( mode == 1 )    // ===
 {
  auto t = state._string("implementation") ;

  try
  {
   auto r = tuple2iter<vector>(IMPL::available(t)[0]) ;

   state = { { "device", r[0] }, { "algorithm", r[1] } , { "feature", r[2] }, { "implementation", r[3] }, { "alias", r[4] } } ;

   OK = true ;
  }
  catch( IMPL::NotImplemented& e)
  {
   cout << " *** " << e.what() << endl ;
   cout << "implementation not set" << endl ;
  }
 }
///

 return OK ;
}
///

// Main entry

template<class OP,typename ... Ts>
auto CALL(OP op, Ts&& ... args)
{
 cout << "CALL(OP op, Ts&& ... args)" << endl ;
 return op(forward<Ts>(args)...) ;
}

template<class OP,typename... Ts>
auto CALL(OP op,vector<VarMat>& f, Ts&&... Args)
{
 cout << "CALL(OP op,vector<VarMat>& f, Ts&&... Args)" << endl ;
 vector<cv::Mat> collect ;

 for(int i = 0 ; i < (f.size()-1) ; i++)
 {
  collect.push_back( op(f[i],f[i+1],forward<Ts>(Args)...) ) ;
 }

 return collect ;
}

template<class OP,typename... Ts>
auto CALL(OP op,vector<cv::Mat>& f_, Ts&&... Args)
{
cout << "CALL(OP op,vector<cv::Mat>& f_, Ts&&... Args)" << endl ;
vector<VarMat> f(f_.size()) ;

 for(int i = 0 ; i < f.size() ; i++)
 {
  f[i] = op.toKerMat(f_[i]) ;
 }

vector<cv::Mat> collect(f.size()-1) ;

 for(int i = 0 ; i < (f.size()-1) ; i++)
 {
  collect[i] = op(f[i],f[i+1],forward<Ts>(Args)...) ;
 }

 return collect ;
}

} ;

using IMPL = OpticalFlow::IMPLEMENTATION ;

#include "include/implementation-listing.h"
IMPL::TABLE IMPL::t = __IMPLEMENTATION__ ;
#include "include/implementation-listing.h"

