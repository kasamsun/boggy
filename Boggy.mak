# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=boggy - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to boggy - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "boggy - Win32 Release" && "$(CFG)" != "boggy - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Boggy.mak" CFG="boggy - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "boggy - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "boggy - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "boggy - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "boggy - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Boggy.exe"

CLEAN : 
	-@erase "$(INTDIR)\BitmapRgnWnd.obj"
	-@erase "$(INTDIR)\BkgWnd.obj"
	-@erase "$(INTDIR)\boggy.obj"
	-@erase "$(INTDIR)\boggy.res"
	-@erase "$(INTDIR)\BoggyAction.obj"
	-@erase "$(INTDIR)\BoggyAI.obj"
	-@erase "$(INTDIR)\BoggyApp.obj"
	-@erase "$(INTDIR)\MainWnd.obj"
	-@erase "$(INTDIR)\PreferenceWnd.obj"
	-@erase "$(OUTDIR)\Boggy.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/Boggy.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x41e /d "NDEBUG"
# ADD RSC /l 0x420 /d "NDEBUG"
RSC_PROJ=/l 0x420 /fo"$(INTDIR)/boggy.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Boggy.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Boggy.pdb" /machine:I386 /out:"$(OUTDIR)/Boggy.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BitmapRgnWnd.obj" \
	"$(INTDIR)\BkgWnd.obj" \
	"$(INTDIR)\boggy.obj" \
	"$(INTDIR)\boggy.res" \
	"$(INTDIR)\BoggyAction.obj" \
	"$(INTDIR)\BoggyAI.obj" \
	"$(INTDIR)\BoggyApp.obj" \
	"$(INTDIR)\MainWnd.obj" \
	"$(INTDIR)\PreferenceWnd.obj"

"$(OUTDIR)\Boggy.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Boggy.exe" "$(OUTDIR)\Boggy.bsc"

CLEAN : 
	-@erase "$(INTDIR)\BitmapRgnWnd.obj"
	-@erase "$(INTDIR)\BitmapRgnWnd.sbr"
	-@erase "$(INTDIR)\BkgWnd.obj"
	-@erase "$(INTDIR)\BkgWnd.sbr"
	-@erase "$(INTDIR)\boggy.obj"
	-@erase "$(INTDIR)\boggy.res"
	-@erase "$(INTDIR)\boggy.sbr"
	-@erase "$(INTDIR)\BoggyAction.obj"
	-@erase "$(INTDIR)\BoggyAction.sbr"
	-@erase "$(INTDIR)\BoggyAI.obj"
	-@erase "$(INTDIR)\BoggyAI.sbr"
	-@erase "$(INTDIR)\BoggyApp.obj"
	-@erase "$(INTDIR)\BoggyApp.sbr"
	-@erase "$(INTDIR)\MainWnd.obj"
	-@erase "$(INTDIR)\MainWnd.sbr"
	-@erase "$(INTDIR)\PreferenceWnd.obj"
	-@erase "$(INTDIR)\PreferenceWnd.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Boggy.bsc"
	-@erase "$(OUTDIR)\Boggy.exe"
	-@erase "$(OUTDIR)\Boggy.ilk"
	-@erase "$(OUTDIR)\Boggy.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Boggy.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x41e /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/boggy.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Boggy.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BitmapRgnWnd.sbr" \
	"$(INTDIR)\BkgWnd.sbr" \
	"$(INTDIR)\boggy.sbr" \
	"$(INTDIR)\BoggyAction.sbr" \
	"$(INTDIR)\BoggyAI.sbr" \
	"$(INTDIR)\BoggyApp.sbr" \
	"$(INTDIR)\MainWnd.sbr" \
	"$(INTDIR)\PreferenceWnd.sbr"

"$(OUTDIR)\Boggy.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Boggy.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Boggy.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BitmapRgnWnd.obj" \
	"$(INTDIR)\BkgWnd.obj" \
	"$(INTDIR)\boggy.obj" \
	"$(INTDIR)\boggy.res" \
	"$(INTDIR)\BoggyAction.obj" \
	"$(INTDIR)\BoggyAI.obj" \
	"$(INTDIR)\BoggyApp.obj" \
	"$(INTDIR)\MainWnd.obj" \
	"$(INTDIR)\PreferenceWnd.obj"

"$(OUTDIR)\Boggy.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "boggy - Win32 Release"
# Name "boggy - Win32 Debug"

