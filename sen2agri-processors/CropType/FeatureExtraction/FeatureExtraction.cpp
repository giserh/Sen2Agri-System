/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {reference polygons}, {sample ratio}
//    OUTPUTS: {training polygons}, {validation_polygons}
//  Software Guide : EndCommandLineArgs


//  Software Guide : BeginLatex
// The sample selection consists in splitting the reference data into 2 disjoint sets, the training set and the validation set.
// These sets are composed of polygons, not individual pixels.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbVectorImage.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbBandMathImageFilter.h"

typedef otb::VectorImage<float, 2>  ImageType;
typedef otb::Image<float, 2>        InternalImageType;
typedef otb::ImageFileReader<ImageType>                                   ReaderType;
typedef otb::ImageFileWriter<ImageType>                                   WriterType;
typedef otb::ImageList<InternalImageType>                                 ImageListType;
typedef otb::VectorImageToImageListFilter<ImageType, ImageListType>       VectorImageToImageListType;
typedef otb::ImageListToVectorImageFilter<ImageListType, ImageType>       ImageListToVectorImageFilterType;
typedef otb::BandMathImageFilter<InternalImageType>                       BandMathImageFilterType;
typedef otb::ObjectList<BandMathImageFilterType>                          BandMathImageFilterListType;

//  Software Guide : EndCodeSnippet

