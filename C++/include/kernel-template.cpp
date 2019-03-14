
struct __KERNEL__  // = =
//ruct CUDA_DualTVL1_dense
{

 using DICT = DICT_DEFAULT_t ;
//sing THIS = CUDA_DualTVL1_dense ;
 using THIS = __KERNEL__ ;

//ypedef ARGSPEC_t<DICT,tuple<double,double,double,int,int,double,int,double,double,bool>> ARGSPEC ;
 typedef ARGSPEC_t<DICT,tuple<__SIGNATURE__>> ARGSPEC ;

 ARGSPEC argspec ;

//ypedef cv::cuda::OpticalFlowDual_TVL1 KERNEL_t ;
//ypedef cv::cuda::GpuMat KerMat ;

 typedef __IMPLEMENTATION__ KERNEL_t ;
 typedef __KER_MAT__ KerMat ;

 cv::Ptr<KERNEL_t> Ker ;

//UDA_DualTVL1_dense()
 __KERNEL__()
  :
  argspec
  (
//  ARGSPEC::Sig_t (0.25,0.15,0.3,5,5,0.01,300,0.8,0.0,false)
//, ARGSPEC::VAR_t ("tau","lambda","theta","nscales","warps","epsilon","iterations","scaleStep","gamma","useInitialFlow")
    ARGSPEC::Sig_t (__DEFAULT__)
  , ARGSPEC::VAR_t (__VAR__)
  )
 {
  cur_opt = argspec.DEFAULT ;
 }

template<typename Sig>
auto slice(auto dict,auto key,bool verbose = false)
{
 return for_each_in_tuple4<Sig>(key, [&dict](const auto &key )->auto { return dict[key] ; },verbose);
}

 ARGSPEC::Sig_t cur_opt ;

 THIS& info()
 {
  cout << "----------------------------------------------------------------" << endl ;
  slice<ARGSPEC::Sig_t>(argspec.as_dict(cur_opt),argspec.VAR,true);
  return *this ;
 }

 THIS& kernel(DICT opt = DICT(),bool verbose=false)
 {
  if( not Ker.empty() ) Ker.release() ;

  if(verbose)

   cout << "----------------------------------------------------------------" << endl ;

  cur_opt = argspec.option(opt,verbose) ;

  Ker = apply(KERNEL_t::create,cur_opt) ;

  return *this ;
 }

// typedef variant<cv::Mat,cv::cuda::GpuMat> VarMat ;

 #if __H2D_CONVERSION__
 //KerMat   toKerMat(const cv::Mat& in, cv::cuda::Stream& s = cv::cuda::Stream::Null()) { KerMat _ ; _.upload(in,s) ; return _ ; }
 VarMat   toKerMat(const cv::Mat& in, cv::cuda::Stream& s = cv::cuda::Stream::Null()) { KerMat _ ; _.upload(in,s) ; return _ ; }
 //cv::InputArray   toKerMat(const cv::Mat& in, cv::cuda::Stream& s = cv::cuda::Stream::Null()) { KerMat _ ; _.upload(in,s) ; return cv::InputArray(_) ; }
 #else
 //KerMat   toKerMat(const cv::Mat& in) { return in ; }
 VarMat   toKerMat(const cv::Mat& in) { return in ; }
 //cv::InputArray   toKerMat(const cv::Mat& in) { return cv::InputArray(in) ; }
 #endif

 KerMat GEN_ResultHolder(cv::Size s) { return KerMat(s, CV_32FC2) ; }

 cv::Mat operator()(VarMat prev,VarMat next,               double& timeSec) // ===
 {
  const int64 start = cv::getTickCount();

  cv::Mat flow( get<KerMat>(prev).size(), CV_32FC2 ) ;

  Ker->calc(get<KerMat>(prev),get<KerMat>(next),flow) ;

  timeSec += (cv::getTickCount() - start) / cv::getTickFrequency();

  return flow ;
 }
///
 void    operator()(VarMat prev,VarMat next, cv::InputOutputArray flow, double& timeSec) // ===
 {
  const int64 start = cv::getTickCount();

  Ker->calc(get<KerMat>(prev),get<KerMat>(next),flow) ;

  timeSec = (cv::getTickCount() - start) / cv::getTickFrequency();
 }
///
/*
 cv::Mat operator()(const KerMat& prev,const KerMat& next,               double& timeSec) // ===
 {
  const int64 start = cv::getTickCount();

  cv::Mat flow( prev.size(), CV_32FC2 ) ;

  Ker->calc(prev,next,flow) ;

  timeSec += (cv::getTickCount() - start) / cv::getTickFrequency();

  return flow ;
 }
///
 void    operator()(const KerMat& prev,const KerMat& next, cv::InputOutputArray flow, double& timeSec) // ===
 {
  const int64 start = cv::getTickCount();

  Ker->calc(prev,next,flow) ;

  timeSec = (cv::getTickCount() - start) / cv::getTickFrequency();
 }
///
*/
 cv::Mat operator()(cv::InputArray prev,cv::InputArray next,               double& timeSec) // ===
 {
  const int64 start = cv::getTickCount();

  cout << "SIZE : " << prev.size() << endl ;

  cv::Mat flow( prev.size(), CV_32FC2 ) ;

  Ker->calc(prev,next,flow) ;

  timeSec += (cv::getTickCount() - start) / cv::getTickFrequency();

  return flow ;
 }
///
 void    operator()(cv::InputArray prev,cv::InputArray next, cv::InputOutputArray flow, double& timeSec) // ===
 {
  const int64 start = cv::getTickCount();

  Ker->calc(prev,next,flow) ;

  timeSec = (cv::getTickCount() - start) / cv::getTickFrequency();
 }
///
 
} ;

///

