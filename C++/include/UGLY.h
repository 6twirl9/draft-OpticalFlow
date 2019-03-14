
// === Tuple to iterables

/*
template <class Tuple, class T = std::decay_t<std::tuple_element_t<0, std::decay_t<Tuple>>>>
std::vector<T> tuple2vector(Tuple&& tuple)
{
 return std::apply([](auto&&... elems){ return std::vector<T>{std::forward<decltype(elems)>(elems)...}; }, std::forward<Tuple>(tuple));
}

template<template<typename X> class S>
struct tuple2iter
{
 template <class Tuple, class T = std::decay_t<std::tuple_element_t<0, std::decay_t<Tuple>>>>
 static S<T> convert(Tuple&& tuple)
 {
  return std::apply([](auto&&... elems){ return S<T>{std::forward<decltype(elems)>(elems)...}; }, std::forward<Tuple>(tuple));
 }
} ;
*/

template <template<typename X> class S, class Tuple, class T = std::decay_t<std::tuple_element_t<0, std::decay_t<Tuple>>>>
static S<T> tuple2iter(Tuple&& tuple)
{
 return std::apply([](auto&&... elems){ return S<T>{std::forward<decltype(elems)>(elems)...}; }, std::forward<Tuple>(tuple));
}

/*
template <typename T, std::size_t... Indices>
auto vectorToTupleHelper(const std::vector<T>& v, std::index_sequence<Indices...>) {
  return std::make_tuple(v[Indices]...);
}

template <std::size_t N, typename T>
auto vectorToTuple(const std::vector<T>& v) {
  assert(v.size() >= N);
  return vectorToTupleHelper(v, std::make_index_sequence<N>());
}

template<template<typename X> class S>
struct iter2tuple
{
// Must have operator[] defined !
template <typename T, std::size_t... Indices>
static auto convert_(const S<T>& v, std::index_sequence<Indices...>) { return std::make_tuple(v[Indices]...); }

template <std::size_t N, typename T>
static auto convert(const S<T>& v) {
  assert(v.size() >= N);
  return convert_(v, std::make_index_sequence<N>());
}
} ;

template<>
struct iter2tuple<list>
{
template<typename X>
using indexable = std::vector<X> ;
template<typename X>
using S = std::list<X> ;
// Must have operator[] defined !
template <typename T, std::size_t... Indices>
static auto convert_(const indexable<T>& v, std::index_sequence<Indices...>) { return std::make_tuple(v[Indices]...); }

template <std::size_t N, typename T>
static auto convert(const S<T>& v) {
  assert(v.size() >= N);
  indexable<T> v_ { std::begin(v), std::end(v) } ;
  return convert_(v_, std::make_index_sequence<N>());
}
} ;
*/

// Must have operator[] defined !
template <typename T, std::size_t... Indices>
static auto iter2tuple_(const T& v, std::index_sequence<Indices...>) { return std::make_tuple(v[Indices]...); }

template <std::size_t N,typename T>
static auto iter2tuple(const T& v) {
  assert(v.size() >= N);
  return iter2tuple_(v, std::make_index_sequence<N>());
}

template <std::size_t N,typename T>
static auto iter2tuple(const list<T>& v) {
  assert(v.size() >= N);
  std::vector<T> v_ { std::begin(v), std::end(v) } ;
  return iter2tuple_(v_, std::make_index_sequence<N>());
}


///

// === DICT of DICT of ...

template<typename... T>
struct DICT_t ;

template<typename... T>
struct UNION_t : public variant<DICT_t<T...>*,T...>
{
 typedef variant<DICT_t<T...>*,T...> Type ;
 typedef DICT_t<T...> DICT ;
// Type __ ;
// UNION_t() {}
 typedef variant<DICT_t<T...>*,T...> baseType ;
//UNION_t(initializer_list<typename baseType::value_type> _) { baseType& that = *this ; that = _ ; }
// UNION_t() {}
// UNION_t(auto _) { __ = _ ; }
using baseType::variant ;
 template<typename X>
 operator X() const { return get<X>(*this) ; }
 operator DICT() const { return *get<DICT *>(*this) ; }
// int index() { return __.index() ; }
// void operator=(auto _) { ((Type*)this)->operator=(_) ; }
} ;

