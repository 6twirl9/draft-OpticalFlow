// === #include...
#include <iostream>
#include <fstream>
///

using namespace std ;

#include "include/contrib/misc.h"
#include "include/opticalflow.h"

int main(int argc, char*argv[])
{
// Conveniences
using IMPL = OpticalFlow::IMPLEMENTATION ;
using DICT = OpticalFlow::DICT ;

double elapsed = 0.;

// Load image data given file names specified on the command line

int n_images = argc -1 ;

vector<cv::Mat> frame(n_images) ;

 cout << "Received files ..." << endl ;

 loop(i,n_images)
 {
  cout << " ... " << argv[i+1] << endl ;

  frame[i] =

   cv::imread(string(argv[i+1]), cv::IMREAD_GRAYSCALE)

  ;
 }

 cout << "Loaded " << frame.size() << " image(s)." << endl ;

// Master dispatcher to actual implementations
OpticalFlow Q ;

/*
 * Select an implementation by specifying device, algorithm & feature.
 *
 * It is also possible to specify the implementation directly.
 *
 */
Q.algorithm("DualTVL1").device("CUDA").feature("dense") ;

 // ... alternatively
 Q.algorithm("DualTVL1") ;
 Q.device("CUDA") ;
 Q.feature("dense") ;

 // ... alternatively, which, of course, is harder to remember
 Q.implementation("cv::cuda::OpticalFlowDual_TVL1") ;

// customization of parameters
DICT opt ;

 opt =
 {
   { "iterations", 1000 }
 , { "warps"     ,    7 }
 } ;

  // alternatively
  opt["iterations"] = 1000 ;
  opt["warps"]      =    7 ;


/*
 * device: CUDA
 *
 * The invocation of 'kernel' automatically instantiates the interface class
 * to the OpenCV class that does the actual work besides overriding the
 * parameter "iterations".
 *
 *  Q.kernel(opt) ;
 *
 * It is also possible to explicitly load an implementation:
 *
 *  Q.load()
 *
 * Once interface to a particular implementation is loaded, it won't get loaded
 * again. However, each invocation of 'kernel' will create a new instance of the
 * OpenCV class that implements the algorithm -- and delete a previous one if it
 * exists.
 */

Q.device("CUDA").kernel(opt).info() ;

vector<VarMat> f(frame.size()) ;

loop(i,frame.size())

 f[i] =  Q.toKerMat(frame[i]) ;

{
elapsed = 0 ;
auto ret = Q(f[0],f[1],elapsed) ;   // specify images individually
cout << ret.size() << " : " << type2str(ret.type()) << " : " << type2size(ret.type()) << " : " << ret.isContinuous() << endl ;
cout << "Timing (CUDA) : " << elapsed << " // calc(image,image) -> flow[x,y]" << endl ;
}

{
elapsed = 0 ;
auto ret = Q(f,elapsed) ;           // takes a vector of images
cout << "Timing (CUDA) : " << elapsed << " // calc(vetor of image) -> vector of flow[x,y]" << endl ;
}

// device: CPU (same algorithm as before)

Q.device("CPU").kernel(opt).info() ;

loop(i,frame.size())

 f[i] = Q.toKerMat(frame[i]) ;

{
elapsed = 0 ;
auto foo = Q(f,elapsed) ;
cout << "Timing (CPU)  : " << elapsed << endl ;
}

return 0 ;
}


