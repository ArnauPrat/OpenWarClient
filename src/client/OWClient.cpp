

#include "OWClient.h"
#include "io/Log.h"
#include "login_sequence.h"
#include "network/NetworkSocket.h"
#include <string>
#include <thread>

namespace owc {

  OWClient::Status        OWClient::client_status_ = E_DISCONNECTED;
  std::string             OWClient::user_name_;
  std::string             OWClient::token_;

  IrrlichtDevice*         OWClient::device_ = NULL;
  video::IVideoDriver*    OWClient::driver_ = NULL;
  scene::ISceneManager*   OWClient::smgr_ = NULL;
  gui::IGUIEnvironment*   OWClient::guienv_ = NULL;
  io::IFileSystem*        OWClient::fsystem_ = NULL;

  Properties*             OWClient::properties_ = NULL;
  MYPArchiveLoader*       OWClient::myp_archive_loader_ = NULL;

  int OWClient::init( const c8* config_file_name ) {

    /** Starting logging system */
    Log::open("./log.txt");
    properties_ = new Properties();

    /** Reading configuration file **/
    Properties properties;
    int res = properties.load(config_file_name);
    if(res) return res;
    return 0;

    /** Initializing Irrlicht subsystems **/
    device_ = createDevice( video::EDT_SOFTWARE, core::dimension2d<u32>(640, 480), 16,
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
    smgr_->addCameraSceneNode(0, core::vector3df(0,30,-40), core::vector3df(0,5,0));

    /** Adding resource files and directories **/
    std::string data_dir = properties.get("data_dir");
    myp_archive_loader_ = new MYPArchiveLoader( fsystem_ );
    fsystem_->addArchiveLoader( myp_archive_loader_ ); 
    fsystem_->addFileArchive( io::path(data_dir.c_str()).append("/").append("art.myp"), false, false, io::EFAT_MYP);
    return 0;
  }

  int OWClient::free() {

    delete myp_archive_loader_;
    device_->drop();
    delete properties_;
    Log::close();
    return 0;
  }

  int OWClient::run() {

/*    std::thread t([ properties ] () mutable {
        char* token;
        if( login( properties->get("user_name").c_str(), 
            properties->get("passwd").c_str(), 
            properties->get("server_ip").c_str(), 
            atoi(properties->get("server_port").c_str()), &token ) ) {
          Log::write("ERROR", "Error logging in");
          return;
        }
        token_ = token;
        ::free(token);
        Log::write("INFO",token_.c_str());
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
