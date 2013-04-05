
include(ExternalProject)

set(cfitsio_PREFIX ${TOP}/external_dependencies/cfitsio)
set(cfitsio_md5 99210838af46b7bf2d945ccbb05e3ad3)
set(cfitsio_CMAKE_ARGS -DINCLUDE_DIR=${INCLUDE_DIR} -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${ARCHIVE_OUTPUT_PATH})

if(UNIX)
  set(cfitsio_src ftp://heasarc.gsfc.nasa.gov/software/fitsio/c/cfitsio3340.tar.gz)
else(UNIX)
  set(cfitsio_src ftp://heasarc.gsfc.nasa.gov/software/fitsio/c/cfit3340.zip)
endif(UNIX)

ExternalProject_Add(cfitsio
  PREFIX ${cfitsio_PREFIX}
  URL ${cfitsio_src}
  URL_MD5 ${cfitsio_md5}
  DOWNLOAD_DIR ${TOP}/external_dependencies

  UPDATE_COMMAND ""
  PATCH_COMMAND  ""
  CONFIGURE_COMMAND cd ${cfitsio_PREFIX}/src/cfitsio-build &&
                    ${CMAKE_COMMAND} ${cfitsio_CMAKE_ARGS} -DCMAKE_BUILD_TYPE=Release ../..
  BUILD_COMMAND cd ${cfitsio_PREFIX}/src/cfitsio-build && make
  INSTALL_COMMAND  ""
  )

set(cfitsio_LIBRARIES ${ARCHIVE_OUTPUT_PATH}/libcfitsio.a)