template<typename... T>
struct DICT_t : public unordered_map<string,UNION_t<T...>>
{
//unordered_map<string,UNION_t<T...>> dict ;
typedef unordered_map<string,UNION_t<T...>> Type ;
typedef UNION_t<T...> VALUE_t ;
typedef DICT_t<T...> DICT ;

typedef unordered_map<string,UNION_t<T...>> baseType ;
//DICT_t(initializer_list<typename baseType::value_type> _) { baseType& that = *this ; that = _ ; }
//DICT_t() {}
using baseType::unordered_map ;

//auto operator[](string key) { return dict[key] ; }
//template<typename ... S>
//auto& operator[](S&& ... s) { return dict.operator[](forward<S>(s) ...) ; }
//auto operator=(initializer_list<VALUE_t>&& l) { ((Type*)this)->operator=(l) ; }
//auto operator=(initializer_list<VALUE_t>&& l) { *(Type*)this=l ; }
//auto operator=(initializer_list<VALUE_t> l) { *(Type*)this=l ; }
auto _string(auto key) { return string((*this)[key]) ; }
auto _bool  (auto key) { return   bool((*this)[key]) ; }
auto _double(auto key) { return double((*this)[key]) ; }
auto _float (auto key) { return  float((*this)[key]) ; }
auto _int   (auto key) { return    int((*this)[key]) ; }
auto _DICT  (auto key) { return   DICT((*this)[key]) ; }
} ;

typedef DICT_t<cv::Size,string,bool,char,int,float,double> DICT_DEFAULT_t ;

///

// === ARGSPEC
template<size_t, class T>
using gen_tuple_type_T_expander = T;

template<class T, size_t... Is>
auto gen_tuple_type(std::index_sequence<Is...>)
{
 return std::tuple<gen_tuple_type_T_expander<Is, T>...>{};
}

template<class T, size_t N>
auto gen_tuple_type() { return gen_tuple_type<T>(std::make_index_sequence<N>{}); }

template<typename DICT, typename Signature>
struct ARGSPEC_t
{
typedef Signature Sig_t ;
typedef DICT      Dict_t ;
static constexpr int N = tuple_size<Sig_t>::value ;
typedef decltype(gen_tuple_type<string,N>()) VAR_t ;
Sig_t DEFAULT, OPT ;
VAR_t VAR ;

ARGSPEC_t(Sig_t _0, VAR_t _1) : DEFAULT(_0), VAR(_1) { }

DICT load_dict()
{
 DICT dict ;
 for_each_in_tuple2(VAR,DEFAULT, [&dict](const auto &key, const auto &value) { dict[key] = value ; });
 return dict ;
}
DICT as_dict(Sig_t opt)
{
 DICT dict ;
 for_each_in_tuple2(VAR,opt, [&dict](const auto &key, const auto &value) { dict[key] = value ; });
 return dict ;
}

DICT& prune(DICT& opt,DICT& dflt)               // ===
{
 set<string> _0, _1, _X ;
 for( auto it = opt.begin(); it != opt.end() ; it++ ) _0.insert(it->first) ;
 for( auto it = dflt.begin(); it != dflt.end() ; it++ ) _1.insert(it->first) ;
 set_difference(_0.begin(),_0.end(),_1.begin(),_1.end(),inserter(_X,_X.begin())) ;

 for(auto it = _X.begin() ; it != _X.end() ; it ++ )
 {
  opt.erase(*it) ;
 }

 return opt ;
}
///
Sig_t option(DICT& opt,bool verbose = false)    // ===
{
 DICT dflt = load_dict() ;

 prune(opt,dflt) ;

 opt.insert(dflt.begin(),dflt.end()) ;

 return for_each_in_tuple3(VAR,DEFAULT,Sig_t(), [&opt](const auto &x, const auto &y)->auto { return opt[x] ; },verbose);
}
///
} ;

///

// === type_name
template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}
///
template<typename T>
string_view CXX_TYPE() { return type_name<decltype(T())>() ; } ;
// === Experimental

