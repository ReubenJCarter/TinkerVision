%module SwigInterface
%{
#include "Core/VectorMath.h"
#include "Core/BoundingBox.h"
#include "Core/Contour.h"
#include "Core/ImageType.h"
#include "Core/Image.h"
#include "Core/ImageGPU.h"
#include "Core/Context.h"
#include "Core/Window.h"

#include "IO/ImageFile.h"
#include "IO/VideoFile.h"
#include "IO/VideoHelper.h"

#include "Process/AdaptiveThreshold.h"
#include "Process/ApproxDistanceTransform.h"
#include "Process/AverageFilter.h"
#include "Process/BilateralFilter.h"
#include "Process/Blend.h"
#include "Process/BrightnessContrast.h"
#include "Process/CameraDistortion.h"
#include "Process/ChannelDemux.h"
#include "Process/ChannelMapper.h"
#include "Process/ClearColor.h"
#include "Process/CopyImage.h"
#include "Process/Crop.h"
#include "Process/Downsample.h"
#include "Process/FindContours.h"
#include "Process/GaussianBlur.h"
#include "Process/GaussianDerivative.h"
#include "Process/GrayScale.h"
#include "Process/HarrisShiTomasiResponse.h"
#include "Process/HighLowThreshold.h"
#include "Process/HSVToRGB.h"
#include "Process/HysteresisEdgeThreshold.h"
#include "Process/InRange.h"
#include "Process/Invert.h"
#include "Process/LocalMaxima.h"
#include "Process/MarkerBitExtract.h"
#include "Process/MarkerDictionary.h"
#include "Process/MedianFilter.h"
#include "Process/Mipmaps.h"
#include "Process/MorphRect.h"
#include "Process/NonMaximumEdgeSuppression.h"
#include "Process/Normalize.h"
#include "Process/Perspective3Point.h"
#include "Process/Renderer.h"
#include "Process/RGBToHSV.h"
#include "Process/Rotate.h"
#include "Process/Sobel.h"
#include "Process/StereoMatchSAD.h"
#include "Process/StructureTensor.h"
#include "Process/Thinning.h"
#include "Process/Threshold.h"
#include "Process/Translate.h"
#include "Process/Upsample.h"

#include "CompositeProcess/ARUCODetector.h"
#include "CompositeProcess/CannyEdgeDetect.h"
#include "CompositeProcess/CornerDetector.h"
#include "CompositeProcess/Resize.h"
#include "CompositeProcess/Sift.h"

%}

//put things into packages correctly and put the JNI class into the Viso package
%feature(nspace);//seems to generate nice namespace packages in java but also break everything when building 
%pragma(java) jniclasspackage="Viso";



//Solve the cPtr being protected problem
//https://stackoverflow.com/questions/24166297/swig-java-how-to-add-a-method-to-a-swigtype-class
//http://www.swig.org/Doc2.0/Java.html
//http://www.swig.org/Doc2.0/Java.html
SWIG_JAVABODY_PROXY(public, public, SWIGTYPE) 
SWIG_JAVABODY_TYPEWRAPPER(public, public, public, SWIGTYPE) 



//solves problems of not finding certain classes when using multiple packages
//http://swig.10945.n7.nabble.com/Using-multiple-modules-and-packages-in-Java-td12215.html
%typemap(javaimports) SWIGTYPE, SWIGTYPE * "import VisoJava.*;"
%pragma(java) jniclassimports="import VisoJava.*;"
%pragma(java) moduleimports="import VisoJava.*;"




#define VISO_EXPORT __declspec(dllimport)

%include "windows.i"
%include "std_string.i" 
%include "cpointer.i"

%include "std_vector.i"
namespace std{
    %template (IntVector) vector<int>;
    %template (ContourVector) vector<Viso::Contour>;
    %template (Vec2Vector) vector<Viso::Vec2>;
    %template (Vec3Vector) vector<Viso::Vec3>;
    %template (ImageVector) vector<Viso::Image>; 
    %template (CameraPoseVector) vector<Viso::CameraPose>; 
}

%include "Core/VectorMath.h"
%include "Core/BoundingBox.h"
%include "Core/Contour.h"
%include "Core/ImageType.h"
%include "Core/Image.h"
%include "Core/ImageGPU.h"
%include "Core/Context.h"
%include "Core/Window.h"

%include "IO/ImageFile.h"
%include "IO/VideoFile.h"
%include "IO/VideoHelper.h"

%include "Process/AdaptiveThreshold.h"
%include "Process/ApproxDistanceTransform.h"
%include "Process/AverageFilter.h"
%include "Process/BilateralFilter.h"
%include "Process/Blend.h"
%include "Process/BrightnessContrast.h"
%include "Process/CameraDistortion.h"
%include "Process/ChannelDemux.h"
%include "Process/ChannelMapper.h"
%include "Process/ClearColor.h"
%include "Process/CopyImage.h"
%include "Process/Crop.h"
%include "Process/Downsample.h"
%include "Process/FindContours.h"
%include "Process/GaussianBlur.h"
%include "Process/GaussianDerivative.h"
%include "Process/GrayScale.h"
%include "Process/HarrisShiTomasiResponse.h"
%include "Process/HighLowThreshold.h"
%include "Process/HSVToRGB.h"
%include "Process/HysteresisEdgeThreshold.h"
%include "Process/InRange.h"
%include "Process/Invert.h"
%include "Process/LocalMaxima.h"
%include "Process/MarkerBitExtract.h"
%include "Process/MarkerDictionary.h"
%include "Process/MedianFilter.h"
%include "Process/Mipmaps.h"
%include "Process/MorphRect.h"
%include "Process/NonMaximumEdgeSuppression.h"
%include "Process/Normalize.h"
%include "Process/Perspective3Point.h"
%include "Process/Renderer.h"
%include "Process/RGBToHSV.h"
%include "Process/Rotate.h"
%include "Process/Sobel.h"
%include "Process/StereoMatchSAD.h"
%include "Process/StructureTensor.h"
%include "Process/Thinning.h"
%include "Process/Threshold.h"
%include "Process/Translate.h"
%include "Process/Upsample.h"

%include "CompositeProcess/ARUCODetector.h"
%include "CompositeProcess/CannyEdgeDetect.h"
%include "CompositeProcess/CornerDetector.h"
%include "CompositeProcess/Resize.h"
%include "CompositeProcess/Sift.h"
