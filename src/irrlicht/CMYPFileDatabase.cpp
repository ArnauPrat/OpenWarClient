
#include <CMYPFileDatabase.h>
#ifdef __IRR_COMPILE_WITH_MYP_ARCHIVE_LOADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CMYPFileDatabase.h"
#include "MYPHash.h"

#include <zlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _IRR_WINDOWS_
#include <windows.h>
#endif 

namespace irr {
  namespace io {

#define MYP_FILE_DESCRIPTOR_SIZE 34
#define MYP_tableBlockheaderSize 12
#define MYP_ARCHIVE_headerSize 16
#define MYP_ARCHIVE_HEADER_START_POSITION 0x0C 
#define MYP_ARCHIVE_HEADER_numFilesPOSITION 0x18
#define MYP_EXPECTED_numFilesPER_BLOCK 1000

#define LINE_BUFFER_BLOCK_SIZE 8

#define MYP_NUM_PREFIXES 4

    static const c8* name_prefixes[MYP_NUM_PREFIXES] = {"sk.0.0.", "it.0.0.", "fi.0.0.", "fg.0.0." };


    /*
     * Struct definitions
     */

    struct tableBlockHeader {

      u32        num_files;
      u64  next_block;

    };


    /*
     * Static function definitions
     */

    static u32 parse_uint( const u8* buffer, u64 offset ) {

      u32 result;
      memcpy(&result,buffer+offset,sizeof(u32));
      return result;

    }

    static u64 parse_ulong( const u8* buffer, u64 offset ) {

      u64 result;
      memcpy(&result,buffer+offset,sizeof(u64));
      return result;

    }


    static void initialize_FileDescriptor(SMYPFileDescriptor* descriptor, const u8* buffer, u32 offset) {

      descriptor->startingPosition   = parse_ulong(buffer, offset + 0);            
      descriptor->headerSize         = parse_uint(buffer, offset + 8);
      descriptor->compressedSize     = parse_uint(buffer, offset + 12);
      descriptor->uncompressedSize   = parse_uint(buffer, offset + 16);
      u32 secondary_hash      = parse_uint(buffer, offset + 20);
      u64 primary_hash        = parse_uint(buffer, offset + 24);
      descriptor->hash = (primary_hash << 32) + secondary_hash;
      descriptor->compressionMethod  = buffer[offset + 32];

    }

    static void initialize_tableBlockHeader( tableBlockHeader* block_header, const u8* buffer ) {

      block_header->num_files  = parse_uint(buffer,0);
      block_header->next_block = parse_ulong(buffer,4);
    }

    /*
     * Class methods
     */

    CMYPFileDatabase::CMYPFileDatabase( IReadFile* file ) : File(file)
    {
      File->grab(); 
    }

    CMYPFileDatabase::~CMYPFileDatabase() {
      File->drop();
    }

