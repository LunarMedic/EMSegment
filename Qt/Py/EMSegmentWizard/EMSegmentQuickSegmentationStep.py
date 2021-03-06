from __main__ import qt, ctk, slicer, tcl

import os

from EMSegmentStep import *
from Helper import *

class EMSegmentQuickSegmentationStep( EMSegmentStep ) :

  def __init__( self, stepid ):
    self.initialize( stepid )
    self.setName( '10. Segment' )
    self.setDescription( '' )
    self.icon = qt.QIcon( ":/Icons/EMSegmentQuick.png" )

  def createUserInterface( self ):
    '''
    '''
    self.__layout = super( EMSegmentQuickSegmentationStep, self ).createUserInterface()

  def runPreProcessing( self ):
    '''
    '''
    # notify user
    dialog = qt.QMessageBox()
    dialog.setWindowTitle("Please wait")
    dialog.setText("Please wait while pre-processing runs..")
    dialog.setModal( False )
    dialog.show()

    # message = qt.QMessageBox( qt.QMessageBox.NoIcon, "Please wait", "Please wait while pre-processing runs..", qt.QMessageBox.Ignore )
    # message.setModal( False )
    # message.show()
    slicer.app.processEvents()

    # run preprocessing
    returnValue = tcl( "::EMSegmenterPreProcessingTcl::Run" )

    # message.hide()
    dialog.hide()

    slicer.app.processEvents()

    if not returnValue or int( returnValue ) != 0:
      # something went wrong!
      # error message!
      messageBox = qt.QMessageBox.warning( self, "Error", "Pre-processing did not execute correctly!" )
      return

    workingDataNode = self.mrmlManager().GetWorkingDataNode()

    if workingDataNode:
      # set flags in the mrml nodes
      workingDataNode.SetAlignedTargetNodeIsValid( 1 )
      workingDataNode.SetAlignedAtlasNodeIsValid( 1 )

      # show preprocessing output in sliceViews
      volumeCollection = workingDataNode.GetInputTargetNode()
      if volumeCollection:
        outputNode = volumeCollection.GetNthVolumeNode( 0 )
        # propagate to sliceViews
        selectionNode = slicer.app.applicationLogic().GetSelectionNode()
        selectionNode.SetReferenceActiveVolumeID( outputNode.GetID() )
        #selectionNode.SetReferenceSecondaryVolumeID( outputNode.GetID() )
        slicer.app.applicationLogic().PropagateVolumeSelection()

    Helper.Info( '=============================================' )
    Helper.Info( 'Pre-processing completed successfully' )
    Helper.Info( '=============================================' )

  def onEntry( self, comingFrom, transitionType ):
    '''
    '''
    super( EMSegmentQuickSegmentationStep, self ).onEntry( comingFrom, transitionType )

    self.runPreProcessing()

    targetNode = self.mrmlManager().GetWorkingDataNode().GetInputTargetNode()
    alignedAtlasNode = self.mrmlManager().GetWorkingDataNode().GetAlignedAtlasNode()

    if alignedAtlasNode and not self.mrmlManager().DoTargetAndAtlasDataTypesMatch( targetNode, alignedAtlasNode ):
      # error
      messageBox = qt.QMessageBox.warning( self, "Error", "Scalar type mismatch for input images; all image scalar types must be the same (including input channels and aligned/resampled/casted atlas images)." )
      return

    workingDirectory = self.mrmlManager().GetSaveWorkingDirectory()
    if self.mrmlManager().GetSaveIntermediateResults():
      # if we want to save the results directly, we have to check if the workingDirectory is setup correctly
      if not workingDirectory:
        # workingDirectory was not configured
        messageBox = qt.QMessageBox.warning( self, "Error", "Saving intermediate results is turned on but no intermediate directory is selected." )
        return
      elif not os.path.exists( workingDirectory ):
        # the workingDirectory path does not exist
        messageBox = qt.QMessageBox.warning( self, "Error", "Saving intermediate results is turned on but the intermediate directory " + workingDirectory + " does not exist!" )
        return

    if self.mrmlManager().GetAtlasNumberOfTrainingSamples() <= 0:
      # compute the number of training samples
      self.mrmlManager().ComputeAtlasNumberOfTrainingSamples()

    # create output volumes
    self.logic().CreateOutputVolumeNode()

    # notify user
    dialog = qt.QMessageBox()
    dialog.setWindowTitle("Please wait")
    dialog.setText("Please wait while the segmentation runs..")
    dialog.setModal( False )
    dialog.show()

#    message = qt.QMessageBox( qt.QMessageBox.NoIcon, "Please wait", "Please wait while the segmentation runs..", qt.QMessageBox.Ignore )
#    message.setModal( False )
#    message.show()

    slicer.app.processEvents()

    # start the segmentation
    returnValue = self.logic().StartSegmentationWithoutPreprocessing( slicer.app.applicationLogic() )

    # message.hide()
    dialog.hide()
    slicer.app.processEvents()

    if returnValue != 0:
      # something went wrong!
      # error message!
      messageBox = qt.QMessageBox.warning( self, "Error", "Segmentation did not execute correctly!" )
      return

    outputNode = self.mrmlManager().GetOutputVolumeNode()

    if not outputNode:
      messageBox = qt.QMessageBox.warning( self, "Error", "Output node was not found!" )
      return

    selectionNode = slicer.app.applicationLogic().GetSelectionNode()
    #selectionNode.SetReferenceActiveVolumeID(bgVolumeID)
    selectionNode.SetReferenceActiveLabelVolumeID( outputNode.GetID() )
    slicer.app.applicationLogic().PropagateVolumeSelection()

    # show foreground volume
    numberOfCompositeNodes = slicer.mrmlScene.GetNumberOfNodesByClass( 'vtkMRMLSliceCompositeNode' )
    for n in xrange( numberOfCompositeNodes ):
      compositeNode = slicer.mrmlScene.GetNthNodeByClass( n, 'vtkMRMLSliceCompositeNode' )
      if compositeNode:
        # display a fade of foreground/background
        compositeNode.SetForegroundOpacity( 0.5 )

    Helper.Debug( "All Done!" )

    workflow = self.workflow()
    if not workflow:
      Helper.Error( "No valid workflow found!" )
      return False

    # we go backward to display statistics
    workflow.goBackward()

  def validate( self, desiredBranchId ):
    '''
    '''
    super( EMSegmentQuickSegmentationStep, self ).validate( desiredBranchId )

    self.validationSucceeded( desiredBranchId )
