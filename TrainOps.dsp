# Microsoft Developer Studio Project File - Name="TrainOps" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TrainOps - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TrainOps.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrainOps.mak" CFG="TrainOps - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrainOps - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TrainOps - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "TrainOps"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TrainOps - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 sqlite3.lib libhpdf.lib /nologo /subsystem:windows /map /debug /machine:I386

!ELSEIF  "$(CFG)" == "TrainOps - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "pdfxlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "USE_THEMES" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 sqlite3.lib libhpdf.lib Msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TrainOps - Win32 Release"
# Name "TrainOps - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Static Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Static Classes\Config.cpp"
# End Source File
# Begin Source File

SOURCE=".\Static Classes\Config.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CabooseDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CabooseDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CarLocationTrainsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CarsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CarsListDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CarTypesDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CarTypesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CommoditiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCarDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCommodityDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditTrainDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EquipmentLocationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EquipmentNotFoundDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EquipmentTrackingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IGDebugDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\IndustriesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\IndustryCarDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\IndustryCarOrdersDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\IndustryDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\IndustryDetailDialog_Cars.cpp
# End Source File
# Begin Source File

SOURCE=.\IndustryDetailDialog_Commodities.cpp
# End Source File
# Begin Source File

SOURCE=.\industryDetailTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IndustryUsageDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LocationDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LocationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LocomotiveDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LocomotiveDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NewVersionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PDFViewerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PhysicalLayoutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Reports.cpp
# End Source File
# Begin Source File

SOURCE=.\RunTrainsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ScheduledTrainsReportsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SidingDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SidingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TerminateNewLocationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TerminateTrainDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TrainDetailDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TrainOps.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\TrainOps.hpj

!IF  "$(CFG)" == "TrainOps - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__TRAIN="hlp\AfxCore.rtf"	"hlp\AfxPrint.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Release
TargetName=TrainOps
InputPath=.\hlp\TrainOps.hpj
InputName=TrainOps

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "TrainOps - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TRAIN="hlp\AfxCore.rtf"	"hlp\AfxPrint.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Debug
TargetName=TrainOps
InputPath=.\hlp\TrainOps.hpj
InputName=TrainOps

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrainOps.rc
# End Source File
# Begin Source File

SOURCE=.\TrainOpsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TrainOpsView.cpp
# End Source File
# Begin Source File

SOURCE=.\TrainsDialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_BldNum.h
# End Source File
# Begin Source File

SOURCE=.\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\CabooseDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\CabooseDialog.h
# End Source File
# Begin Source File

SOURCE=.\CarLocationTrainsDialog.h
# End Source File
# Begin Source File

SOURCE=.\CarsDialog.h
# End Source File
# Begin Source File

SOURCE=.\CarsListDialog.h
# End Source File
# Begin Source File

SOURCE=.\CarTypesDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\CarTypesDialog.h
# End Source File
# Begin Source File

SOURCE=.\CommoditiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditCarDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditCommodityDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditTrainDialog.h
# End Source File
# Begin Source File

SOURCE=.\EquipmentLocationDialog.h
# End Source File
# Begin Source File

SOURCE=.\EquipmentNotFoundDialog.h
# End Source File
# Begin Source File

SOURCE=.\EquipmentTrackingDialog.h
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\IGDebugDialog.h
# End Source File
# Begin Source File

SOURCE=.\IndustriesDialog.h
# End Source File
# Begin Source File

SOURCE=.\IndustryCarDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\IndustryCarOrdersDialog.h
# End Source File
# Begin Source File

SOURCE=.\IndustryDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\IndustryDetailDialog_Cars.h
# End Source File
# Begin Source File

SOURCE=.\IndustryDetailDialog_Commodities.h
# End Source File
# Begin Source File

SOURCE=.\industryDetailTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\IndustryUsageDialog.h
# End Source File
# Begin Source File

SOURCE=.\LocationDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\LocationDialog.h
# End Source File
# Begin Source File

SOURCE=.\LocomotiveDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\LocomotiveDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NewVersionDialog.h
# End Source File
# Begin Source File

SOURCE=.\PDFViewerDialog.h
# End Source File
# Begin Source File

