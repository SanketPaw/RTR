cls

cl.exe /c /EHsc /I C:\glew\include\GL OGL.cpp 

rc.exe OGL.rc  

link.exe OGL.obj OGl.res /LIBPATH:C:\glew\lib\Release\x64 user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

OGL.exe