    u32 CMYPFileDatabase::loadArchive() {

      u32 fileDescriptorIndex = FileDescriptors.size(); 

      u8 buffer[4];
      u32 numTotalFiles;
      u32 numReadFiles;
      u64 nextTableBlockHeader;


      /* Buffer to store myp archive header  */
      u8 archiveHeaderBuffer[MYP_ARCHIVE_headerSize];

      /* Buffer to store myp table block header  */
      u8 mypTableHeaderBlockBuffer[MYP_tableBlockheaderSize];

      /* Create buffers to store file descriptors. Initial size to 1000 as it is typically observed in practice.  */
      u32 mypFileDescriptorBufferSize = MYP_EXPECTED_numFilesPER_BLOCK * MYP_FILE_DESCRIPTOR_SIZE;
      u8* mypFileDescriptorBuffer = (u8*)malloc(sizeof(u8)*mypFileDescriptorBufferSize);

      /* Create buffers to store file descriptors. Initial size to 1000 as it is typically observed in practice.  */
      SMYPFileDescriptor auxFileDescriptor;

      u32 error_code = 0;

      /* Obtain the position of the first table block */ 
      if(!File->seek(MYP_ARCHIVE_HEADER_START_POSITION,false )) {
        error_code = MYP_ERROR_EOF;
        goto error;
      }

      if(File->read(archiveHeaderBuffer, MYP_ARCHIVE_headerSize) != MYP_ARCHIVE_headerSize ) {
        error_code = MYP_ERROR_EOF;
        goto error; 
      }

      nextTableBlockHeader = parse_ulong(archiveHeaderBuffer, 0 );

      /* Get total number of files in the archive */
      if(!File->seek( MYP_ARCHIVE_HEADER_numFilesPOSITION, false )) {
        error_code = MYP_ERROR_EOF;
        goto error;
      }

      if( File->read(buffer,4) != 4) {
        error_code = MYP_ERROR_EOF;
        goto error;
      }

      numTotalFiles = parse_uint(buffer,0);
      numReadFiles = 0;

      while( nextTableBlockHeader != 0 ) {

        /* We read the next table block header */
        if(!File->seek(nextTableBlockHeader , false )) {

          error_code = MYP_ERROR_EOF;
          goto error;
        }

        if ( File->read(mypTableHeaderBlockBuffer, MYP_tableBlockheaderSize ) !=  MYP_tableBlockheaderSize ) {
          error_code = MYP_ERROR_EOF;
          goto error;
        }

        tableBlockHeader tableBlockHeader;
        initialize_tableBlockHeader(&tableBlockHeader, mypTableHeaderBlockBuffer );

        /* Read file descriptors*/
        if( tableBlockHeader.num_files*MYP_FILE_DESCRIPTOR_SIZE > mypFileDescriptorBufferSize ) { // resizing buffer if needed
          free( mypFileDescriptorBuffer );
          mypFileDescriptorBufferSize = tableBlockHeader.num_files*MYP_FILE_DESCRIPTOR_SIZE;
          mypFileDescriptorBuffer = (u8*)malloc(mypFileDescriptorBufferSize);
        }

        if( (u32)(File->read(mypFileDescriptorBuffer, MYP_FILE_DESCRIPTOR_SIZE * tableBlockHeader.num_files)) !=
            MYP_FILE_DESCRIPTOR_SIZE*tableBlockHeader.num_files ) {
          error_code = MYP_ERROR_EOF;
          goto error;
        }

        for ( u32 i = 0; i < tableBlockHeader.num_files; ++i) {
          initialize_FileDescriptor(&auxFileDescriptor, mypFileDescriptorBuffer, i*MYP_FILE_DESCRIPTOR_SIZE);
          if( auxFileDescriptor.startingPosition > 0 ) {
            FileDescriptors.push_back( auxFileDescriptor );
            numReadFiles++;
          }
        }

        nextTableBlockHeader = tableBlockHeader.next_block;
      }

      if ( numTotalFiles != numReadFiles ){
        error_code = MYP_ERROR_NFILES_MISSMATCH;
        goto error;
      }
      free(mypFileDescriptorBuffer);
      return 0;
error:
      /* We undo the adding of the whole archive */
      return error_code;
    }


    /*u32 CMYPFileDatabase::load_hash_dictionary( const c8* fileName ) {

      FILE* fp = fopen(fileName, "r");

      u32 line_buffer_size = LINE_BUFFER_BLOCK_SIZE;
      c8* line_buffer = (c8*)malloc(sizeof(c8)*LINE_BUFFER_BLOCK_SIZE);

      while( (line_buffer = fgets(line_buffer, line_buffer_size, fp)) ) {
      u32 line_size = strlen(line_buffer);
      while( line_size == (line_buffer_size-1) &&
      line_buffer[line_buffer_size - 2] != '\n' && 
      !feof(fp) ) { //resizing the buffer and filling with more data in case we do not found an eof
      line_buffer_size += LINE_BUFFER_BLOCK_SIZE;
      line_buffer = (c8*) realloc(line_buffer, line_buffer_size );
      fgets(&line_buffer[line_buffer_size - LINE_BUFFER_BLOCK_SIZE - 1], LINE_BUFFER_BLOCK_SIZE + 1, fp);  
      line_size = strlen(line_buffer);
      }
      add_hash_to_filename_entry(line_buffer);
      }
      free(line_buffer);
      fclose(fp);
      return 0;
      }*/

          /*void CMYPFileDatabase::add_hash_to_filename_entry( const c8* line ) {
            c8* next_sharp; 
            u64 ph = strtol(line,&next_sharp,16);
            u64 sh = strtol(next_sharp+1,&next_sharp,16);
            u64 full_hash = (ph << 32) + sh;
            c8* last_sharp = strchr(next_sharp+1,'#');
            hash_to_fileNames_.insert(std::pair<u64, core::stringc > ( full_hash,
            core::stringc(next_sharp+1,last_sharp) ) );
            }*/