SOURCE=.\PhysicalLayoutDialog.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesDialog.h
# End Source File
# Begin Source File

SOURCE=.\Reports.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "TrainOps - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=TrainOps
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "TrainOps - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=TrainOps
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RunTrainsDialog.h
# End Source File
# Begin Source File

SOURCE=.\ScheduledTrainsReportsDialog.h
# End Source File
# Begin Source File

SOURCE=.\SidingDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\SidingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TerminateNewLocationDialog.h
# End Source File
# Begin Source File

SOURCE=.\TerminateTrainDialog.h
# End Source File
# Begin Source File

SOURCE=.\TrainDetailDialog.h
# End Source File
# Begin Source File

SOURCE=.\TrainOps.h
# End Source File
# Begin Source File

SOURCE=.\TrainOpsDoc.h
# End Source File
# Begin Source File

SOURCE=.\TrainOpsView.h
# End Source File
# Begin Source File

SOURCE=.\TrainsDialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dbschema.xml
# End Source File
# Begin Source File

SOURCE=.\res\folders.bmp
# End Source File
# Begin Source File

SOURCE=.\res\releasenotes.txt
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TrainOps.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\TrainOps.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrainOps.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TrainOpsDoc.ico
# End Source File
# End Group
# Begin Group "CustomControls"

# PROP Default_Filter ""
# Begin Group "MSChart"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\mschart.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart.h
# End Source File
# Begin Source File

SOURCE=.\vcaxis.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxis.h
# End Source File
# Begin Source File

SOURCE=.\vcaxisgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxisgrid.h
# End Source File
# Begin Source File

SOURCE=.\vcaxisscale.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxisscale.h
# End Source File
# Begin Source File

SOURCE=.\vcaxistitle.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxistitle.h
# End Source File
# Begin Source File

SOURCE=.\vcbackdrop.cpp
# End Source File
# Begin Source File

SOURCE=.\vcbackdrop.h
# End Source File
# Begin Source File

SOURCE=.\vcbrush.cpp
# End Source File
# Begin Source File

SOURCE=.\vcbrush.h
# End Source File
# Begin Source File

SOURCE=.\vccategoryscale.cpp
# End Source File
# Begin Source File

SOURCE=.\vccategoryscale.h
# End Source File
# Begin Source File

SOURCE=.\vccolor.cpp
# End Source File
# Begin Source File

SOURCE=.\vccolor.h
# End Source File
# Begin Source File

SOURCE=.\vccoor.cpp
# End Source File
# Begin Source File

SOURCE=.\vccoor.h
# End Source File
# Begin Source File

SOURCE=.\vcdatagrid.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatagrid.h
# End Source File
# Begin Source File

SOURCE=.\vcdatapoint.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatapoint.h
# End Source File
# Begin Source File

SOURCE=.\vcdatapointlabel.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatapointlabel.h
# End Source File
# Begin Source File

SOURCE=.\vcdatapoints.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatapoints.h
# End Source File
# Begin Source File

SOURCE=.\vcfill.cpp
# End Source File
# Begin Source File

SOURCE=.\vcfill.h
# End Source File
# Begin Source File

SOURCE=.\vcfont.cpp
# End Source File
# Begin Source File

SOURCE=.\vcfont.h
# End Source File
# Begin Source File

SOURCE=.\vcfootnote.cpp
# End Source File
# Begin Source File

SOURCE=.\vcfootnote.h
# End Source File
# Begin Source File

SOURCE=.\vcframe.cpp
# End Source File
# Begin Source File

SOURCE=.\vcframe.h
# End Source File
# Begin Source File

SOURCE=.\vcintersection.cpp
# End Source File
# Begin Source File

SOURCE=.\vcintersection.h
# End Source File
# Begin Source File

SOURCE=.\vclabel.cpp
# End Source File
# Begin Source File

SOURCE=.\vclabel.h
# End Source File
# Begin Source File

SOURCE=.\vclabels.cpp
# End Source File
# Begin Source File

SOURCE=.\vclabels.h
# End Source File
# Begin Source File

SOURCE=.\vclcoor.cpp
# End Source File
# Begin Source File

