#ifndef __vtkMRMLEMSTargetNode_h
#define __vtkMRMLEMSTargetNode_h

#include "vtkMRMLNode.h"
#include "vtkEMSegment.h"
#include "vtkMRMLEMSVolumeCollectionNode.h"

//
// Onyl here for legacy purposes 
//
class VTK_EMSEGMENT_EXPORT vtkMRMLEMSTargetNode : 
  public vtkMRMLEMSVolumeCollectionNode
{
public:
  static vtkMRMLEMSTargetNode *New();
  vtkTypeMacro(vtkMRMLEMSTargetNode,vtkMRMLEMSVolumeCollectionNode);

  virtual vtkMRMLNode* CreateNodeInstance() VTK_OVERRIDE;

  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() VTK_OVERRIDE {return "EMSTarget";}

protected:
  vtkMRMLEMSTargetNode() {} ;
  ~vtkMRMLEMSTargetNode() {};
  vtkMRMLEMSTargetNode(const vtkMRMLEMSTargetNode&);
  void operator=(const vtkMRMLEMSTargetNode&);
  // For LEGACY - should not be used anymore 
  virtual int AddNode(const char* key, const char* volumeNodeID) VTK_OVERRIDE
  {
     return Superclass::AddNode(key,volumeNodeID);
  }

  virtual void SetNthNodeID(int n, const char* nodeID)  VTK_OVERRIDE
  {
     Superclass::SetNthNodeID(n,nodeID);
  }

  virtual void  MoveNthNode(int n, int toIndex) VTK_OVERRIDE
  {
    Superclass::MoveNthNode(n, toIndex);
  }
};

#endif
