CMAKE_MINIMUM_REQUIRED( VERSION 2.8.3 )
PROJECT( irrlicht )

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAG} -DNO_IRR_USE_NON_SYSTEM_ZLIB_ -fno-rtti -Wall -Wno-unused-variable \
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAG} -fno-rtti -Wall -Wno-unused-variable \
-Wno-unused-function -Wno-switch -Wno-deprecated-declarations -pipe -fno-exceptions -fstrict-aliasing ")	
  SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} " -O3 -fexpensive-optimizations -DPNG_THREAD_UNSAFE_OK -DPNG_NO_MMX_CODE -DPNG_NO_MNG_FEATURES ")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG")	
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -fexpensive-optimizations -O3")

IF( UNIX )
  #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAG} "-Wall -Wno-unused-variable -pipe -fno-exceptions -fno-rtti -fstrict-aliasing")	
  #SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-g -D_DEBUG")	
  #SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-fexpensive-optimizations -O3")
  #SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-O3 -fexpensive-optimizations -DPNG_THREAD_UNSAFE_OK -DPNG_NO_MMX_CODE -DPNG_NO_MNG_FEATURES ")
  IF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    MESSAGE( FATAL_ERROR "Linux32 is Unsupported" )
  ENDIF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
  SET(PLATFORM "linux64" )
ENDIF( UNIX )

IF( WIN32 ) 

  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -DNO_IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_ -DNO_IRR_COMPILE_WITH_DIRECT3D_9_")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -D__GNUWIN64__ -D_WIN64 -DWIN64 -D_WINDOWS -D_MBCS -D_USRDLL -D__NO_INLINE__")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -D_IRR_STATIC_LIB_")

  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -DNO_IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_ -DNO_IRR_COMPILE_WITH_DIRECT3D_9_")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -D__GNUWIN64__ -D_WIN64 -DWIN64 -D_WINDOWS -D_MBCS -D_USRDLL -D__NO_INLINE__")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -D_IRR_STATIC_LIB_")
  SET(STATIC_LIBRARY_FLAGS " -Wl,--add-stdcall-alias ")
  #IF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
  #  MESSAGE( FATAL_ERROR "Win32 is Unsupported" )
  #ENDIF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
  SET(PLATFORM "windows64" )
ENDIF ( WIN32 )

IF( NOT PLATFORM )
  MESSAGE(FATAL_ERROR "Unable to detect platform")
ENDIF( NOT PLATFORM )

SET( CMAKE_VERBOSE_MAKEFILE TRUE)

FIND_PACKAGE(OpenGL)
#FIND_PACKAGE(ZLIB)

SET( IRR_INCLUDE_DIR 
  ${CMAKE_CURRENT_SOURCE_DIR}/include 
  PARENT_SCOPE )

INCLUDE_DIRECTORIES( ./ )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
#INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/zlib )
#INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/jpeglib )
#INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/libpng )


SET( IRRMESHLOADER ./CBSPMeshFileLoader.cpp
  ./CMD2MeshFileLoader.cpp 
  ./CMD3MeshFileLoader.cpp 
  ./CMS3DMeshFileLoader.cpp 
  ./CB3DMeshFileLoader.cpp 
  ./C3DSMeshFileLoader.cpp 
  ./COgreMeshFileLoader.cpp 
  ./COBJMeshFileLoader.cpp 
  ./CColladaFileLoader.cpp 
  ./CCSMLoader.cpp 
  ./CDMFLoader.cpp 
  ./CLMTSMeshFileLoader.cpp 
  ./CMY3DMeshFileLoader.cpp 
  ./COCTLoader.cpp 
  ./CXMeshFileLoader.cpp 
  ./CIrrMeshFileLoader.cpp 
  ./CSTLMeshFileLoader.cpp 
  ./CLWOMeshFileLoader.cpp 
  ./CPLYMeshFileLoader.cpp 
  ./CSMFMeshFileLoader.cpp )

SET( IRRMESHWRITER ./CColladaMeshWriter.cpp 
  ./CIrrMeshWriter.cpp 
  ./CSTLMeshWriter.cpp 
  ./COBJMeshWriter.cpp
  ./CPLYMeshWriter.cpp )

SET( IRRMESHOBJ   ${IRRMESHLOADER} 
  ${IRRMESHWRITER} 
  ./CSkinnedMesh.cpp 
  ./CBoneSceneNode.cpp 
  ./CMeshSceneNode.cpp
  ./CAnimatedMeshSceneNode.cpp
  ./CAnimatedMeshMD2.cpp
  ./CAnimatedMeshMD3.cpp
  ./CQ3LevelMesh.cpp
  ./CQuake3ShaderSceneNode.cpp
  ./CAnimatedMeshHalfLife.cpp ) 

