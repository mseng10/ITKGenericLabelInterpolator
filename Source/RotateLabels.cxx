#include <itkImage.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLabelImageGaussianInterpolateImageFunction.h>
#include <itkBSplineInterpolateImageFunction.h>
#include "itkLabelImageGenericInterpolateImageFunction.h"
#include <itkResampleImageFilter.h>
#include <itkVersorRigid3DTransform.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include "itkLabelSelectionAdaptor.h"
#include <iostream>
#include <string>

/* This demo is a torture test for interpolators: it takes an input label map,
 * rotates it one full turn in ten steps, and writes the result */

// The BSpline interpolator has more arguments than other interpolators, so we set the additional parameter to the default value
using namespace std;
template<class TImage,typename TCoordRep> class BSplineInterpolator : public itk::BSplineInterpolateImageFunction<TImage,TCoordRep> {};
 
int main(int argc, char *argv[])
{
	string outputFilename = "resampled.mha";
	string inputFilename = "classif.mha";

	typedef unsigned char     PixelType;
	const   unsigned int    Dimension = 3;
	typedef itk::Image< PixelType, Dimension >  ImageType;

	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer r = ReaderType::New();
	r->SetFileName(inputFilename);
	r->Update();

	typedef itk::VersorRigid3DTransform<double> TransformType;
	TransformType::AxisType axis;
	axis[0]=0;
	axis[1]=1;
	axis[2]=0;
	TransformType::InputPointType center;
	center[0] = 144;
	center[1] = 86;
	center[2] = 101;
	const unsigned number_of_rotations=10;
	TransformType::Pointer rot = TransformType::New();
	rot->SetCenter(center);
	rot->SetRotation(axis,2.*M_PI/number_of_rotations);

	//typedef itk::LabelImageGenericInterpolateImageFunction<ImageType,itk::NearestNeighborInterpolateImageFunction> InterpolatorType;
	typedef itk::LabelImageGenericInterpolateImageFunction<ImageType,itk::LinearInterpolateImageFunction> InterpolatorType;
	//typedef itk::LabelImageGenericInterpolateImageFunction<ImageType,BSplineInterpolator> InterpolatorType;
	//typedef itk::LabelImageGaussianInterpolateImageFunction<ImageType,double> InterpolatorType;
	//typedef itk::NearestNeighborInterpolateImageFunction<ImageType,double> InterpolatorType;
	//typedef itk::LinearInterpolateImageFunction<ImageType,double> InterpolatorType;
	typedef itk::ResampleImageFilter<ImageType,ImageType> ResampleFilterType;


	ResampleFilterType::Pointer rs = ResampleFilterType::New();
	rs->SetInput(r->GetOutput());
	rs->SetReferenceImage( r->GetOutput() );
	rs->SetUseReferenceImage(true);
	rs->SetTransform(rot);
	InterpolatorType::Pointer interp = InterpolatorType::New();
	//interp->SetSigma(0.3); // Used for the gaussian interpolator
	rs->SetInterpolator(interp);
	ImageType::Pointer out ;
	for (unsigned i = 0; i<number_of_rotations; ++i) {
		rs->SetReferenceImage( r->GetOutput() );
		rs->SetUseReferenceImage(true);
		rs->Update();
		out = rs->GetOutput();
		out->DisconnectPipeline();
		rs->SetInput(out);
		rs->SetTransform(rot);
	}

	typedef  itk::ImageFileWriter< ImageType  > WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(outputFilename);
	writer->SetInput(out);
	writer->SetUseCompression(true);
	writer->Write();

	return EXIT_SUCCESS;
}