        static void findExtension(const u8* data, c8* ext) {

          sprintf(ext,"%s","txt"); 

          c8 buff[4];
          memcpy((u8*)buff, data, sizeof(u8)*4);

          if (buff[0] == 0 && buff[1] == 1 && buff[2] == 0)
          {
            sprintf(ext,"%s","ttf"); 
          }
          else if (buff[0] == 0x0a && buff[1] == 0x05 && buff[2] == 0x01 && buff[3] == 0x08)
          {
            sprintf(ext,"%s","pcx"); 
          }
          else if (strstr(buff,"PK") )
          {
            sprintf(ext,"%s","zip"); 
          }
          else if (strstr(buff,"SCPT") )
          {
            sprintf(ext,"%s","scpt"); 
          }
          else if (strstr(buff,"<") )
          {
            sprintf(ext,"%s","xml"); 
          }
          else if ( strstr((c8*)data, "lua") && ( strstr((c8*)data, "lua") - (const c8*)data) < 50)
          {
            sprintf(ext,"%s","lua"); 
          }
          else if (strstr(buff,"DDS") )
          {
            sprintf(ext,"%s","dds"); 
          }
          else if (strstr(buff,"XSM") )
          {
            sprintf(ext,"%s","xsm"); 
          }
          else if (strstr(buff,"XAC") )
          {
            sprintf(ext,"%s","xac"); 
          }
          else if (strstr(buff,"8BPS") )
          {
            sprintf(ext,"%s","8bps"); 
          }
          else if (strstr(buff,"bdLF") )
          {
            sprintf(ext,"%s","db"); 
          }
          else if (strstr(buff,"gsLF") )
          {
            sprintf(ext,"%s","geom"); 
          }
          else if (strstr(buff,"idLF") )
          {
            sprintf(ext,"%s","diffuse"); 
          }
          else if (strstr(buff,"psLF") )
          {
            sprintf(ext,"%s","specular"); 
          }
          else if (strstr(buff,"amLF") )
          {
            sprintf(ext,"%s","mask"); 
          }
          else if (strstr(buff,"ntLF") )
          {
            sprintf(ext,"%s","tint"); 
          }
          else if (strstr(buff,"lgLF") )
          {
            sprintf(ext,"%s","glow"); 
          }
          else if (strstr((c8*)data, "Gamebry") )
          {
            sprintf(ext,"%s","nif"); 
          }
          else if (strstr((c8*)data,"WMPHOTO") )
          {
            sprintf(ext,"%s","lmp"); 
          }
          else if (strstr(buff,"RIFF") )
          {
            c8 buff2[4];
            memcpy(buff2, &data[8], sizeof(u8)*4);
            if (strstr(buff2,"WAVE") )
            {
              sprintf(ext,"%s","wav"); 
            }
            else
            {
              sprintf(ext,"%s","riff"); 
            }
          }
          else if (strstr(buff,"; Zo") )
          {
            sprintf(ext,"%s","zone.txt"); 
          }
          else if (strstr(buff,"\0\0\0\0") )
          {
            sprintf(ext,"%s","zero.txt"); 
          }
          else if (strstr(buff,"PNG") )
          {
            sprintf(ext,"%s","png"); 
          }
          else if (strstr(buff,"AMX") )
          {
            sprintf(ext,"%s","amx"); 
          }
          else if (strstr(buff,"SIDS") )
          {
            sprintf(ext,"%s","sids"); 
          }
          /* else if (commaNum >= 10)
             {
             ext = "csv";
             }
             */

        }

        u32 CMYPFileDatabase::extract( const io::path& path ) {

          u32 numFiles = FileDescriptors.size();
          for ( u32 i = 0; i < numFiles; ++i ) {

            SMYPFileDescriptor* fileDescriptor = &FileDescriptors[i];
            u32 res = extract(*fileDescriptor,path, io::path());
            if(res) return res;

          }
          return 0;
        }

        u32 CMYPFileDatabase::extract( const io::path& fileName, const io::path& outputFolderName ) {
          u32 ph = 0;
          u32 sh = 0;
          hashlittle2((void*)fileName.c_str(), fileName.size(), &sh, &ph); 
          u64 hash  = ((u64)ph << 32) + sh;
          u64 num_files = FileDescriptors.size();
          for( u64 i = 0; i < num_files; ++i ) {
            SMYPFileDescriptor* descriptor = &FileDescriptors[i];
            if ( descriptor->hash == hash ) {
              return extract(*descriptor, outputFolderName, fileName);
            }
          }
          return MYP_ERROR_FILE_NOT_FOUND;
        }