SET( IRROBJ  ./CBillboardSceneNode.cpp
  ./CCameraSceneNode.cpp 
  ./CDummyTransformationSceneNode.cpp
  ./CEmptySceneNode.cpp
  ./CGeometryCreator.cpp
  ./CLightSceneNode.cpp
  ./CMeshManipulator.cpp
  ./CMetaTriangleSelector.cpp
  ./COctreeSceneNode.cpp
  ./COctreeTriangleSelector.cpp
  ./CSceneCollisionManager.cpp
  ./CSceneManager.cpp
  ./CShadowVolumeSceneNode.cpp
  ./CSkyBoxSceneNode.cpp
  ./CSkyDomeSceneNode.cpp
  ./CTerrainSceneNode.cpp
  ./CTerrainTriangleSelector.cpp
  ./CVolumeLightSceneNode.cpp
  ./CCubeSceneNode.cpp
  ./CSphereSceneNode.cpp
  ./CTextSceneNode.cpp
  ./CTriangleBBSelector.cpp
  ./CTriangleSelector.cpp
  ./CWaterSurfaceSceneNode.cpp
  ./CMeshCache.cpp
  ./CDefaultSceneNodeAnimatorFactory.cpp
  ./CDefaultSceneNodeFactory.cpp
  ./CSceneLoaderIrr.cpp )

SET( IRRPARTICLEOBJ  ./CParticleAnimatedMeshSceneNodeEmitter.cpp
  ./CParticleBoxEmitter.cpp
  ./CParticleCylinderEmitter.cpp
  ./CParticleMeshEmitter.cpp
  ./CParticlePointEmitter.cpp
  ./CParticleRingEmitter.cpp
  ./CParticleSphereEmitter.cpp
  ./CParticleAttractionAffector.cpp
  ./CParticleFadeOutAffector.cpp
  ./CParticleGravityAffector.cpp
  ./CParticleRotationAffector.cpp
  ./CParticleSystemSceneNode.cpp
  ./CParticleScaleAffector.cpp )

SET( IRRANIMOBJ ./CSceneNodeAnimatorCameraFPS.cpp
  ./CSceneNodeAnimatorCameraMaya.cpp
  ./CSceneNodeAnimatorCollisionResponse.cpp
  ./CSceneNodeAnimatorDelete.cpp
  ./CSceneNodeAnimatorFlyCircle.cpp
  ./CSceneNodeAnimatorFlyStraight.cpp
  ./CSceneNodeAnimatorFollowSpline.cpp
  ./CSceneNodeAnimatorRotation.cpp
  ./CSceneNodeAnimatorTexture.cpp )

SET( IRRDRVRGLOBJ   ./CNullDriver.cpp
  ./COpenGLDriver.cpp
  ./COpenGLNormalMapRenderer.cpp
  ./COpenGLParallaxMapRenderer.cpp
  ./COpenGLShaderMaterialRenderer.cpp
  ./COpenGLTexture.cpp
  ./COpenGLSLMaterialRenderer.cpp
  ./COpenGLExtensionHandler.cpp
  )

SET( IRRDRVRD3DOBJ  ./CD3D8Driver.cpp
  ./CD3D8NormalMapRenderer.cpp
  ./CD3D8ParallaxMapRenderer.cpp
  ./CD3D8ShaderMaterialRenderer.cpp
  ./CD3D8Texture.cpp
  ./CD3D9Driver.cpp
  ./CD3D9HLSLMaterialRenderer.cpp
  ./CD3D9NormalMapRenderer.cpp
  ./CD3D9ParallaxMapRenderer.cpp
  ./CD3D9ShaderMaterialRenderer.cpp
  ./CD3D9Texture.cpp )

SET( IRRIMAGEOBJ  ./CColorConverter.cpp
  ./CImage.cpp
  ./CImageLoaderBMP.cpp
  ./CImageLoaderDDS.cpp
  ./CImageLoaderJPG.cpp
  ./CImageLoaderPCX.cpp
  ./CImageLoaderPNG.cpp
  ./CImageLoaderPSD.cpp
  ./CImageLoaderTGA.cpp
  ./CImageLoaderPPM.cpp
  ./CImageLoaderWAL.cpp
  ./CImageLoaderRGB.cpp 
  ./CImageWriterBMP.cpp 
  ./CImageWriterJPG.cpp 
  ./CImageWriterPCX.cpp
  ./CImageWriterPNG.cpp
  ./CImageWriterPPM.cpp
  ./CImageWriterPSD.cpp
  ./CImageWriterTGA.cpp )

SET( IRRVIDEOOBJ  ./CVideoModeList.cpp 
  ./CFPSCounter.cpp 
  #  ${IRRDRVRD3DOBJ} 
  ${IRRDRVRGLOBJ} 
  ${IRRIMAGEOBJ} )

