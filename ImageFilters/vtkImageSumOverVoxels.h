/*=auto=========================================================================

(c) Copyright 2001 Massachusetts Institute of Technology 

Permission is hereby granted, without payment, to copy, modify, display 
and distribute this software and its documentation, if any, for any purpose, 
provided that the above copyright notice and the following three paragraphs 
appear on all copies of this software.  Use of this software constitutes 
acceptance of these terms and conditions.

IN NO EVENT SHALL MIT BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, 
INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE 
AND ITS DOCUMENTATION, EVEN IF MIT HAS BEEN ADVISED OF THE POSSIBILITY OF 
SUCH DAMAGE.

MIT SPECIFICALLY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTIES INCLUDING, 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.

THE SOFTWARE IS PROVIDED "AS IS."  MIT HAS NO OBLIGATION TO PROVIDE 
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================auto=*/
#ifndef __vtkImageSumOverVoxels_h
#define __vtkImageSumOverVoxels_h

// EMSegment includes
#include "vtkEMSegment.h"

// VTK includes
#if VTK_MAJOR_VERSION <= 5
#include "vtkImageToImageFilter.h"
#else
#include <vtkImageAlgorithm.h>
#endif

class VTK_EMSEGMENT_EXPORT vtkImageSumOverVoxels
#if VTK_MAJOR_VERSION <= 5
  : public vtkImageToImageFilter
#else
  : public vtkImageAlgorithm
#endif
{
  public:
  static vtkImageSumOverVoxels *New();
#if VTK_MAJOR_VERSION <= 5
  vtkTypeMacro(vtkImageSumOverVoxels,vtkImageToImageFilter);
#else
  vtkTypeMacro(vtkImageSumOverVoxels, vtkImageAlgorithm);
#endif
  void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;
  
  double GetVoxelSum() {return this->VoxelSum;}
  vtkGetVector3Macro(Centroid,double); 

  vtkSetMacro(ComputeCentroid,int);
  vtkBooleanMacro(ComputeCentroid,int);

protected:

  vtkImageSumOverVoxels();
  //  vtkImageSumOverVoxels(const vtkImageSumOverVoxels&) {};
  ~vtkImageSumOverVoxels(){};

  void operator=(const vtkImageSumOverVoxels&) {};
  
  // When it works on parallel machines use : 
  //  void ThreadedExecute(vtkImageData *inData, vtkImageData *outData,int outExt[6], int id);
  // If you do not want to have it multi threaded 
#if VTK_MAJOR_VERSION <= 5
  void ComputeInputUpdateExtent(int inExt[6], int outExt[6]);
  void ExecuteData(vtkDataObject *);
  void ExecuteInformation(){this->vtkImageToImageFilter::ExecuteInformation();};
  void ExecuteInformation(vtkImageData *inData,vtkImageData *outData);
#else
  virtual int RequestData(vtkInformation* request,
                          vtkInformationVector** inputVector,
                          vtkInformationVector* outputVector) VTK_OVERRIDE;
#endif

  double VoxelSum;
  double Centroid[3];
  int ComputeCentroid;
};
#endif



 







