# Microsoft Developer Studio Project File - Name="uni" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=uni - Win32 Free
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "uni.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "uni.mak" CFG="uni - Win32 Free"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "uni - Win32 Checked" (based on "Win32 (x86) External Target")
!MESSAGE "uni - Win32 Free" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "uni - Win32 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "objchk_wxp_x86"
# PROP BASE Intermediate_Dir "objchk_wxp_x86"
# PROP BASE Cmd_Line "..\MakeDrv.bat . wxp c"
# PROP BASE Rebuild_Opt "-nmake /a"
# PROP BASE Target_File "uni.dll"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "objchk_wxp_x86"
# PROP Intermediate_Dir "objchk_wxp_x86"
# PROP Cmd_Line "..\MakeDrv.bat . wxp c"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "uni.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "uni - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "objfre_wxp_x86"
# PROP BASE Intermediate_Dir "objfre_wxp_x86"
# PROP BASE Cmd_Line "..\MakeDrv.bat . wxp c"
# PROP BASE Rebuild_Opt "-nmake /a"
# PROP BASE Target_File "uni.dll"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "objfre_wxp_x86"
# PROP Intermediate_Dir "objfre_wxp_x86"
# PROP Cmd_Line "..\MakeDrv.bat . wxp f"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "uni.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "uni - Win32 Checked"
# Name "uni - Win32 Free"

!IF  "$(CFG)" == "uni - Win32 Checked"

!ELSEIF  "$(CFG)" == "uni - Win32 Free"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\color.cpp
# End Source File
# Begin Source File

SOURCE=.\ddiblt.cpp
# End Source File
# Begin Source File

SOURCE=.\ddicntl.cpp
# End Source File
# Begin Source File

SOURCE=.\ddifont.cpp
# End Source File
# Begin Source File

SOURCE=.\ddimisc.cpp
# End Source File
# Begin Source File

SOURCE=.\ddipath.cpp
# End Source File
# Begin Source File

SOURCE=.\dditext.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\devmode.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgclbk.cpp
# End Source File
# Begin Source File

SOURCE=.\dllentry.cpp
# End Source File
# Begin Source File

SOURCE=.\enable.cpp
# End Source File
# Begin Source File

SOURCE=.\globals.cpp
# End Source File
# Begin Source File

SOURCE=.\imgproc.cpp
# End Source File
# Begin Source File

SOURCE=.\intrface.cpp
# End Source File
# Begin Source File

SOURCE=.\output.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\ddiblt.h
# End Source File
# Begin Source File

SOURCE=.\ddicntl.h
# End Source File
# Begin Source File

SOURCE=.\ddifont.h
# End Source File
# Begin Source File

SOURCE=.\ddimisc.h
# End Source File
# Begin Source File

SOURCE=.\ddipath.h
# End Source File
# Begin Source File

SOURCE=.\dditext.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\devmode.h
# End Source File
# Begin Source File

SOURCE=.\dlgclbk.h
# End Source File
# Begin Source File

SOURCE=.\enable.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\imgproc.h
# End Source File
# Begin Source File

SOURCE=.\intrface.h
# End Source File
# Begin Source File

SOURCE=.\name.h
# End Source File
# Begin Source File

SOURCE=.\oem.h
# End Source File
# Begin Source File

SOURCE=.\oemuni.h
# End Source File
# Begin Source File

SOURCE=.\output.h
# End Source File
# Begin Source File

SOURCE=.\precomp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\uni.rc
# End Source File
# End Group
# Begin Group "Extra Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\i386\copy2sys.bat
# End Source File
# Begin Source File

SOURCE=..\MakeDrv.bat
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\rebuild.cmd
# End Source File
# Begin Source File

SOURCE=.\sources
# End Source File
# Begin Source File

SOURCE=.\UNI.def
# End Source File
# End Group
# End Target
# End Project
