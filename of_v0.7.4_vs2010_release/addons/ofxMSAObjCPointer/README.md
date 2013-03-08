ofxMSAObjCPointer
=====================================

Introduction
------------
C++ openFrameworks addon to provide simple ObjectiveC style Smart Pointer functionality, which keeps reference count and frees memory when reference count is zero. This is accomplished by providing retain/release methods similar to Objective C. This is now probably superseded by built in shared pointers.

Licence
-------
The code in this repository is available under the [MIT License](https://secure.wikimedia.org/wikipedia/en/wiki/Mit_license).  
Copyright (c) 2008-2012 Memo Akten, [www.memo.tv](http://www.memo.tv)  
The Mega Super Awesome Visuals Company


Installation
------------
Copy to your openFrameworks/addons folder.

Dependencies
------------
none

Compatibility
------------
openFrameworks 0072  
I am generally testing only with [openFrameworks](www.openframeworks.cc), however it should work with [Cinder](www.libcinder.org) too. If it doesn't, please file an issue.


Known issues
------------
none

Version history
------------
### v1.4    23/09/2012
- compatible with OF0072
- renamed (uppercase) MSA namespace to (lowercase) msa. (kept MSA as an alias for backwards compatibility)

### v1.3
- move to centralized MSALibs
- everything is msa:: namespace

### v1.2	07/04/2009
- changed license to revised BSD (a lot more more permissive than GPL)

### v1.1	29/02/09
- added verbose option (off by default)

### v1.01	16/10/08
- added virtual destructor

### v1.0	15/10/08
- initial version


