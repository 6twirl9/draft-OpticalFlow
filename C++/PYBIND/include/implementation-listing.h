#ifndef __IMPLEMENTATION__
#define __IMPLEMENTATION__ \
{\
  {"CUDA",      "DualTVL1", "dense",    "cv::cuda::OpticalFlowDual_TVL1",               "CUDA_DualTVL1_dense"}\
, {"CUDA",          "Brox", "dense",         "cv::cuda::BroxOpticalFlow",                   "CUDA_Brox_dense"}\
, {"CUDA",         "PyrLK", "dense",   "cv::cuda::DensePyrLKOpticalFlow",                  "CUDA_PyrLK_dense"}\
, {"CUDA",     "Farneback", "dense",    "cv::cuda::FarnebackOpticalFlow",              "CUDA_Farneback_dense"}\
, { "CPU",      "DualTVL1", "dense",           "cv::DualTVL1OpticalFlow",                "CPU_DualTVL1_dense"}\
, { "CPU",     "Farneback", "dense",          "cv::FarnebackOpticalFlow",               "CPU_Farneback_dense"}\
}
#else
#undef __IMPLEMENTATION__
#endif
