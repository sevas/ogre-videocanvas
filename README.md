Description
-----------

Video-to-texture proof of concept for Ogre 1.6. Uses OpenCV-1.1 to read the video file.
This work is not intended to be a final product.

However, it works.

Dependencies
------------

- Ogre 1.6
- boost 1.36 (not tested with )
- OpenCV 1.1 (not tested with OpenCV-2.x yet)

To build the project, you need to set the following environment variables:

- `$(OGRE16_HOME)`
- `$(BOOST136_HOME)`

Path for OpenCV headers and static libraries is not configurable yet. Change in the .vcproj file if necessary.


The following DLLs must be copied to the executable directory for a successful run:

- OgreMain(_d).dll
- OIS(_d).dll
- highgui(d).dll
- cxcore(d).dll
- cvaux(d).dll
- cv(d).dll

Don't forget to modify the `Plugins.cfg` file to point to the folder that holds the Ogre plugin DLLs (RenderSystemGL, PluginCgProgramManager et al.) 


Licence
-------

WTFPL. 
See COPYING file for details.