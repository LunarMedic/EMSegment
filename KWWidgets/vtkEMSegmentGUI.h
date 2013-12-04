#ifndef __vtkEMSegmentGUI_h
#define __vtkEMSegmentGUI_h

#include "vtkSlicerModuleGUI.h"
#include "vtkEMSegment.h"

class vtkEMSegmentLogic;
class vtkEMSegmentMRMLManager;
class vtkKWWizardWidget;
class vtkEMSegmentParametersSetStep;
class vtkEMSegmentIntensityImagesStep;
class vtkEMSegmentAnatomicalStructureStep;
class vtkEMSegmentSpatialPriorsStep;
class vtkEMSegmentNodeParametersStep;
class vtkEMSegmentIntensityDistributionsStep;
class vtkEMSegmentRegistrationParametersStep;
class vtkEMSegmentRunSegmentationStep;
class vtkEMSegmentInputChannelsStep;
class vtkEMSegmentPreProcessingStep;
class vtkKWWizardStep;
class vtkKWLabel;
class vtkMRMLEMSTemplateNode;

#define EMSEG_MENU_BUTTON_WIDTH 15
#define EMSEG_WIDGETS_LABEL_WIDTH 25



class VTK_EMSEGMENT_EXPORT vtkEMSegmentGUI : 
  public vtkSlicerModuleGUI
{
public:
  static vtkEMSegmentGUI *New();
  vtkTypeMacro(vtkEMSegmentGUI,vtkSlicerModuleGUI);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description: Get the categorization of the module.
  const char *GetCategory() const
  { return "Segmentation"; }

  // Description: 
  // Get/Set logic node
  vtkGetObjectMacro(Logic, vtkEMSegmentLogic);

  /// Implement setter for vtkLogic* pointer
  virtual void SetModuleLogic(vtkSlicerLogic*);

   // Description: 
  // Get/Set MRML node
  vtkGetObjectMacro(Node, vtkMRMLEMSTemplateNode);
  virtual void SetNode(vtkMRMLEMSTemplateNode*);

  // Description: 
  // Get wizard widget
  vtkGetObjectMacro(WizardWidget, vtkKWWizardWidget);
  vtkGetObjectMacro(AnatomicalStructureStep, vtkEMSegmentAnatomicalStructureStep);
  vtkGetObjectMacro(ParametersSetStep,vtkEMSegmentParametersSetStep);
  vtkGetObjectMacro(NodeParametersStep,vtkEMSegmentNodeParametersStep);
  vtkGetObjectMacro(PreProcessingStep,vtkEMSegmentPreProcessingStep);
  vtkGetObjectMacro(InputChannelStep,vtkEMSegmentInputChannelsStep);


  //BTX
  enum {
    SegmentationModeSimple = 1,
    SegmentationModeAdvanced 
  };
  //ETX

  void SetSegmentationModeToAdvanced() {this->SegmentationMode =  SegmentationModeAdvanced; }
  void SetSegmentationModeToSimple() {this->SegmentationMode =  SegmentationModeSimple; }
  vtkGetMacro(SegmentationMode,int);
  int IsSegmentationModeAdvanced() {return (this->SegmentationMode ==  SegmentationModeAdvanced); }

  // Description:
  // Create widgets
  virtual void BuildGUI();
  //BTX
  using vtkSlicerComponentGUI::BuildGUI;
  //ETX

  // Description:
  // Initialize module
  virtual void Init();

  // Description:
  // Delete Widgets
  virtual void TearDownGUI();

  // Description:
  // Add observers to GUI widgets
  virtual void AddGUIObservers();
  
  // Description:
  // Remove observers to GUI widgets
  virtual void RemoveGUIObservers();

  // Description:
  // Remove observers to MRML node
  virtual void RemoveMRMLNodeObservers();

  // Description:
  // Remove observers to Logic
  virtual void RemoveLogicObservers();
  
  // Description:
  // Pprocess events generated by Logic
  virtual void ProcessLogicEvents( vtkObject *caller, unsigned long event,
                                   void *callData);

  // Description:
  // Pprocess events generated by GUI widgets
  virtual void ProcessGUIEvents( vtkObject *caller, unsigned long event,
                                 void *callData);

  // Description:
  // Pprocess events generated by MRML
  virtual void ProcessMRMLEvents( vtkObject *caller, unsigned long event, 
                                  void *callData);
  // Description:
  // Describe behavior at module startup and exit.
  virtual void Enter(){};
  //BTX
  using vtkSlicerComponentGUI::Enter;
  //ETX

  virtual void Exit(){};

  // Description: The name of the Module - this is used to 
  // construct the proc invocations
  vtkGetStringMacro(ModuleName);
  vtkSetStringMacro(ModuleName);

  // Description: set an observer by number (work around
  // limitation in kwwidgets tcl wrapping)
  unsigned long AddObserverByNumber(vtkObject *observee, unsigned long event);

  void StartSegmentation(); 

  // Description:
  // Initialize a menu with loaded volumes
  static void PopulateMenuWithLoadedVolumes(vtkEMSegmentMRMLManager *mrmlManager, vtkKWMenu *menu, vtkObject* object, const char* callback); 

  vtkEMSegmentMRMLManager* GetMRMLManager();

protected:

private:
  vtkEMSegmentGUI();
  ~vtkEMSegmentGUI();
  vtkEMSegmentGUI(const vtkEMSegmentGUI&);
  void operator=(const vtkEMSegmentGUI&);

  // Description:
  // Updates GUI widgets based on parameters values in MRML node
  void UpdateGUI();

  // Description:
  // Updates parameters values in MRML node based on GUI widgets 
  void UpdateMRML();
  
  // Description:
  // Updates registration progress on the status bar of the main application. 
  virtual void UpdateRegistrationProgress();

  void SetLogic(vtkEMSegmentLogic*);

  vtkEMSegmentLogic       *Logic;

  // Never Delete it - it is needed to know what the current node is ! 
  vtkMRMLEMSTemplateNode* Node; 

  char *ModuleName;

  // Description:
  // The wizard widget and steps
  vtkKWWizardWidget                      *WizardWidget;
  vtkEMSegmentParametersSetStep          *ParametersSetStep;
  vtkEMSegmentAnatomicalStructureStep    *AnatomicalStructureStep;
  vtkEMSegmentSpatialPriorsStep          *SpatialPriorsStep;
  vtkEMSegmentIntensityImagesStep        *IntensityImagesStep;
  vtkEMSegmentIntensityDistributionsStep *IntensityDistributionsStep;
  vtkEMSegmentNodeParametersStep         *NodeParametersStep;
  vtkEMSegmentRegistrationParametersStep *RegistrationParametersStep;
  vtkEMSegmentRunSegmentationStep        *RunSegmentationStep;
  vtkEMSegmentInputChannelsStep          *InputChannelStep;  
  vtkEMSegmentPreProcessingStep          *PreProcessingStep;

  vtkKWWizardStep *StartSegmentStep;

   /// 
  /// Acknowledgement icons
  vtkKWLabel *NACLabel;
  vtkKWLabel *UPennLabel;

  int SegmentationMode;
};

#endif
