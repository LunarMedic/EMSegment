#ifndef __vtkMRMLEMSTemplateNode_h
#define __vtkMRMLEMSTemplateNode_h

#include "vtkMRML.h"
#include "vtkMRMLNode.h"
#include "vtkEMSegment.h"
#include "vtkMRMLScene.h"

class vtkMRMLEMSGlobalParametersNode;
class vtkMRMLEMSTreeNode;
class vtkMRMLEMSAtlasNode;
class vtkMRMLEMSVolumeCollectionNode;
class vtkMRMLEMSWorkingDataNode;

class VTK_EMSEGMENT_EXPORT vtkMRMLEMSTemplateNode : 
  public vtkMRMLNode
{
public:
  static vtkMRMLEMSTemplateNode *New();
  vtkTypeMacro(vtkMRMLEMSTemplateNode,vtkMRMLNode);
  void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

  virtual vtkMRMLNode* CreateNodeInstance() VTK_OVERRIDE;

  // Description:
  // Set node attributes
  virtual void ReadXMLAttributes(const char** atts) VTK_OVERRIDE;

  // Description:
  // Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent) VTK_OVERRIDE;

  // Description:
  // Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node) VTK_OVERRIDE;

  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() VTK_OVERRIDE {return "EMSTemplate";}


  // Description:
  // Set the references of the node to the scene - only used in Slicer4 
  virtual void SetSceneReferences() VTK_OVERRIDE;

  // Description:
  // Updates this node if it depends on other nodes
  // when the node is deleted in the scene
  virtual void UpdateReferences() VTK_OVERRIDE;

  // Description:
  // Update the stored reference to another node in the scene
  virtual void UpdateReferenceID(const char *oldID, const char *newID) VTK_OVERRIDE;

  // associated parameters nodes
  vtkGetStringMacro(TreeNodeID);
  vtkSetReferenceStringMacro(TreeNodeID);
  vtkMRMLEMSTreeNode* GetTreeNode();

  vtkGetStringMacro(GlobalParametersNodeID);
  vtkSetReferenceStringMacro(GlobalParametersNodeID);
  vtkMRMLEMSGlobalParametersNode* GetGlobalParametersNode();

  vtkGetStringMacro(SpatialAtlasNodeID);
  //BTX
  vtkSetReferenceStringMacro(SpatialAtlasNodeID);
  //ETX
  void SetReferenceSpatialAtlasNodeID(const char* name)
  {
    this->SetSpatialAtlasNodeID(name);
  } 
  vtkMRMLEMSAtlasNode* GetSpatialAtlasNode();
  
  vtkGetStringMacro(SubParcellationNodeID);
  //BTX
  vtkSetReferenceStringMacro(SubParcellationNodeID);
  //ETX
  void SetReferenceSubParcellationNodeID(const char* name)
  {
    this->SetSubParcellationNodeID(name);
  } 
  vtkMRMLEMSVolumeCollectionNode* GetSubParcellationNode();

  vtkGetStringMacro         (EMSWorkingDataNodeID);
  vtkSetReferenceStringMacro(EMSWorkingDataNodeID);
  vtkMRMLEMSWorkingDataNode* GetEMSWorkingDataNode();

protected:
  vtkMRMLEMSTemplateNode();
  ~vtkMRMLEMSTemplateNode();
  vtkMRMLEMSTemplateNode(const vtkMRMLEMSTemplateNode&);
  void operator=(const vtkMRMLEMSTemplateNode&);

  char*                               TreeNodeID;
  char*                               GlobalParametersNodeID;
  char*                               SpatialAtlasNodeID;
  char*                               SubParcellationNodeID;
  char*                               EMSWorkingDataNodeID;
};

#endif