template<class F, class...Ts, std::size_t...Is>
void for_each_in_tuple(const std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>){
 using expander = int[];
 (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template<class F, class...Ts>
void for_each_in_tuple(const std::tuple<Ts...> & tuple, F func){
 for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
}

template<class F, class...Ts0, class...Ts1, std::size_t...Is>
void for_each_in_tuple2(const std::tuple<Ts0...> & tuple0, const std::tuple<Ts1...> & tuple1, F func, std::index_sequence<Is...>){
 using expander = int[];
 (void) expander { ((void)func(std::get<Is>(tuple0),std::get<Is>(tuple1)),static_cast<int>(Is) )... };
}

template<class F, class...Ts0, class...Ts1>
void for_each_in_tuple2(const std::tuple<Ts0...> & tuple0, const std::tuple<Ts1...> & tuple1, F func){
 for_each_in_tuple2(tuple0, tuple1, func, std::make_index_sequence<sizeof...(Ts0)>());
}

template<int N,typename T,class S>
auto FOO(T a, string var, S b, bool verbose)
{
using Z = typename tuple_element<N,T>::type ;
//cout << type_name<decltype(Z())>() << " : " << type_name<decltype(b)>() << " : " << N << " : " << type_name<decltype(Z())>() << " : " << (Z)b << endl ;
if(verbose)
cout << setw(2) << N << " : " << std::setw(16) << var << " : " << setw(16) << (Z)b << " (" << type_name<decltype(Z())>() << ")" << endl ;
return (Z)b ;
}

template<int N,typename T,class S>
auto FOO1(string var, S b, bool verbose)
{
using Z = typename tuple_element<N,T>::type ;
//cout << type_name<decltype(Z())>() << " : " << type_name<decltype(b)>() << " : " << N << " : " << type_name<decltype(Z())>() << " : " << (Z)b << endl ;
if(verbose)
 cout << setw(2) << N << " : " << std::setw(16) << var << " : " << setw(48) << (Z)b << " (" << type_name<decltype(Z())>() << ")" << endl ;
return (Z)b ;
}

template<class F, class...Ts0, class...Ts1, typename Ts2, std::size_t...Is>
auto for_each_in_tuple3(const std::tuple<Ts0...> & tuple0, const std::tuple<Ts1...> & tuple1, Ts2 tuple2, F func, std::index_sequence<Is...>,bool verbose = false){
 auto ret = Ts2 ( FOO<Is>(tuple2,std::get<Is>(tuple0),func(std::get<Is>(tuple0),std::get<Is>(tuple1)),verbose) ... );
 return ret ;
}

template<class F, class...Ts0, class...Ts1, typename Ts2>
auto for_each_in_tuple3(const std::tuple<Ts0...> & tuple0, const std::tuple<Ts1...> & tuple1, Ts2 tuple2, F func, bool verbose = false){
 return for_each_in_tuple3(tuple0, tuple1, tuple2, func, std::make_index_sequence<sizeof...(Ts0)>(),verbose);
}

template<typename Ts1, class F, class...Ts0, std::size_t...Is>
auto for_each_in_tuple4(const std::tuple<Ts0...> & tuple0, F func, std::index_sequence<Is...>,bool verbose = false){
 auto ret = Ts1 ( FOO1<Is,Ts1>(std::get<Is>(tuple0),func(std::get<Is>(tuple0)),verbose) ... );
 return ret ;
}

template<typename Ts1, class F, class...Ts0>
auto for_each_in_tuple4(const std::tuple<Ts0...> & tuple0, F func, bool verbose = false){
 return for_each_in_tuple4<Ts1>(tuple0, func, std::make_index_sequence<sizeof...(Ts0)>(),verbose);
}

template<typename Ts1, class F, class...Ts0, std::size_t...Is>
auto for_each_in_tuple5(const std::tuple<Ts0...> & tuple0, F func, std::index_sequence<Is...>,bool verbose = false){
 auto ret = func ( FOO1<Is,Ts1>(std::get<Is>(tuple0),verbose) ... );
 return ret ;
}

template<typename Ts1, class F, class...Ts0>
auto for_each_in_tuple5(F func, const std::tuple<Ts0...> & tuple0, bool verbose = false){
 return for_each_in_tuple5<Ts1>(tuple0, func, std::make_index_sequence<sizeof...(Ts0)>(),verbose);
}

///
// === tuple <-> dict
/*
template<class T>
auto dict2tuple(auto opt,T& O)
{
 using ARGSPEC_TYPE_t = typename T::ARGSPEC_TYPE_t ;
 return for_each_in_tuple3(O.VAR,O.DEFAULT,ARGSPEC_TYPE_t(), [&opt](const auto &x, const auto &y)->auto { return opt[x] ; });
}

auto tuple2dict(auto& opt,auto& O)
{
 return for_each_in_tuple2(O.VAR,O.DEFAULT, [&opt](const auto &x, const auto &y) { opt[x] = y ; });
}
*/
///


/*
struct PyTool
{
using DICT = DICT_DEFAULT_t ;

static DICT fromPyDict(PyObject *_0)
{
PyObject *key, *value;
Py_ssize_t pos = 0;
DICT _1 ;

 while (PyDict_Next(_0, &pos, &key, &value))
 {
  _1[string(PyString_AsString(key))] = 
 }
}

}
*/

string type2str(int type)	// ===
{
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r =   "8U"; break;
    case CV_8S:  r =   "8S"; break;
    case CV_16U: r =  "16U"; break;
    case CV_16S: r =  "16S"; break;
    case CV_32S: r =  "32S"; break;
    case CV_32F: r =  "32F"; break;
    case CV_64F: r =  "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
///
int    type2size(int type)	// ===
{
  int r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = 1; break;
    case CV_8S:  r = 1; break;
    case CV_16U: r = 2; break;
    case CV_16S: r = 2; break;
    case CV_32S: r = 4; break;
    case CV_32F: r = 4; break;
    case CV_64F: r = 8; break;
    default:     r = 0; break;
  }

  return r * chans;
}
///

void dump(const cv::Mat& m, const string& file)         // ===
{
ofstream fout ;
fout.open(file,ios::binary|ios::out) ;
fout.write(m.ptr<char>(0),m.size().area()*type2size(m.type())) ;
fout.close();
}
///
void dump(const vector<cv::Mat>& m, const string& file) // ===
{
ofstream fout ;
fout.open(file,ios::binary|ios::out) ;
fout.write(m[0].ptr<char>(0),m.size()*m[0].size().area()*type2size(m[0].type())) ;
fout.close();
}
///