SET( IRRSWRENDEROBJ  ./CSoftwareDriver.cpp 
  ./CSoftwareTexture.cpp 
  ./CTRFlat.cpp 
  ./CTRFlatWire.cpp 
  ./CTRGouraud.cpp 
  ./CTRGouraudWire.cpp 
  ./CTRNormalMap.cpp 
  ./CTRStencilShadow.cpp 
  ./CTRTextureFlat.cpp 
  ./CTRTextureFlatWire.cpp 
  ./CTRTextureGouraud.cpp 
  ./CTRTextureGouraudAdd.cpp 
  ./CTRTextureGouraudNoZ.cpp 
  ./CTRTextureGouraudWire.cpp 
  ./CZBuffer.cpp 
  ./CTRTextureGouraudVertexAlpha2.cpp 
  ./CTRTextureGouraudNoZ2.cpp 
  ./CTRTextureLightMap2_M2.cpp 
  ./CTRTextureLightMap2_M4.cpp 
  ./CTRTextureLightMap2_M1.cpp 
  ./CSoftwareDriver2.cpp 
  ./CSoftwareTexture2.cpp 
  ./CTRTextureGouraud2.cpp 
  ./CTRGouraud2.cpp 
  ./CTRGouraudAlpha2.cpp 
  ./CTRGouraudAlphaNoZ2.cpp 
  ./CTRTextureDetailMap2.cpp 
  ./CTRTextureGouraudAdd2.cpp 
  ./CTRTextureGouraudAddNoZ2.cpp 
  ./CTRTextureWire2.cpp 
  ./CTRTextureLightMap2_Add.cpp 
  ./CTRTextureLightMapGouraud2_M4.cpp 
  ./IBurningShader.cpp 
  ./CTRTextureBlend.cpp 
  ./CTRTextureGouraudAlpha.cpp 
  ./CTRTextureGouraudAlphaNoZ.cpp 
  ./CDepthBuffer.cpp 
  ./CBurningShader_Raster_Reference.cpp )

SET( IRRIOOBJ ./CFileList.cpp 
  ./CFileSystem.cpp 
  ./CLimitReadFile.cpp 
  ./CMemoryFile.cpp 
  ./CReadFile.cpp 
  ./CWriteFile.cpp 
  ./CXMLReader.cpp 
  ./CXMLWriter.cpp
  ./CWADReader.cpp
  ./CZipReader.cpp
  ./CPakReader.cpp
  ./CMYPReader.cpp
  ./MYPHash.cpp
  ./CMYPFileDatabase.cpp
  ./CNPKReader.cpp
  ./CTarReader.cpp
  ./CMountPointReader.cpp
  ./irrXML.cpp
  ./CAttributes.cpp
  ./lzma/LzmaDec.c )

SET( IRROTHEROBJ  ./CIrrDeviceSDL.cpp
  ./CIrrDeviceLinux.cpp
  ./CIrrDeviceConsole.cpp
  ./CIrrDeviceStub.cpp
  ./CIrrDeviceWin32.cpp
  ./CIrrDeviceFB.cpp
  ./CLogger.cpp
  ./COSOperator.cpp
  ./Irrlicht.cpp 
  ./os.cpp )

SET( IRRGUIOBJ    ./CGUIButton.cpp
  ./CGUICheckBox.cpp
  ./CGUIComboBox.cpp 
  ./CGUIContextMenu.cpp 
  ./CGUIEditBox.cpp 
  ./CGUIEnvironment.cpp 
  ./CGUIFileOpenDialog.cpp 
  ./CGUIFont.cpp 
  ./CGUIImage.cpp 
  ./CGUIInOutFader.cpp 
  ./CGUIListBox.cpp 
  ./CGUIMenu.cpp 
  ./CGUIMeshViewer.cpp 
  ./CGUIMessageBox.cpp 
  ./CGUIModalScreen.cpp 
  ./CGUIScrollBar.cpp 
  ./CGUISpinBox.cpp 
  ./CGUISkin.cpp 
  ./CGUIStaticText.cpp 
  ./CGUITabControl.cpp 
  ./CGUITable.cpp 
  ./CGUIToolBar.cpp 
  ./CGUIWindow.cpp 
  ./CGUIColorSelectDialog.cpp 
  ./CDefaultGUIElementFactory.cpp 
  ./CGUISpriteBank.cpp 
  ./CGUIImageList.cpp 
  ./CGUITreeView.cpp )

SET( ZLIBOBJ  ./zlib/adler32.c 
  ./zlib/compress.c 
  ./zlib/crc32.c
  ./zlib/deflate.c
  ./zlib/inffast.c 
  ./zlib/inflate.c
  ./zlib/inftrees.c
  ./zlib/trees.c 
  ./zlib/uncompr.c
  ./zlib/zutil.c )