SOURCE=.\vclcoor.h
# End Source File
# Begin Source File

SOURCE=.\vclegend.cpp
# End Source File
# Begin Source File

SOURCE=.\vclegend.h
# End Source File
# Begin Source File

SOURCE=.\vclight.cpp
# End Source File
# Begin Source File

SOURCE=.\vclight.h
# End Source File
# Begin Source File

SOURCE=.\vclightsource.cpp
# End Source File
# Begin Source File

SOURCE=.\vclightsource.h
# End Source File
# Begin Source File

SOURCE=.\vclightsources.cpp
# End Source File
# Begin Source File

SOURCE=.\vclightsources.h
# End Source File
# Begin Source File

SOURCE=.\vclocation.cpp
# End Source File
# Begin Source File

SOURCE=.\vclocation.h
# End Source File
# Begin Source File

SOURCE=.\vcmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\vcmarker.h
# End Source File
# Begin Source File

SOURCE=.\vcpen.cpp
# End Source File
# Begin Source File

SOURCE=.\vcpen.h
# End Source File
# Begin Source File

SOURCE=.\vcplot.cpp
# End Source File
# Begin Source File

SOURCE=.\vcplot.h
# End Source File
# Begin Source File

SOURCE=.\vcplotbase.cpp
# End Source File
# Begin Source File

SOURCE=.\vcplotbase.h
# End Source File
# Begin Source File

SOURCE=.\vcrect.cpp
# End Source File
# Begin Source File

SOURCE=.\vcrect.h
# End Source File
# Begin Source File

SOURCE=.\vcseries.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseries.h
# End Source File
# Begin Source File

SOURCE=.\vcseriescollection.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseriescollection.h
# End Source File
# Begin Source File

SOURCE=.\vcseriesmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseriesmarker.h
# End Source File
# Begin Source File

SOURCE=.\vcseriesposition.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseriesposition.h
# End Source File
# Begin Source File

SOURCE=.\vcshadow.cpp
# End Source File
# Begin Source File

SOURCE=.\vcshadow.h
# End Source File
# Begin Source File

SOURCE=.\vcstatline.cpp
# End Source File
# Begin Source File

SOURCE=.\vcstatline.h
# End Source File
# Begin Source File

SOURCE=.\vctextlayout.cpp
# End Source File
# Begin Source File

SOURCE=.\vctextlayout.h
# End Source File
# Begin Source File

SOURCE=.\vctick.cpp
# End Source File
# Begin Source File

SOURCE=.\vctick.h
# End Source File
# Begin Source File

SOURCE=.\vctitle.cpp
# End Source File
# Begin Source File

SOURCE=.\vctitle.h
# End Source File
# Begin Source File

SOURCE=.\vcvaluescale.cpp
# End Source File
# Begin Source File

SOURCE=.\vcvaluescale.h
# End Source File
# Begin Source File

SOURCE=.\vcview3d.cpp
# End Source File
# Begin Source File

SOURCE=.\vcview3d.h
# End Source File
# Begin Source File

SOURCE=.\vcwall.cpp
# End Source File
# Begin Source File

SOURCE=.\vcwall.h
# End Source File
# Begin Source File

SOURCE=.\vcweighting.cpp
# End Source File
# Begin Source File

SOURCE=.\vcweighting.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\amsEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\amsEdit.h
# End Source File
# Begin Source File

SOURCE=.\CheckableGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckableGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\Color.h
# End Source File
# Begin Source File

SOURCE=.\ColorEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorEdit.h
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\ComboListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CoolBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolBtn.h
# End Source File
# Begin Source File

SOURCE=.\Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InputBox.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBox.h
# End Source File
# Begin Source File

SOURCE=.\LinePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\LinePlot.h
# End Source File
# Begin Source File

SOURCE=.\MenuXP.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuXP.h
# End Source File
# Begin Source File

SOURCE=.\SplashScreenEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashScreenEx.h
# End Source File
# Begin Source File

SOURCE=.\StatusBarXP.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBarXP.h
# End Source File
# Begin Source File

SOURCE=.\TBTreeStateMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TBTreeStateMgr.h
# End Source File
# Begin Source File

