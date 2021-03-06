from __main__ import qt, ctk, slicer, tcl

from EMSegmentStep import *
from Helper import *
from EMSegmentDynamicFrame import *

class EMSegmentQuickStep1( EMSegmentStep ) :

  def __init__( self, stepid ):
    self.initialize( stepid )
    self.setName( '1. Define Input Datasets' )
    self.setDescription( 'Name how many volumes should be segmented and select the set of scans for segmentation.' )

    self.__stepid = stepid
    self.__dynamicFrame = None
    self.__updating = 0
    self.__initialized = False

  def dynamicFrame( self ):
    '''
    '''
    if not self.__dynamicFrame:

      self.__dynamicFrame = EMSegmentDynamicFrame()

    return self.__dynamicFrame

  def createUserInterface( self ):
    '''
    '''
    slicer.modules.emsegmentinitialdistance = 4

    # just call to copy the emsegmenter task to a temp dir
    self.logic().GetTasks()

    self.__layout = super( EMSegmentQuickStep1, self ).createUserInterface()

    infoLabel = qt.QLabel( 'This module provides EM segmentation without an atlas.\nIt is possible to segment different structures by manual sampling.\n\n' )
    infoLabel.setFont( self.getBoldFont() )
    self.__layout.addWidget( infoLabel )

    # the input channels
    inputChannelGroupBox = qt.QGroupBox()
    inputChannelGroupBox.setTitle( 'Input Datasets' )
    inputChannelGroupBox.toolTip = 'Please configure the datasets which should be segmented.'
    self.__layout.addWidget( inputChannelGroupBox )

    inputChannelGroupBoxLayout = qt.QFormLayout( inputChannelGroupBox )

    self.__inputChannelList = slicer.qSlicerEMSegmentInputChannelListWidget()
    self.__inputChannelList.setMRMLManager( self.mrmlManager() )
    inputChannelGroupBoxLayout.addWidget( self.__inputChannelList )

    # add empty row
    self.__layout.addRow( "", qt.QWidget() )

    #
    # dynamic frame
    #
    dynamicFrame = qt.QGroupBox()
    dynamicFrame.setTitle( 'Check List' )
    dynamicFrame.toolTip = 'Please check anything applicable.'
    self.__layout.addWidget( dynamicFrame )
    dynamicFrameLayout = qt.QVBoxLayout( dynamicFrame )

    # .. now pass the layout to the dynamicFrame
    self.dynamicFrame().setLayout( dynamicFrameLayout )
    #
    # end of dynamic frame
    #

    # check if we have a valid EMEasy MRML Node
    templateNodes = slicer.mrmlScene.GetNodesByClassByName( 'vtkMRMLEMSTemplateNode', 'EMEasy' )

    if templateNodes.GetNumberOfItems() > 0:

      # we load the last template node which fits the taskname
      templateNode = templateNodes.GetItemAsObject( templateNodes.GetNumberOfItems() - 1 )

      loadResult = self.mrmlManager().SetLoadedParameterSetIndex( templateNode )

      if int( loadResult ) != 0:
        Helper.Info( "EMS node is corrupted - the manager could not be updated with new task: " + taskName )
        #return False
      else:
        Helper.Info( "Loading completed." )

      self.logic().DefineTclTaskFileFromMRML()

      # clear the dynamic panel
      self.dynamicFrame().setMRMLManager( self.mrmlManager() )
      self.dynamicFrame().clearElements()

      self.logic().SourceTaskFiles()

      slicer.modules.emsegmentAdvancedDynamicFrame = self.dynamicFrame()

      logicTclName = self.logic().GetSlicerCommonInterface().GetTclNameFromPointer( self.logic() )

      tcl( '::EMSegmenterPreProcessingTcl::ShowUserInterface ' + str( logicTclName ) )

      slicer.modules.emsegmenteasystep2.disableNumberOfStructures()

      self.__initialized = True


  def propagateToMRML( self ):
    '''
    '''
    if not self.__updating:

      self.__updating = 1

      self.__inputChannelList.updateMRMLFromWidget()

      # propagate dynamic frame settings to MRML
      self.dynamicFrame().SaveSettingsToMRML()

      self.__updating = 0



  def loadFromMRML( self ):
    '''
    '''
    if not self.__updating:

      self.__updating = 1

      self.__inputChannelList.updateWidgetFromMRML()

      if self.__inputChannelList.inputChannelCount() == 0:
        self.__inputChannelList.addInputChannel()

      self.dynamicFrame().LoadSettingsFromMRML()

      self.__updating = 0


  def reset( self ):
    '''
    '''
    self.__initialized = False


  def onEntry( self, comingFrom, transitionType ):
    '''
    '''
    super( EMSegmentQuickStep1, self ).onEntry( comingFrom, transitionType )

    # Remove everything if needed - did not work properly -however when running it without it  then it resets pipeline but leaves nodes in scend ! 
    # self.logic().RemoveTaskAndTempFiles()
    # self.reset()

    slicer.modules.emsegmentAdvancedDynamicFrame = self.dynamicFrame()


    if not self.__initialized:

      # use default taskfile
      taskFileShort = slicer.vtkMRMLEMSGlobalParametersNode.GetDefaultTaskTclFileName()

      self.mrmlManager().CreateAndObserveNewParameterSet()
      templateNodes = slicer.mrmlScene.GetNodesByClass( 'vtkMRMLEMSTemplateNode' )

      self.mrmlManager().SetNthParameterName( templateNodes.GetNumberOfItems() - 1, 'EMEasy' )
      self.mrmlManager().SetTclTaskFilename( taskFileShort )

      self.logic().SourceTaskFiles()

      loadResult = self.mrmlManager().SetLoadedParameterSetIndex( templateNodes.GetItemAsObject( templateNodes.GetNumberOfItems() - 1 ) )

      if int( loadResult ) != 0:
        Helper.Info( "EMS node is corrupted - the manager could not be updated with new task: EMQuick" )
        #return False
      else:
        Helper.Info( "Loading completed." )

      self.logic().DefineTclTaskFileFromMRML()

      # use anatomy label colors
      self.mrmlManager().GetGlobalParametersNode().SetColormap( 'vtkMRMLColorTableNodeFileGenericAnatomyColors.txt' )

      # clear the dynamic panel
      self.dynamicFrame().setMRMLManager( self.mrmlManager() )
      self.dynamicFrame().clearElements()

      logicTclName = self.logic().GetSlicerCommonInterface().GetTclNameFromPointer( self.logic() )

      tcl( '::EMSegmenterPreProcessingTcl::ShowUserInterface ' + str( logicTclName ) )

      self.__initialized = True

    self.loadFromMRML()



  def onExit( self, goingTo, transitionType ):
    '''
    '''
    self.propagateToMRML()

    #self.mrmlManager().GetWorkingDataNode().SetAlignedTargetNodeIsValid( 0 )
    #self.mrmlManager().GetWorkingDataNode().SetAlignedAtlasNodeIsValid( 0 )

    super( EMSegmentQuickStep1, self ).onExit( goingTo, transitionType )



  def validate( self, desiredBranchId ):
    '''
    '''
    super( EMSegmentQuickStep1, self ).validate( desiredBranchId )

    # we need at least one input channel
    if self.__inputChannelList.inputChannelCount() == 0:
      self.validationFailed( desiredBranchId, 'Input Channel Error', 'Please add at least one input channel!' )
      return

    # we need an assigned volume for each channel
    for c in range( self.__inputChannelList.inputChannelCount() ):
      if not self.__inputChannelList.inputChannelVolume( c ):
        self.validationFailed( desiredBranchId, 'Input Channel Error', 'Please assign a volume to each input channel!' )
        return

    # check if all channels have different volumes assigned
    if self.__inputChannelList.identicalInputVolumes():
      self.validationFailed( desiredBranchId, 'Input Channel Error', 'Please assign different volumes to individual input channel!' )
      return

    # check if all channels have different names
    if self.__inputChannelList.identicalInputChannelNames():
      self.validationFailed( desiredBranchId, 'Input Channel Error', 'Please assign different names to individual input channel!' )
      return

    self.validationSucceeded( desiredBranchId )