        u32 CMYPFileDatabase::extract( const SMYPFileDescriptor& fileDescriptor, const io::path& outputFolderName, const io::path& outputFileName ) {

          u64 fileNameBufferSize = sizeof(c8)*1024;;
          c8* fileName = (c8*)malloc(fileNameBufferSize);;
          u8* data;
          u64 dataSize;
          getFileData(fileDescriptor,&data, &dataSize );

          if( outputFileName.size() == 0) {

            c8 ext[16];
            findExtension(data,ext);
#ifdef _IRR_LINUX_PLATFORM_
            sprintf(fileName,"%lX.%s", fileDescriptor.hash, ext);
#endif

#ifdef _IRR_WINDOWS_
            sprintf(fileName,"%I64X.%s", fileDescriptor.hash, ext);
#endif

          } else {
            u64 outputFileNameLength = outputFileName.size() + 1;
            if( outputFileNameLength >= fileNameBufferSize ) {
              fileNameBufferSize = outputFileNameLength;
              fileName = (c8*)realloc( fileName, fileNameBufferSize );
            }
            strcpy(fileName,outputFileName.c_str());
          }

          u32 pathLength = outputFolderName.size();
          u32 fileNameSize = strlen(fileName);
          u32 finalFileNameLength = pathLength+fileNameSize;
          c8* finalFileName = (c8*)malloc(sizeof(c8*)*finalFileNameLength + 1);
          sprintf(finalFileName , "%s/%s", outputFolderName.c_str(), fileName);

          /* Creating folders from path */
          c8* subPath = (c8*)malloc(sizeof(c8)*finalFileNameLength+1);
#ifdef _IRR_LINUX_PLATFORM_
          c8 SEPARATOR = '/';
#endif

#ifdef _IRR_WINDOWS_
          c8 SEPARATOR = '\\';
#endif
          c8* nextToken = strchr((c8*)finalFileName,SEPARATOR);
          while(nextToken) {
            nextToken++;
            u32 end = nextToken - finalFileName;
            strncpy(subPath, finalFileName, end );
            subPath[end] = '\0';
#ifdef _IRR_LINUX_PLATFORM_
            mkdir((c8*)subPath, S_IRWXU);
#endif
#ifdef _IRR_WINDOWS_
            CreateDirectory((c8*)subPath, NULL);
#endif
            nextToken = strchr(nextToken,SEPARATOR);
          }

          FILE* wfp = fopen(finalFileName, "wb");
          if( wfp ) {
            fwrite(data, fileDescriptor.uncompressedSize, 1, wfp);
            fclose(wfp);
          } else {
            fprintf(stderr, "ERROR creating file %s\n", finalFileName );
          }
          freeFileData(data);
          free(fileName);
          return 0;
        }

        SMYPFileDatabaseStats CMYPFileDatabase::getStats() const {
          SMYPFileDatabaseStats stats;
          stats.numFiles = FileDescriptors.size();
          return stats;
        }

        u32 CMYPFileDatabase::getFileData(const io::path& fileName, u8** data, u64* data_size) {
          *data = NULL;
          u32 ph = 0;
          u32 sh = 0;
          hashlittle2(fileName.c_str(), fileName.size(), &sh, &ph); 
          u64 hash  = ((u64)ph << 32) + sh;
          u64 numFiles = FileDescriptors.size();
          for( u64 i = 0; i < numFiles; ++i ) {
            SMYPFileDescriptor* descriptor = &FileDescriptors[i];
            if ( descriptor->hash == hash ) {
              return getFileData(*descriptor, data, data_size );
            }
          }
          return 1;
        }

        u32 CMYPFileDatabase::getFileData(const SMYPFileDescriptor& fileDescriptor, u8** data, u64* dataSize) {

          *dataSize = fileDescriptor.uncompressedSize;
          *data = (u8*)malloc(sizeof(u8)*fileDescriptor.uncompressedSize);;

          if( fileDescriptor.compressionMethod != 0 ) { /* It is compressed */
            u8* compressed_buffer = (u8*)malloc(sizeof(u8)*fileDescriptor.compressedSize);
            File->seek(fileDescriptor.startingPosition + fileDescriptor.headerSize, false);
            File->read(compressed_buffer, sizeof(u8)*fileDescriptor.compressedSize);
            z_stream zstr;
            zstr.next_in = compressed_buffer;
            zstr.avail_in = fileDescriptor.compressedSize;
            zstr.next_out = *data;
            zstr.avail_out = fileDescriptor.uncompressedSize;
            zstr.zalloc = Z_NULL;
            zstr.zfree = Z_NULL;
            zstr.opaque = Z_NULL;
            if ( inflateInit(&zstr) != Z_OK ) fprintf(stderr, "Error when initializing zlib\n");
            u32 ret = inflate(&zstr,Z_FINISH) ; 
            if ( ret != Z_OK && ret != Z_STREAM_END ) {
              fprintf(stderr, "Error when inflating compressed buffer: %d\n", ret);
            }
            if ( inflateEnd(&zstr) != Z_OK ) fprintf(stderr, "Error when ending zlib\n");
            free(compressed_buffer);
          } else {
            File->seek(fileDescriptor.startingPosition + fileDescriptor.headerSize, false);
            File->read(*data, sizeof(u8)*fileDescriptor.uncompressedSize);
          }
          return 0;
        }

        u32 CMYPFileDatabase::freeFileData( u8* data) {
          free(data);
          return 0;
        }
  } //end namespace io 
} //end namespace irr
#endif