namespace otb
{

//  Software Guide : BeginLatex
//  Application class is defined in Wrapper namespace.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
namespace Wrapper
{
//  Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  SampleSelection class is derived from Application class.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
class FeatureExtraction : public Application
//  Software Guide : EndCodeSnippet
{
public:
  //  Software Guide : BeginLatex
  // The \code{ITK} public types for the class, the superclass and smart pointers.
  // Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef FeatureExtraction Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  Invoke the macros necessary to respect ITK object factory mechanisms.
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  itkNewMacro(Self)
;

  itkTypeMacro(FeatureExtraction, otb::Application)
;
  //  Software Guide : EndCodeSnippet


private:

  //  Software Guide : BeginLatex
  //  \code{DoInit()} method contains class information and description, parameter set up, and example values.
  //  Software Guide : EndLatex

  void DoInit()
  {

    // Software Guide : BeginLatex
    // Application name and description are set using following methods :
    // \begin{description}
    // \item[\code{SetName()}] Name of the application.
    // \item[\code{SetDescription()}] Set the short description of the class.
    // \item[\code{SetDocName()}] Set long name of the application (that can be displayed \dots).
    // \item[\code{SetDocLongDescription()}] This methods is used to describe the class.
    // \item[\code{SetDocLimitations()}] Set known limitations (threading, invalid pixel type \dots) or bugs.
    // \item[\code{SetDocAuthors()}] Set the application Authors. Author List. Format : "John Doe, Winnie the Pooh" \dots
    // \item[\code{SetDocSeeAlso()}] If the application is related to one another, it can be mentioned.
    // \end{description}
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
      SetName("FeatureExtraction");
      SetDescription("The feature extraction step produces the relevant features for the classication.");

      SetDocName("FeatureExtraction");
      SetDocLongDescription("The feature extraction step produces the relevant features for the classication. The features are computed"
                            "for each date of the resampled and gaplled time series and concatenated together into a single multi-channel"
                            "image file. The selected features are the surface reflectances, the NDVI, the NDWI and the brightness.");
      SetDocLimitations("None");
      SetDocAuthors("LBU");
      SetDocSeeAlso(" ");
    //  Software Guide : EndCodeSnippet


    // Software Guide : BeginLatex
    // \code{AddDocTag()} method categorize the application using relevant tags.
    // \code{Code/ApplicationEngine/otbWrapperTags.h} contains some predefined tags defined in \code{Tags} namespace.
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    AddDocTag(Tags::Vector);
    //  Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    // The input parameters:
    // - ref: Vector file containing reference data
    // - ratio: Ratio between the number of training and validation polygons per class (dafault: 0.75)
    // The output parameters:
    // - tp: Vector file containing reference data for training
    // - vp: Vector file containing reference data for validation
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    AddParameter(ParameterType_InputImage, "rtocr", "Resampled S2 L2A surface reflectances");
    AddParameter(ParameterType_Int, "mode", "What files are produced ");
    SetDefaultParameterInt("mode", 0);

    AddParameter(ParameterType_OutputImage, "fts", "Feature time series");
    MandatoryOff("fts");
    AddParameter(ParameterType_OutputImage, "ndvi", "NDVI time series");
    MandatoryOff("ndvi");
    AddParameter(ParameterType_OutputImage, "ndwi", "NDWI time series");
    MandatoryOff("ndwi");
    AddParameter(ParameterType_OutputImage, "brightness", "Brightness time series");
    MandatoryOff("brightness");

     //  Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    // An example commandline is automatically generated. Method \code{SetDocExampleParameterValue()} is
    // used to set parameters. Dataset should be located in  \code{OTB-Data/Examples} directory.
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    SetDocExampleParameterValue("rtocr", "reflectances.tif");
    SetDocExampleParameterValue("fts", "feature-time-series.tif");
    //  Software Guide : EndCodeSnippet
  }

  // Software Guide : BeginLatex
  // \code{DoUpdateParameters()} is called as soon as a parameter value change. Section \ref{sec:appDoUpdateParameters}
  // gives a complete description of this method.
  // Software Guide : EndLatex
  //  Software Guide :BeginCodeSnippet
  void DoUpdateParameters()
  {
      int mode = GetParameterInt("mode");

      // verify what parameters are set
      if(mode == 0) {
          m_FullSeries = true;
          m_ftsConcat = ImageListToVectorImageFilterType::New();
      } else {
          m_FullSeries = false;
          m_ndviConcat = ImageListToVectorImageFilterType::New();
          m_ndwiConcat = ImageListToVectorImageFilterType::New();
          m_brightnessConcat = ImageListToVectorImageFilterType::New();
      }
      m_imgReader = ReaderType::New();
      m_imgSplit = VectorImageToImageListType::New();
      m_bandMathFilterList = BandMathImageFilterListType::New();

      bandsPerImage = 4;

      // The expressions correspond to the case where a list of images is passes to the BandMath filter
      // The significance of the bands is:
      // b1 - G
      // b2 - R
      // b3 - NIR
      // b4 - SWIR
#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
          ndviExpr = "(b3==-10000) ? -10000 : (abs(b3+b2)<0.000001) ? 0 : (b3-b2)/(b3+b2)";
          ndwiExpr = "(b3==-10000) ? -10000 : (abs(b4+b3)<0.000001) ? 0 : (b4-b3)/(b4+b3)";
          brightnessExpr = "(b1==-10000) ? -10000 : sqrt((b1 * b1) + (b2 * b2) + (b3 * b3) + (b4 * b4))";
#else
          ndviExpr = "if(b3==-10000,-10000,if(abs(b3+b2)<0.000001,0,(b3-b2)/(b3+b2)";
          ndwiExpr = "if(b3==-10000,-10000,if(abs(b4+b3)<0.000001,0,(b4-b3)/(b4+b3)";
          brightnessExpr = "if(b1==-10000,-10000,sqrt((b1 * b1) + (b2 * b2) + (b3 * b3) + (b4 * b4))";
#endif

  }
  //  Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The algorithm consists in a random sampling without replacement of the polygons of each class with
  // probability p = sample_ratio value for the training set and
  // 1 - p for the validation set.
  // Software Guide : EndLatex
  //  Software Guide :BeginCodeSnippet
  void DoExecute()
  {
      // define all needed types

      //Read all input parameters
      m_imgReader->SetFileName(GetParameterString("rtocr"));

      m_imgReader->UpdateOutputInformation();

      // Use a filter to split the input into a list of one band images
      m_imgSplit->SetInput(m_imgReader->GetOutput());

      m_imgSplit->UpdateOutputInformation();

      int nbBands = m_imgReader->GetOutput()->GetNumberOfComponentsPerPixel();
      // compute the number of images
      int nbImages = nbBands / bandsPerImage;

      //Create the image list used at the final concatenation step
      ImageListType::Pointer ftsList = ImageListType::New();
      ImageListType::Pointer ndviList = ImageListType::New();
      ImageListType::Pointer ndwiList = ImageListType::New();
      ImageListType::Pointer brightnessList = ImageListType::New();

      //loop through all images
      for (int i = 0; i < nbImages; i++) {
          // Create the ndvi bandmath filter
          BandMathImageFilterType::Pointer ndviFilter = BandMathImageFilterType::New();
          // Create the ndwi bandmath filter
          BandMathImageFilterType::Pointer ndwiFilter = BandMathImageFilterType::New();
          // Create the brightness bandmath filter
          BandMathImageFilterType::Pointer brightnessFilter = BandMathImageFilterType::New();

          for (int j = 0; j < bandsPerImage; j++) {
            // Set all bands of the current image as inputs
            ndviFilter->SetNthInput(j, m_imgSplit->GetOutput()->GetNthElement(i * bandsPerImage + j));
            ndwiFilter->SetNthInput(j, m_imgSplit->GetOutput()->GetNthElement(i * bandsPerImage + j));
            brightnessFilter->SetNthInput(j, m_imgSplit->GetOutput()->GetNthElement(i * bandsPerImage + j));

            // add the band to the list
            if (m_FullSeries) {
                ftsList->PushBack(m_imgSplit->GetOutput()->GetNthElement(i * bandsPerImage + j));
            }
          }

          // set the expressions
          ndviFilter->SetExpression(ndviExpr);
          ndwiFilter->SetExpression(ndwiExpr);
          brightnessFilter->SetExpression(brightnessExpr);

          // add the outputs of the filters to the list
          if (m_FullSeries) {
              ftsList->PushBack(ndviFilter->GetOutput());
              ftsList->PushBack(ndwiFilter->GetOutput());
              ftsList->PushBack(brightnessFilter->GetOutput());
          } else {
              ndviList->PushBack(ndviFilter->GetOutput());
              ndwiList->PushBack(ndwiFilter->GetOutput());
              brightnessList->PushBack(brightnessFilter->GetOutput());
          }


          //add all filters to the list
          m_bandMathFilterList->PushBack(ndviFilter);
          m_bandMathFilterList->PushBack(ndwiFilter);
          m_bandMathFilterList->PushBack(brightnessFilter);
      }


      if (m_FullSeries) {
          // add the list as input to the concat filter
          m_ftsConcat->SetInput(ftsList);
          SetParameterOutputImage("fts", m_ftsConcat->GetOutput());
      } else {
          m_ndviConcat->SetInput(ndviList);
          m_ndwiConcat->SetInput(ndwiList);
          m_brightnessConcat->SetInput(brightnessList);
          SetParameterOutputImage("ndvi", m_ndviConcat->GetOutput());
          SetParameterOutputImage("ndwi", m_ndwiConcat->GetOutput());
          SetParameterOutputImage("brightness", m_brightnessConcat->GetOutput());
      }
  }
  //  Software Guide :EndCodeSnippet

  // The number of bands per image
  int bandsPerImage;
  // The BandMath expression for NDVI
  std::string ndviExpr;
  // The BandMath expression for NDWI
  std::string ndwiExpr;
  // The BandMath expression for Brigthness
  std::string brightnessExpr;

  ReaderType::Pointer                       m_imgReader;
  VectorImageToImageListType::Pointer       m_imgSplit;
  BandMathImageFilterListType::Pointer      m_bandMathFilterList;
  ImageListToVectorImageFilterType::Pointer m_ftsConcat;
  ImageListToVectorImageFilterType::Pointer m_ndviConcat;
  ImageListToVectorImageFilterType::Pointer m_ndwiConcat;
  ImageListToVectorImageFilterType::Pointer m_brightnessConcat;
  bool                                      m_FullSeries;
};
}
}

// Software Guide : BeginLatex
// Finally \code{OTB\_APPLICATION\_EXPORT} is called.
// Software Guide : EndLatex
//  Software Guide :BeginCodeSnippet
OTB_APPLICATION_EXPORT(otb::Wrapper::FeatureExtraction)
//  Software Guide :EndCodeSnippet

