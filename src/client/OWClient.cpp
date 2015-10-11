

#include "OWClient.h"
#include "login_sequence.h"
#include "network/NetworkSocket.h"
#include <string>
#include <thread>
#include <cstdlib>

namespace owc {

  OWClient::Status        OWClient::client_status_ = E_DISCONNECTED;
  std::string             OWClient::user_name_;
  std::string             OWClient::token_;

  IrrlichtDevice*         OWClient::device_ = NULL;
  video::IVideoDriver*    OWClient::driver_ = NULL;
  scene::ISceneManager*   OWClient::smgr_ = NULL;
  gui::IGUIEnvironment*   OWClient::guienv_ = NULL;
  io::IFileSystem*        OWClient::fsystem_ = NULL;
  ILogger*                OWClient::logger_ = NULL;

  Properties*             OWClient::properties_ = NULL;

  io::CMYPArchiveLoader*  OWClient::myp_archive_loader_ = NULL;



  u32 OWClient::init( const c8* config_file_name ) {

    /** Reading configuration file **/
    properties_ = new Properties();
    int res = properties_->load(config_file_name);
    if(res) return res;

    /** Initializing Irrlicht subsystems **/
    /*device_ = createDevice( video::EDT_SOFTWARE, core::dimension2d<u32>(640, 480), 16,
          false, false, false, 0);*/

    device_ = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(1280, 720), 16,
          false, false, false, 0);
    if (!device_)
      return 1;


    fsystem_ = device_->getFileSystem();
    device_->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
    driver_ = device_->getVideoDriver();
    smgr_ = device_->getSceneManager();
    guienv_ = device_->getGUIEnvironment();
    guienv_->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
        core::rect<s32>(10,10,260,22), true);


    /** Setting up the camera */
    scene::ICameraSceneNode* camera = smgr_->addCameraSceneNodeFPS();
    camera->setFarValue(64000.0f);
    core::list<scene::ISceneNodeAnimator*>::ConstIterator camera_animators = camera->getAnimators().begin();
    scene::ISceneNodeAnimatorCameraFPS* camera_animator = (scene::ISceneNodeAnimatorCameraFPS*) *camera_animators;
    camera_animator->setMoveSpeed( 5.0f );
    logger_ = device_->getLogger();

    /** Adding resource files and directories **/
    std::string data_dir = properties_->get("data_dir");
    myp_archive_loader_ = new io::CMYPArchiveLoader( fsystem_ );
    fsystem_->addArchiveLoader( myp_archive_loader_ ); 

    logger_->log("Loading art.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("art.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading art.myp", irr::ELL_ERROR);
    }

    logger_->log("loading art2.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("art2.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading art2.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading art3.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("art3.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading art.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading data.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("data.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading data.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading world.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("world.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading world.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading interface.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("interface.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading interface.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading mft.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("mft.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading mft.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading patch.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("patch.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading patch.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading vo_english.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("vo_english.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading vo_english.myp", irr::ELL_ERROR);
    }

    logger_->log("Loading audio.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("audio.myp"), false, false, io::EFAT_MYP)) {
      logger_->log("Error loading audio.myp", irr::ELL_ERROR);
    }


    std::string zone = properties_->get("zone").c_str();

    logger_->log("Loading terrain", irr::ELL_INFORMATION);
    scene::ITerrainSceneNode* terrain = smgr_->addTerrainSceneNode(
        std::string("zones/zone").append(zone.c_str()).append("/offset.pcx").c_str(),
        std::string("zones/zone").append(zone.c_str()).append("/terrain.pcx").c_str(),
        0,                  
        -1,                 
        core::vector3df(0.0f, 0.f, 0.f),     // position
        core::vector3df(0.0f, 0.f, 0.f),     // rotation
        core::vector3df(64.0f, 256.0f, 64.0f),  // scale
        video::SColor ( 255, 255, 255, 255 ),   // vertexColor
        3,                  
        scene::ETPS_17,             
        0                   
        );

    terrain->setMaterialFlag(video::EMF_WIREFRAME, true);
    return 0;
  }

  u32 OWClient::free() {

    device_->drop();
    delete myp_archive_loader_;
    delete properties_;
    return 0;
  }

  u32 OWClient::run() {

/*    std::thread t([ properties ] () mutable {
        char* token;
        if( login( properties->get("user_name").c_str(), 
            properties->get("passwd").c_str(), 
            properties->get("server_ip").c_str(), 
            atoi(properties->get("server_port").c_str()), &token ) ) {
          return;
        }
        token_ = token;
        ::free(token);
        client_status_ = E_LOGGED_IN;
        });
    t.join();
    */
    while(device_->run())
    {

      driver_->beginScene(true, true, video::SColor(255,100,101,140));

      smgr_->drawAll();
      guienv_->drawAll();

      driver_->endScene();
    }

    return 0;
  }

}
