

#include "OWClient.h"
#include "login_sequence.h"
#include "network/NetworkSocket.h"
#include <string>
#include <thread>
#include <cstdlib>
#include <world/CZone.h>
#include <os.h>
#include <vector3d.h>
#include <irrMath.h>

namespace owc {

  OWClient::Status        OWClient::client_status_ = E_DISCONNECTED;
  std::string             OWClient::user_name_;
  std::string             OWClient::token_;

  IrrlichtDevice*         OWClient::device_ = NULL;
  video::IVideoDriver*    OWClient::driver_ = NULL;
  scene::ISceneManager*   OWClient::smgr_ = NULL;
  gui::IGUIEnvironment*   OWClient::guienv_ = NULL;
  irr::io::IFileSystem*        OWClient::fsystem_ = NULL;
  ILogger*                OWClient::logger_ = NULL;

  io::CProperties*             OWClient::properties_ = NULL;

  irr::io::CMYPArchiveLoader*  OWClient::myp_archive_loader_ = NULL;



  u32 OWClient::init( const c8* config_file_name ) {


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

    /** Reading configuration file **/
    properties_ = new io::CProperties();
    c8 str[256];
    sprintf(str, "Reading config file: %s", config_file_name );
    os::Printer::log(str);
    io::IReadFile* configFile = fsystem_->createAndOpenFile(config_file_name);
    if(!configFile) {
      os::Printer::log("Unable to open config file", ELL_ERROR );
    }
    int res = properties_->load(configFile);
    configFile->drop();
    if(res) return res;


    /** Setting up the camera */
    scene::ICameraSceneNode* camera = smgr_->addCameraSceneNodeFPS();
    camera->setFarValue(64000.0f);
    //camera->setFOV(1*core::PI/6.0);


    core::list<scene::ISceneNodeAnimator*>::ConstIterator camera_animators = camera->getAnimators().begin();
    scene::ISceneNodeAnimatorCameraFPS* camera_animator = (scene::ISceneNodeAnimatorCameraFPS*) *camera_animators;
    camera_animator->setMoveSpeed( 5.0f );


    logger_ = device_->getLogger();


    /** Adding resource files and directories **/
    core::stringc data_dir = properties_->get("data_dir");
    myp_archive_loader_ = new irr::io::CMYPArchiveLoader( fsystem_ );
    fsystem_->addArchiveLoader( myp_archive_loader_ ); 

    logger_->log("Loading art.myp", irr::ELL_INFORMATION);
    if(!fsystem_->addFileArchive( irr::io::path(data_dir.c_str()).append("/").append("art.myp"), false, false, io::EFAT_MYP)) {
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


    u32 zoneId = atoi(properties_->get("zone").c_str());

    os::Printer::log("Loading Zone");
    world::CZone zone(smgr_);
    zone.loadZone(zoneId);
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