SOURCE=.\TextScroller.cpp
# End Source File
# Begin Source File

SOURCE=.\TextScroller.h
# End Source File
# Begin Source File

SOURCE=.\Tmschema.h
# End Source File
# Begin Source File

SOURCE=.\TOListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TOListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\Uxtheme.h
# End Source File
# Begin Source File

SOURCE=.\VisualStylesXP.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualStylesXP.h
# End Source File
# Begin Source File

SOURCE=.\XComboList.cpp
# End Source File
# Begin Source File

SOURCE=.\XComboList.h
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.h
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\TrainOps.cnt

!IF  "$(CFG)" == "TrainOps - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\TrainOps.cnt
InputName=TrainOps

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "TrainOps - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\TrainOps.cnt
InputName=TrainOps

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "ResizableDialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ResizableDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.h
# End Source File
# Begin Source File

SOURCE=.\ResizableFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableFormView.h
# End Source File
# Begin Source File

SOURCE=.\ResizableGrip.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableGrip.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableLayout.h
# End Source File
# Begin Source File

SOURCE=.\ResizableMinMax.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableMinMax.h
# End Source File
# Begin Source File

SOURCE=.\ResizablePage.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizablePage.h
# End Source File
# Begin Source File

SOURCE=.\ResizablePageEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizablePageEx.h
# End Source File
# Begin Source File

SOURCE=.\ResizableSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableSheet.h
# End Source File
# Begin Source File

SOURCE=.\ResizableSheetEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableSheetEx.h
# End Source File
# Begin Source File

SOURCE=.\ResizableState.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableState.h
# End Source File
# End Group
# Begin Group "Helper Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SQLite\Common\CppSQLite3.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\SQLite\Common\CppSQLite3.h
# End Source File
# Begin Source File

SOURCE=.\julian.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\julian.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\Parseit.cpp
# End Source File
# Begin Source File

SOURCE=.\Parseit.h
# End Source File
# Begin Source File

SOURCE=.\RestartAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\RestartAPI.h
# End Source File
# Begin Source File

SOURCE=.\SendFileTo.h
# End Source File
# Begin Source File

SOURCE=.\SimpleDate.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleDate.h
# End Source File
# Begin Source File

SOURCE=.\SQLite\Common\sqlite3.h
# End Source File
# Begin Source File

SOURCE=.\url.h
# End Source File
# Begin Source File

