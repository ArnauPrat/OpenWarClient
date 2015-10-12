
********************************************
list of modifications over original irrlicht
********************************************

  [scene]
- Changed the color format when loading heightmaps at CTerrainSceneNode to support RGB.
- Added functions in CTerrainSceneNode to support loading heightmaps based on two separate heightmap files, one
  containing a base, and the other containing an offset.
  [io]
- Added archive loader for MYP files
- Added CProperties class, that allows reading a text file of properties
- Added support for reading text lines in IReadFile
- Added support for performing fseek at positions larger than 32bits for linux and windows platforms