SET( JPEGLIBOBJ ./jpeglib/jcapimin.c 
  ./jpeglib/jcapistd.c 
  ./jpeglib/jccoefct.c 
  ./jpeglib/jccolor.c 
  ./jpeglib/jcdctmgr.c 
  ./jpeglib/jchuff.c 
  ./jpeglib/jcinit.c 
  ./jpeglib/jcmainct.c 
  ./jpeglib/jcmarker.c 
  ./jpeglib/jcmaster.c 
  ./jpeglib/jcomapi.c 
  ./jpeglib/jcparam.c 
  ./jpeglib/jcprepct.c 
  ./jpeglib/jcsample.c 
  ./jpeglib/jctrans.c 
  ./jpeglib/jdapimin.c 
  ./jpeglib/jdapistd.c 
  ./jpeglib/jdatadst.c 
  ./jpeglib/jdatasrc.c 
  ./jpeglib/jdcoefct.c 
  ./jpeglib/jdcolor.c 
  ./jpeglib/jddctmgr.c 
  ./jpeglib/jdhuff.c 
  ./jpeglib/jdinput.c 
  ./jpeglib/jdmainct.c 
  ./jpeglib/jdmarker.c 
  ./jpeglib/jdmaster.c
  ./jpeglib/jdmerge.c
  ./jpeglib/jdpostct.c
  ./jpeglib/jdsample.c
  ./jpeglib/jdtrans.c 
  ./jpeglib/jerror.c 
  ./jpeglib/jfdctflt.c 
  ./jpeglib/jfdctfst.c 
  ./jpeglib/jfdctint.c 
  ./jpeglib/jidctflt.c 
  ./jpeglib/jidctfst.c 
  ./jpeglib/jidctint.c 
  ./jpeglib/jmemmgr.c 
  ./jpeglib/jmemnobs.c 
  ./jpeglib/jquant1.c 
  ./jpeglib/jquant2.c 
  ./jpeglib/jutils.c
  ./jpeglib/jcarith.c 
  ./jpeglib/jdarith.c 
  ./jpeglib/jaricom.c )

SET( LIBPNGOBJ  ./libpng/png.c 
  ./libpng/pngerror.c 
  ./libpng/pngget.c 
  ./libpng/pngmem.c 
  ./libpng/pngpread.c 
  ./libpng/pngread.c 
  ./libpng/pngrio.c 
  ./libpng/pngrtran.c 
  ./libpng/pngrutil.c 
  ./libpng/pngset.c 
  ./libpng/pngtrans.c 
  ./libpng/pngwio.c 
  ./libpng/pngwrite.c 
  ./libpng/pngwtran.c 
  ./libpng/pngwutil.c )

SET( LIBAESGM ./aesGladman/aescrypt.cpp
  ./aesGladman/aeskey.cpp 
  ./aesGladman/aestab.cpp 
  ./aesGladman/fileenc.cpp 
  ./aesGladman/hmac.cpp 
  ./aesGladman/prng.cpp 
  ./aesGladman/pwd2key.cpp 
  ./aesGladman/sha1.cpp 
  ./aesGladman/sha2.cpp )

SET ( BZIP2OBJ ./bzip2/blocksort.c 
  ./bzip2/huffman.c 
  ./bzip2/crctable.c 
  ./bzip2/randtable.c 
  ./bzip2/bzcompress.c 
  ./bzip2/decompress.c 
  ./bzip2/bzlib.c )

SET( SOURCE_FILES   ${IRRMESHOBJ} 
  ${IRROBJ} 
  ${IRRPARTICLEOBJ} 
  ${IRRANIMOBJ}
  ${IRRVIDEOOBJ} 
  ${IRRSWRENDEROBJ} 
  ${IRRIOOBJ} 
  ${IRROTHEROBJ}
  ${IRRGUIOBJ} 
  ${ZLIBOBJ} 
  ${JPEGLIBOBJ} 
  ${LIBPNGOBJ} 
  ${LIBAESGM}
  ${BZIP2OBJ} 
  ${EXTRAOBJ} )


ADD_LIBRARY( irrlicht STATIC ${SOURCE_FILES} )
IF(${PLATFORM} STREQUAL "linux64")
  TARGET_LINK_LIBRARIES(irrlicht ${OPENGL_LIBRARIES} X11 Xxf86vm)
ELSEIF(${PLATFORM} STREQUAL "windows64")
  TARGET_LINK_LIBRARIES(irrlicht gdi32 opengl32 winmm)
ENDIF(${PLATFORM} STREQUAL "linux64")
