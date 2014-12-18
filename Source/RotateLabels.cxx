#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <itkImage.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLabelImageGaussianInterpolateImageFunction.h>
#include <itkBSplineInterpolateImageFunction.h>
#include <itkResampleImageFilter.h>
#include <itkVersorRigid3DTransform.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkTestingComparisonImageFilter.h>
#include <itkTimeProbe.h>

#include "itkLabelImageGenericInterpolateImageFunction.h"
#include "itkLabelSelectionAdaptor.h"

/* This demo is a torture test for interpolators: it takes an input label map,
 * rotates it one full turn in ten steps, and writes the result */

template <class ImageType>
void
RotateNTimes(typename ImageType::Pointer input, 
    typename itk::InterpolateImageFunction<ImageType,double>* interpolator,
    unsigned int number_of_rotations, std::string filename_prefix) {
	typedef itk::VersorRigid3DTransform<double> TransformType;
	typedef itk::ResampleImageFilter<ImageType,ImageType> ResampleFilterType;
	TransformType::AxisType axis;
	axis[0]=0;
	axis[1]=1;
	axis[2]=0;
	TransformType::InputPointType center;
	center[0] = 144;
	center[1] = 86;
	center[2] = 101;
	TransformType::Pointer rot = TransformType::New();
	rot->SetCenter(center);
	rot->SetRotation(axis,2.*M_PI/number_of_rotations);

	typename ResampleFilterType::Pointer rs = ResampleFilterType::New();
	rs->SetInput( input );
	rs->SetReferenceImage( input );
	rs->SetUseReferenceImage(true);
	rs->SetTransform(rot);
	rs->SetInterpolator(interpolator);
	typename ImageType::Pointer out ;
  itk::TimeProbe timer; timer.Start();
	for (unsigned i = 0; i<number_of_rotations; ++i) {
		rs->SetReferenceImage( input );
		rs->SetUseReferenceImage(true);
		rs->Update();
		out = rs->GetOutput();
		out->DisconnectPipeline();
		rs->SetInput(out);
		rs->SetTransform(rot);
	}
  timer.Stop();
  typedef itk::Testing::ComparisonImageFilter<ImageType,ImageType> ComparisonFilterType;
  typename ComparisonFilterType::Pointer compare = ComparisonFilterType::New();
  compare->SetValidInput( input );
  compare->SetTestInput( out );
  compare->Update();
  std::cout << "Pixels with differences: " << std::setw(8) << compare->GetNumberOfPixelsWithDifferences() << 
    " ( " << std::fixed << std::setprecision(2) << static_cast<double>(compare->GetNumberOfPixelsWithDifferences()) /
    input->GetLargestPossibleRegion().GetNumberOfPixels() * 100.<< 
    "% ) in " << timer.GetTotal() << "s" << std::endl;
	typedef  itk::ImageFileWriter< ImageType  > WriterType;
	typename WriterType::Pointer writer = WriterType::New();
	writer->SetUseCompression(true);
	writer->SetInput( out );
  std::ostringstream fname_stream;
  fname_stream << "output/" << filename_prefix << "_" << number_of_rotations << ".mha";
	writer->SetFileName(fname_stream.str());
	writer->Write();
}


// The BSpline interpolator has more arguments than other interpolators, so we set the additional parameter to the default value
using namespace std;
template<class TImage,typename TCoordRep> class BSplineInterpolator : public itk::BSplineInterpolateImageFunction<TImage,TCoordRep> {};
 
int main(int argc, char *argv[])
{
	string inputFilename = "../Source/classification.mha";

	typedef unsigned char     PixelType;
	const   unsigned int    Dimension = 3;
	typedef itk::Image< PixelType, Dimension >  ImageType;

	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer r = ReaderType::New();
	r->SetFileName(inputFilename);
	r->Update();

	for( unsigned number_of_rotations=3; number_of_rotations <=7;
      number_of_rotations+=2 ) {
    std::cout << "Testing with " << number_of_rotations << " rotations..." << std::endl << std::endl;

    std::cout << "Nearest neighbor interpolator...                        " << std::flush;
    typedef itk::NearestNeighborInterpolateImageFunction<ImageType,double> NNInterpolatorType;
    NNInterpolatorType::Pointer nn_interp = NNInterpolatorType::New();
    RotateNTimes<ImageType>(r->GetOutput(), nn_interp, number_of_rotations, "nearest");

    std::cout << "Linear interpolator...                                  " << std::flush;
    typedef itk::LinearInterpolateImageFunction<ImageType,double> LInterpolatorType;
    LInterpolatorType::Pointer l_interp = LInterpolatorType::New();
    RotateNTimes<ImageType>(r->GetOutput(), l_interp, number_of_rotations, "linear");

    std::cout << "Label Gaussian interpolator type...                     " << std::flush;
    typedef itk::LabelImageGaussianInterpolateImageFunction<ImageType,double> LGInterpolatorType;
    LGInterpolatorType::Pointer lg_interp = LGInterpolatorType::New();
    lg_interp->SetSigma(0.3);
    RotateNTimes<ImageType>(r->GetOutput(), lg_interp, number_of_rotations, "label_gaussian");

    std::cout << "Generic label interpolator with nearest neighbor...     " << std::flush;
    typedef itk::LabelImageGenericInterpolateImageFunction<ImageType,itk::NearestNeighborInterpolateImageFunction> GNNInterpolatorType;
    GNNInterpolatorType::Pointer gnn_interp = GNNInterpolatorType::New();
    RotateNTimes<ImageType>(r->GetOutput(), gnn_interp, number_of_rotations, "gl_nearest");

    std::cout << "Generic label interpolator with linear interpolation... " << std::flush;
    typedef itk::LabelImageGenericInterpolateImageFunction<ImageType,itk::LinearInterpolateImageFunction> GLInterpolatorType;
    GLInterpolatorType::Pointer gl_interp = GLInterpolatorType::New();
    RotateNTimes<ImageType>(r->GetOutput(), gl_interp, number_of_rotations, "gl_linear");

    std::cout << "Generic label interpolator with B-Spline interpolation...             " << std::flush;
    typedef itk::LabelImageGenericInterpolateImageFunction<ImageType,BSplineInterpolator> GBSInterpolatorType;
    GBSInterpolatorType::Pointer gbs_interp = GBSInterpolatorType::New();
    RotateNTimes<ImageType>(r->GetOutput(), gbs_interp, number_of_rotations, "gl_bspline");

    std::cout << "Generic label interpolator with Gaussian interpolation...             " << std::flush;
    typedef itk::LabelImageGenericInterpolateImageFunction<ImageType,itk::GaussianInterpolateImageFunction> GGInterpolatorType;
    GGInterpolatorType::Pointer gg_interp = GGInterpolatorType::New();
    RotateNTimes<ImageType>(r->GetOutput(), gg_interp, number_of_rotations, "gl_gaussian");

    std::cout << std::endl;
  }


	return EXIT_SUCCESS;
}
