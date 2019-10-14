# Project2
go to your project properties
C/C++ > General > Additional Include Directories
  add the directory ..\Dependencies\Include
Linker > General > Additional Library Directories
  add the directory ..\Dependencies\lib\fltklib
  and also ..\Dependencies\lib\vs14
Linker > Input > Additional Dependencies
  fltkd.lib
  wsock32.lib
  comctl32.lib   (that is a lowercase L, not a one)
  fltkjpegd.lib
  fltkimagesd.lib
  libeay32.lib
  ssleay32.lib
  mysqlcppconn8.lib
  mysqlcppconn.lib
Linker > System > Subsystem
  Select "Console"
Linker > Optimization > References
  select "Yes"
Linker > Optimization > References
  select "Yes"
APPLY AND PRESS OK

At the top be sure to set to release and x86

When you run the program the command prompt will ask for your
MySQL database information
  Your user name will be your be what you use to access MySQL databases, likely your computer user name or "root"
  The password will be whatever you used when setting up MySQL as well
  
The username and password for the GUI are in the C++ code
  User name: user
  Password: pass
