
Files in this directory are provided to customize the autolink function
provided by the build wizard. By including one of these files before
secall.h in your stdafx.h, your app will autolink to that specific
build configuration tailored through the build wizard.

A custom build configuration can include both a subset of the total
Objective Toolkit library functionality as well as custom target
names for the library/dll.

Example stdafx.h includes:

#include "toolkit/config/MyConfig1.h"
#include "toolkit/secall.h"

Note also: if you include secall.h without prepending a custom build 
configuration header, the last configuration set by the build wizard
will be used by default.