!IF  "$(CFG)" == "boggy - Win32 Release"

!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\boggy.cpp
DEP_CPP_BOGGY=\
	".\BitmapRgnWnd.h"\
	".\Boggy.h"\
	".\BoggyAction.h"\
	".\BoggyAI.h"\
	".\BoggyApp.h"\
	".\PreferenceWnd.h"\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\boggy.obj" : $(SOURCE) $(DEP_CPP_BOGGY) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


"$(INTDIR)\boggy.obj" : $(SOURCE) $(DEP_CPP_BOGGY) "$(INTDIR)"

"$(INTDIR)\boggy.sbr" : $(SOURCE) $(DEP_CPP_BOGGY) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\boggy.rc
DEP_RSC_BOGGY_=\
	".\res\boggy.ico"\
	".\res\preferen.bmp"\
	".\res\run.bmp"\
	".\res\runs.bmp"\
	".\res\skull.cur"\
	".\res\stand.bmp"\
	

"$(INTDIR)\boggy.res" : $(SOURCE) $(DEP_RSC_BOGGY_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BoggyAction.cpp
DEP_CPP_BOGGYA=\
	".\Boggy.h"\
	".\BoggyAction.h"\
	".\BoggyAI.h"\
	".\BoggyApp.h"\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\BoggyAction.obj" : $(SOURCE) $(DEP_CPP_BOGGYA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


"$(INTDIR)\BoggyAction.obj" : $(SOURCE) $(DEP_CPP_BOGGYA) "$(INTDIR)"

"$(INTDIR)\BoggyAction.sbr" : $(SOURCE) $(DEP_CPP_BOGGYA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BoggyAI.cpp
DEP_CPP_BOGGYAI=\
	".\Boggy.h"\
	".\BoggyAction.h"\
	".\BoggyAI.h"\
	".\BoggyApp.h"\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\BoggyAI.obj" : $(SOURCE) $(DEP_CPP_BOGGYAI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


"$(INTDIR)\BoggyAI.obj" : $(SOURCE) $(DEP_CPP_BOGGYAI) "$(INTDIR)"

"$(INTDIR)\BoggyAI.sbr" : $(SOURCE) $(DEP_CPP_BOGGYAI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BkgWnd.cpp
DEP_CPP_BKGWN=\
	".\BkgWnd.h"\
	".\Boggy.h"\
	".\BoggyAction.h"\
	".\BoggyAI.h"\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\BkgWnd.obj" : $(SOURCE) $(DEP_CPP_BKGWN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


"$(INTDIR)\BkgWnd.obj" : $(SOURCE) $(DEP_CPP_BKGWN) "$(INTDIR)"

"$(INTDIR)\BkgWnd.sbr" : $(SOURCE) $(DEP_CPP_BKGWN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BitmapRgnWnd.cpp
DEP_CPP_BITMA=\
	".\BitmapRgnWnd.h"\
	".\BoggyApp.h"\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\BitmapRgnWnd.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


"$(INTDIR)\BitmapRgnWnd.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"

"$(INTDIR)\BitmapRgnWnd.sbr" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PreferenceWnd.cpp
DEP_CPP_PREFE=\
	".\BitmapRgnWnd.h"\
	".\PreferenceWnd.h"\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\PreferenceWnd.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


"$(INTDIR)\PreferenceWnd.obj" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"

"$(INTDIR)\PreferenceWnd.sbr" : $(SOURCE) $(DEP_CPP_PREFE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BoggyApp.cpp
DEP_CPP_BOGGYAP=\
	".\BitmapRgnWnd.h"\
	".\BkgWnd.h"\
	".\Boggy.h"\
	".\BoggyAction.h"\
	".\BoggyAI.h"\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\BoggyApp.obj" : $(SOURCE) $(DEP_CPP_BOGGYAP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


"$(INTDIR)\BoggyApp.obj" : $(SOURCE) $(DEP_CPP_BOGGYAP) "$(INTDIR)"

"$(INTDIR)\BoggyApp.sbr" : $(SOURCE) $(DEP_CPP_BOGGYAP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\dev\Kswafx\src\MainWnd.cpp
DEP_CPP_MAINW=\
	{$(INCLUDE)}"\MainWnd.h"\
	

!IF  "$(CFG)" == "boggy - Win32 Release"


"$(INTDIR)\MainWnd.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "boggy - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MainWnd.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MainWnd.sbr" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
