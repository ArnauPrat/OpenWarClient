

#include <world/CZone.h>
#include <os.h>
#include <vector3d.h>

namespace owc {
  namespace world {

    CZone::CZone( scene::ISceneManager* sceneManager ) : ZoneId(0), SceneManager(sceneManager ) {
      SceneManager->grab();
    }
    
    CZone::~CZone() {
      SceneManager->drop();
    }

    u32 CZone::loadZone( u32 zoneId ) {
      io::IFileSystem* fileSystem = SceneManager->getFileSystem();
      fileSystem->grab();

      core::stringc baseFolder;
      c8 id[4];
      sprintf(id,"%03d",zoneId);
      baseFolder.append("zones/zone").append(id);
      io::path sectorFileName(baseFolder);
      io::path offsetFileName(baseFolder);
      io::path terrainFileName(baseFolder);
      sectorFileName.append("/sector.dat");
      offsetFileName.append("/offset.pcx");
      terrainFileName.append("/terrain.pcx");

      io::IReadFile* sectorFile = fileSystem->createAndOpenFile(sectorFileName);
      Properties.load(sectorFile);
      SSectorData sectorData( Properties.getInt("sizex"),
                              Properties.getInt("sizey"),
                              Properties.getInt("offsetfactor"), 
                              Properties.getInt("scalefactor"));
      sectorFile->drop();

      scene::ISplatterTerrainSceneNode* terrain = SceneManager->addSplatterTerrainSceneNode(
          offsetFileName,
          terrainFileName,
          0,                  
          -1,                 
          core::vector3df(0.0f, 0.f, 0.f),     // position
          core::vector3df(0.0f, 0.f, 0.f),     // rotation
          core::vector3df(64.0f, 1.25f, 64.0f),  // scale
          sectorData.BaseFactor,
          sectorData.OffsetFactor,
          video::SColor ( 255, 255, 255, 255 ),   // vertexColor
          3,                  
          scene::ETPS_129,             
          0                   
          );

      /*scene::ISplatterTerrainSceneNode* terrain = SceneManager->addSplatterTerrainSceneNode(
          "./Heightmap.png",
          "./Heightmap.png",
          0,                  
          -1,                 
          core::vector3df(0.0f, 0.f, 0.f),     // position
          core::vector3df(0.0f, 0.f, 0.f),     // rotation
          core::vector3df(8.0f, 1.0f, 8.0f),  // scale
          video::SColor ( 255, 255, 255, 255 ),   // vertexColor
          3,                  
          scene::ETPS_129,             
          0                   
          );
          */

      terrain->setPosition(-terrain->getTerrainCenter());
      terrain->setMaterialFlag(video::EMF_WIREFRAME, true);
      
      fileSystem->drop();
      return 0;
    }

    u32 CZone::getZoneId() {
      return ZoneId;
    }
  }
}