SOURCE=.\XMLite.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLite.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\debugging.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\results.txt
# End Source File
# End Target
# End Project
# Section TrainOps : {6341C6BC-87CF-4F42-86B9-88240C7A5EE5}
# 	2:5:Class:CVSPDFPageInfo
# 	2:10:HeaderFile:vspdfpageinfo.h
# 	2:8:ImplFile:vspdfpageinfo.cpp
# End Section
# Section TrainOps : {E9E074E6-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFootnote
# 	2:10:HeaderFile:vcfootnote.h
# 	2:8:ImplFile:vcfootnote.cpp
# End Section
# Section TrainOps : {1CEE20F1-0C80-4A17-94CB-B16D5C6C57D0}
# 	2:21:DefaultSinkHeaderFile:_gdviewercnt.h
# 	2:16:DefaultSinkClass:C_GdViewerCnt
# End Section
# Section TrainOps : {E9E074CD-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcRect
# 	2:10:HeaderFile:vcrect.h
# 	2:8:ImplFile:vcrect.cpp
# End Section
# Section TrainOps : {D02701A1-67B1-11D1-AA5F-0060081C43D9}
# 	2:5:Class:CToc
# 	2:10:HeaderFile:toc.h
# 	2:8:ImplFile:toc.cpp
# End Section
# Section TrainOps : {E9E074D1-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcPen
# 	2:10:HeaderFile:vcpen.h
# 	2:8:ImplFile:vcpen.cpp
# End Section
# Section TrainOps : {4C42DA1D-BA10-4A67-8716-B15EDFC7FEAE}
# 	2:5:Class:CVSPDFViewer
# 	2:10:HeaderFile:vspdfviewer.h
# 	2:8:ImplFile:vspdfviewer.cpp
# End Section
# Section TrainOps : {1731C330-651D-11D1-AA58-0060081C43D9}
# 	2:5:Class:CPaperSizes
# 	2:10:HeaderFile:papersizes.h
# 	2:8:ImplFile:papersizes.cpp
# End Section
# Section TrainOps : {50409DDA-0CFB-4582-A6AB-027B9E8A26AA}
# 	2:5:Class:C_GdViewerCnt
# 	2:10:HeaderFile:_gdviewercnt.h
# 	2:8:ImplFile:_gdviewercnt.cpp
# End Section
# Section TrainOps : {1F573201-F7C7-49CC-83E8-8C935591CD9C}
# 	2:21:DefaultSinkHeaderFile:pdfreader.h
# 	2:16:DefaultSinkClass:CPDFReader
# End Section
# Section TrainOps : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section TrainOps : {BBC8271F-9A1A-4918-9EF5-B1A99D4E8AE3}
# 	2:5:Class:C_GdViewer
# 	2:10:HeaderFile:_gdviewer.h
# 	2:8:ImplFile:_gdviewer.cpp
# End Section
# Section TrainOps : {3A2B370A-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CMSChart
# 	2:10:HeaderFile:mschart.h
# 	2:8:ImplFile:mschart.cpp
# End Section
# Section TrainOps : {92FA2B44-468C-4A96-8AB1-A367D029AF8C}
# 	2:21:DefaultSinkHeaderFile:_pdfview.h
# 	2:16:DefaultSinkClass:C_PDFView
# End Section
# Section TrainOps : {057F8DD0-67B3-11D1-AA5F-0060081C43D9}
# 	2:5:Class:CHistory
# 	2:10:HeaderFile:history.h
# 	2:8:ImplFile:history.cpp
# End Section
# Section TrainOps : {5FF94445-7D98-45AC-85FF-8E67B338E017}
# 	2:5:Class:CWPViewPLUS
# 	2:10:HeaderFile:wpviewplus.h
# 	2:8:ImplFile:wpviewplus.cpp
# End Section
# Section TrainOps : {E9E07506-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcStatLine
# 	2:10:HeaderFile:vcstatline.h
# 	2:8:ImplFile:vcstatline.cpp
# End Section
# Section TrainOps : {E9E0751D-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcIntersection
# 	2:10:HeaderFile:vcintersection.h
# 	2:8:ImplFile:vcintersection.cpp
# End Section
# Section TrainOps : {4E1A6CF4-2639-4B23-AF6B-1FAC5E823640}
# 	2:21:DefaultSinkHeaderFile:ziepdf.h
# 	2:16:DefaultSinkClass:CZIEPDF
# End Section
# Section TrainOps : {53D6352D-E1F4-45BF-832F-FF74FE707331}
# 	2:5:Class:C_gdViewer
# 	2:10:HeaderFile:_gdviewer.h
# 	2:8:ImplFile:_gdviewer.cpp
# End Section
# Section TrainOps : {AFD5DD64-D791-4FD4-873E-912839806BC4}
# 	2:21:DefaultSinkHeaderFile:_gdviewer.h
# 	2:16:DefaultSinkClass:C_gdViewer
# End Section
# Section TrainOps : {00C7C2A0-8B82-11D1-8B57-00A0C98CD92B}
# 	2:21:DefaultSinkHeaderFile:arviewer.h
# 	2:16:DefaultSinkClass:CARViewer
# End Section
# Section TrainOps : {E9E07521-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxisScale
# 	2:10:HeaderFile:vcaxisscale.h
# 	2:8:ImplFile:vcaxisscale.cpp
# End Section
# Section TrainOps : {E9E07511-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeries
# 	2:10:HeaderFile:vcseries.h
# 	2:8:ImplFile:vcseries.cpp
# End Section
# Section TrainOps : {E9E074D5-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcTextLayout
# 	2:10:HeaderFile:vctextlayout.h
# 	2:8:ImplFile:vctextlayout.cpp
# End Section
# Section TrainOps : {6758A40E-5DB2-4820-B28B-C69AA98374C4}
# 	2:5:Class:CVSPDFCoreDictObj
# 	2:10:HeaderFile:vspdfcoredictobj.h
# 	2:8:ImplFile:vspdfcoredictobj.cpp
# End Section
# Section TrainOps : {E9E074DC-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcShadow
# 	2:10:HeaderFile:vcshadow.h
# 	2:8:ImplFile:vcshadow.cpp
# End Section
# Section TrainOps : {E9E074EC-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLightSource
# 	2:10:HeaderFile:vclightsource.h
# 	2:8:ImplFile:vclightsource.cpp
# End Section
# Section TrainOps : {6B5C725F-6C15-49DF-ACF0-893F77AA0128}
# 	2:5:Class:CVSPDFViewerOutline
# 	2:10:HeaderFile:vspdfvieweroutline.h
# 	2:8:ImplFile:vspdfvieweroutline.cpp
# End Section
# Section TrainOps : {E9E074F0-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLight
# 	2:10:HeaderFile:vclight.h
# 	2:8:ImplFile:vclight.cpp
# End Section
# Section TrainOps : {E9E074E0-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFrame
# 	2:10:HeaderFile:vcframe.h
# 	2:8:ImplFile:vcframe.cpp
# End Section
# Section TrainOps : {E9E074FC-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcWall
# 	2:10:HeaderFile:vcwall.h
# 	2:8:ImplFile:vcwall.cpp
# End Section
# Section TrainOps : {5C210E00-F309-11D0-A8E9-00A0C90F29FC}
# 	2:5:Class:CPrinter
# 	2:10:HeaderFile:printer.h
# 	2:8:ImplFile:printer.cpp
# End Section
# Section TrainOps : {FA0BFF20-8D3D-11D1-8B5E-9AD7CD000000}
# 	2:5:Class:CDDTools
# 	2:10:HeaderFile:ddtools.h
# 	2:8:ImplFile:ddtools.cpp
# End Section
# Section TrainOps : {E9E074C9-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcCoor
# 	2:10:HeaderFile:vccoor.h
# 	2:8:ImplFile:vccoor.cpp
# End Section
# Section TrainOps : {E9E07515-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxisTitle
# 	2:10:HeaderFile:vcaxistitle.h
# 	2:8:ImplFile:vcaxistitle.cpp
# End Section
# Section TrainOps : {9AB6FDC1-60AA-4439-BBD6-3D5BBD399566}
# 	2:5:Class:CVSPDFPrinter
# 	2:10:HeaderFile:vspdfprinter.h
# 	2:8:ImplFile:vspdfprinter.cpp
# End Section
# Section TrainOps : {E9E0750C-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataPointLabel
# 	2:10:HeaderFile:vcdatapointlabel.h
# 	2:8:ImplFile:vcdatapointlabel.cpp
# End Section
# Section TrainOps : {E9E07525-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxis
# 	2:10:HeaderFile:vcaxis.h
# 	2:8:ImplFile:vcaxis.cpp
# End Section
# Section TrainOps : {20E34D59-4C57-4122-9596-F53EFA44304B}
# 	2:5:Class:CPDFReader
# 	2:10:HeaderFile:pdfreader.h
# 	2:8:ImplFile:pdfreader.cpp
# End Section
# Section TrainOps : {92182371-0531-11D1-A92F-0060081C43D9}
# 	2:5:Class:CCanvas
# 	2:10:HeaderFile:canvas.h
# 	2:8:ImplFile:canvas.cpp
# End Section
# Section TrainOps : {E9E074E4-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcTitle
# 	2:10:HeaderFile:vctitle.h
# 	2:8:ImplFile:vctitle.cpp
# End Section
# Section TrainOps : {E9E074CB-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLCoor
# 	2:10:HeaderFile:vclcoor.h
# 	2:8:ImplFile:vclcoor.cpp
# End Section
# Section TrainOps : {E9E074F4-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcPlotBase
# 	2:10:HeaderFile:vcplotbase.h
# 	2:8:ImplFile:vcplotbase.cpp
# End Section
# Section TrainOps : {BDF3E9D2-5F7A-4F4A-A914-7498C862EA6A}
# 	2:21:DefaultSinkHeaderFile:vspdfviewer.h
# 	2:16:DefaultSinkClass:CVSPDFViewer
# End Section
# Section TrainOps : {D8D3A8F6-46EF-4424-AE49-4376F1FE1EB4}
# 	2:21:DefaultSinkHeaderFile:wpviewpdfx.h
# 	2:16:DefaultSinkClass:CWPViewPDFX
# End Section
# Section TrainOps : {E9E07504-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeriesMarker
# 	2:10:HeaderFile:vcseriesmarker.h
# 	2:8:ImplFile:vcseriesmarker.cpp
# End Section
# Section TrainOps : {E9E074E8-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLegend
# 	2:10:HeaderFile:vclegend.h
# 	2:8:ImplFile:vclegend.cpp
# End Section
# Section TrainOps : {E9E074D8-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFont
# 	2:10:HeaderFile:vcfont.h
# 	2:8:ImplFile:vcfont.cpp
# End Section
# Section TrainOps : {E9E074CF-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcColor
# 	2:10:HeaderFile:vccolor.h
# 	2:8:ImplFile:vccolor.cpp
# End Section
# Section TrainOps : {3BA0F3F2-947F-49CE-9DDF-B235EC726B27}
# 	2:21:DefaultSinkHeaderFile:_gdviewer.h
# 	2:16:DefaultSinkClass:C_GdViewer
# End Section
# Section TrainOps : {E9E0751B-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxisGrid
# 	2:10:HeaderFile:vcaxisgrid.h
# 	2:8:ImplFile:vcaxisgrid.cpp
# End Section
# Section TrainOps : {3CB5B0C3-8CF9-11D1-8B5E-9AD7CD000000}
# 	2:5:Class:CDDToolBar
# 	2:10:HeaderFile:ddtoolbar.h
# 	2:8:ImplFile:ddtoolbar.cpp
# End Section
# Section TrainOps : {E9E074D3-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcMarker
# 	2:10:HeaderFile:vcmarker.h
# 	2:8:ImplFile:vcmarker.cpp
# End Section
# Section TrainOps : {3A2B370C-BA0A-11D1-B137-0000F8753F5D}
# 	2:21:DefaultSinkHeaderFile:mschart.h
# 	2:16:DefaultSinkClass:CMSChart
# End Section
# Section TrainOps : {7C472508-2517-4528-BC8D-163CCEB5B1A3}
# 	2:5:Class:CWPViewPDFX
# 	2:10:HeaderFile:wpviewpdfx.h
# 	2:8:ImplFile:wpviewpdfx.cpp
# End Section
# Section TrainOps : {E9E074DA-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcBrush
# 	2:10:HeaderFile:vcbrush.h
# 	2:8:ImplFile:vcbrush.cpp
# End Section
# Section TrainOps : {F0A37FB6-996D-4664-ABC4-B0AC98B3D38B}
# 	2:5:Class:CZIEPDF
# 	2:10:HeaderFile:ziepdf.h
# 	2:8:ImplFile:ziepdf.cpp
# End Section
# Section TrainOps : {FA0BFF23-8D3D-11D1-8B5E-9AD7CD000000}
# 	2:5:Class:CDDTool
# 	2:10:HeaderFile:ddtool.h
# 	2:8:ImplFile:ddtool.cpp
# End Section
# Section TrainOps : {E9E074FA-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcWeighting
# 	2:10:HeaderFile:vcweighting.h
# 	2:8:ImplFile:vcweighting.cpp
# End Section
# Section TrainOps : {E9E074EA-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataGrid
# 	2:10:HeaderFile:vcdatagrid.h
# 	2:8:ImplFile:vcdatagrid.cpp
# End Section
# Section TrainOps : {E9E07508-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLabel
# 	2:10:HeaderFile:vclabel.h
# 	2:8:ImplFile:vclabel.cpp
# End Section
# Section TrainOps : {BC265422-0654-4EFF-ADD4-9ED076A613BF}
# 	2:5:Class:C_PDFView
# 	2:10:HeaderFile:_pdfview.h
# 	2:8:ImplFile:_pdfview.cpp
# End Section
# Section TrainOps : {60F907F7-3F21-413E-849E-90F6C4CC222B}
# 	2:5:Class:CBravaXView
# 	2:10:HeaderFile:bravaxview.h
# 	2:8:ImplFile:bravaxview.cpp
# End Section
# Section TrainOps : {E9E0751F-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcTick
# 	2:10:HeaderFile:vctick.h
# 	2:8:ImplFile:vctick.cpp
# End Section
# Section TrainOps : {B0F1971D-468F-4647-BBF4-81E71E1CEF94}
# 	2:21:DefaultSinkHeaderFile:bravaxview.h
# 	2:16:DefaultSinkClass:CBravaXView
# End Section
# Section TrainOps : {E9E0750A-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLabels
# 	2:10:HeaderFile:vclabels.h
# 	2:8:ImplFile:vclabels.cpp
# End Section
# Section TrainOps : {E9E07523-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcValueScale
# 	2:10:HeaderFile:vcvaluescale.h
# 	2:8:ImplFile:vcvaluescale.cpp
# End Section
# Section TrainOps : {E9E074DE-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFill
# 	2:10:HeaderFile:vcfill.h
# 	2:8:ImplFile:vcfill.cpp
# End Section
# Section TrainOps : {E9E07513-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeriesCollection
# 	2:10:HeaderFile:vcseriescollection.h
# 	2:8:ImplFile:vcseriescollection.cpp
# End Section
# Section TrainOps : {E9E0752A-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataPoints
# 	2:10:HeaderFile:vcdatapoints.h
# 	2:8:ImplFile:vcdatapoints.cpp
# End Section
# Section TrainOps : {E9E074EE-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLightSources
# 	2:10:HeaderFile:vclightsources.h
# 	2:8:ImplFile:vclightsources.cpp
# End Section
# Section TrainOps : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section TrainOps : {28F708E9-E741-4277-BBDC-80A0A23685E0}
# 	2:5:Class:CVSPDFCoreObj
# 	2:10:HeaderFile:vspdfcoreobj.h
# 	2:8:ImplFile:vspdfcoreobj.cpp
# End Section
# Section TrainOps : {E9E074F2-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcView3d
# 	2:10:HeaderFile:vcview3d.h
# 	2:8:ImplFile:vcview3d.cpp
# End Section
# Section TrainOps : {E9E074E2-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcBackdrop
# 	2:10:HeaderFile:vcbackdrop.h
# 	2:8:ImplFile:vcbackdrop.cpp
# End Section
# Section TrainOps : {998431B3-7685-4D61-9120-5CC079AE7BD5}
# 	2:5:Class:CVSPDFDoc
# 	2:10:HeaderFile:vspdfdoc.h
# 	2:8:ImplFile:vspdfdoc.cpp
# End Section
# Section TrainOps : {E9E07517-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcCategoryScale
# 	2:10:HeaderFile:vccategoryscale.h
# 	2:8:ImplFile:vccategoryscale.cpp
# End Section
# Section TrainOps : {E9E07527-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcPlot
# 	2:10:HeaderFile:vcplot.h
# 	2:8:ImplFile:vcplot.cpp
# End Section
# Section TrainOps : {E9E07502-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeriesPosition
# 	2:10:HeaderFile:vcseriesposition.h
# 	2:8:ImplFile:vcseriesposition.cpp
# End Section
# Section TrainOps : {E9E074D6-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLocation
# 	2:10:HeaderFile:vclocation.h
# 	2:8:ImplFile:vclocation.cpp
# End Section
# Section TrainOps : {4AB670E3-96C7-11D1-8B77-3C346B000000}
# 	2:5:Class:CPages
# 	2:10:HeaderFile:pages.h
# 	2:8:ImplFile:pages.cpp
# End Section
# Section TrainOps : {49989DD2-7E77-4AE3-A108-8BB778B77855}
# 	2:5:Class:CVSPDFVisiblePages
# 	2:10:HeaderFile:vspdfvisiblepages.h
# 	2:8:ImplFile:vspdfvisiblepages.cpp
# End Section
# Section TrainOps : {E9E0750E-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataPoint
# 	2:10:HeaderFile:vcdatapoint.h
# 	2:8:ImplFile:vcdatapoint.cpp
# End Section
