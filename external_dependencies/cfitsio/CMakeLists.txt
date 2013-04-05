# encoding: utf-8

cmake_minimum_required(VERSION 2.8)
project(Cfitsio)

function(build_cfitsio)
  set(CFITSIO_LIB_NAME cfitsio)
  set(CFITSIO_SRC_PATH src/cfitsio)
  include_directories(${CFITSIO_SRC_PATH})

  if(NOT INCLUDE_DIR)
    message(FATAL_ERROR "INCLUDE_DIR has not been set")
  endif(NOT INCLUDE_DIR)

  set(INSTALL_HEADERS fitsio.h longnam.h)
  foreach(hdr ${INSTALL_HEADERS})
    configure_file(${CFITSIO_SRC_PATH}/${hdr} ${INCLUDE_DIR}/${hdr} COPYONLY)
  endforeach(hdr ${INSTALL_HEADERS})
  set(INSTALL_HEADERS ${CFITSIO_SRC_PATH}/fitsio.h ${CFITSIO_SRC_PATH}/longnam.h)
  set(cfitsio_defs
    -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1
    -DHAVE_MATH_H=1 -DHAVE_LIMITS_H=1 -DHAVE_LONGLONG=1
    )
  if(UNIX)
    set(cfitsio_defs
      ${cfitsio_defs} -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -DHAVE_FTRUNCATE=1
      )
  endif(UNIX)

  set(cfitsio_Sources
    ${CFITSIO_SRC_PATH}/buffers.c
    ${CFITSIO_SRC_PATH}/cfileio.c
    ${CFITSIO_SRC_PATH}/checksum.c
    ${CFITSIO_SRC_PATH}/drvrfile.c
    ${CFITSIO_SRC_PATH}/drvrmem.c
    ${CFITSIO_SRC_PATH}/drvrnet.c
    ${CFITSIO_SRC_PATH}/drvrsmem.c
    ${CFITSIO_SRC_PATH}/drvrgsiftp.c
    ${CFITSIO_SRC_PATH}/editcol.c
    ${CFITSIO_SRC_PATH}/edithdu.c
    ${CFITSIO_SRC_PATH}/eval_l.c
    ${CFITSIO_SRC_PATH}/eval_y.c
    ${CFITSIO_SRC_PATH}/eval_f.c
    ${CFITSIO_SRC_PATH}/fitscore.c
    ${CFITSIO_SRC_PATH}/getcol.c
    ${CFITSIO_SRC_PATH}/getcolb.c
    ${CFITSIO_SRC_PATH}/getcold.c
    ${CFITSIO_SRC_PATH}/getcole.c
    ${CFITSIO_SRC_PATH}/getcoli.c
    ${CFITSIO_SRC_PATH}/getcolj.c
    ${CFITSIO_SRC_PATH}/getcolk.c
    ${CFITSIO_SRC_PATH}/getcoll.c
    ${CFITSIO_SRC_PATH}/getcols.c
    ${CFITSIO_SRC_PATH}/getcolsb.c
    ${CFITSIO_SRC_PATH}/getcoluk.c
    ${CFITSIO_SRC_PATH}/getcolui.c
    ${CFITSIO_SRC_PATH}/getcoluj.c
    ${CFITSIO_SRC_PATH}/getkey.c
    ${CFITSIO_SRC_PATH}/group.c
    ${CFITSIO_SRC_PATH}/grparser.c
    ${CFITSIO_SRC_PATH}/histo.c
    ${CFITSIO_SRC_PATH}/iraffits.c
    ${CFITSIO_SRC_PATH}/modkey.c
    ${CFITSIO_SRC_PATH}/putcol.c
    ${CFITSIO_SRC_PATH}/putcolb.c
    ${CFITSIO_SRC_PATH}/putcold.c
    ${CFITSIO_SRC_PATH}/putcole.c
    ${CFITSIO_SRC_PATH}/putcoli.c
    ${CFITSIO_SRC_PATH}/putcolj.c
    ${CFITSIO_SRC_PATH}/putcolk.c
    ${CFITSIO_SRC_PATH}/putcoluk.c
    ${CFITSIO_SRC_PATH}/putcoll.c
    ${CFITSIO_SRC_PATH}/putcols.c
    ${CFITSIO_SRC_PATH}/putcolsb.c
    ${CFITSIO_SRC_PATH}/putcolu.c
    ${CFITSIO_SRC_PATH}/putcolui.c
    ${CFITSIO_SRC_PATH}/putcoluj.c
    ${CFITSIO_SRC_PATH}/putkey.c
    ${CFITSIO_SRC_PATH}/region.c
    ${CFITSIO_SRC_PATH}/scalnull.c
    ${CFITSIO_SRC_PATH}/swapproc.c
    ${CFITSIO_SRC_PATH}/wcssub.c
    ${CFITSIO_SRC_PATH}/wcsutil.c
    ${CFITSIO_SRC_PATH}/imcompress.c
    ${CFITSIO_SRC_PATH}/quantize.c
    ${CFITSIO_SRC_PATH}/ricecomp.c
    ${CFITSIO_SRC_PATH}/pliocomp.c
    ${CFITSIO_SRC_PATH}/fits_hcompress.c
    ${CFITSIO_SRC_PATH}/fits_hdecompress.c
    ${CFITSIO_SRC_PATH}/zuncompress.c
    ${CFITSIO_SRC_PATH}/zcompress.c
    ${CFITSIO_SRC_PATH}/adler32.c
    ${CFITSIO_SRC_PATH}/crc32.c
    ${CFITSIO_SRC_PATH}/inffast.c
    ${CFITSIO_SRC_PATH}/inftrees.c
    ${CFITSIO_SRC_PATH}/trees.c
    ${CFITSIO_SRC_PATH}/zutil.c
    ${CFITSIO_SRC_PATH}/deflate.c
    ${CFITSIO_SRC_PATH}/infback.c
    ${CFITSIO_SRC_PATH}/inflate.c
    ${CFITSIO_SRC_PATH}/uncompr.c
    ${CFITSIO_SRC_PATH}/f77_wrap1.c
    ${CFITSIO_SRC_PATH}/f77_wrap2.c
    ${CFITSIO_SRC_PATH}/f77_wrap3.c
    ${CFITSIO_SRC_PATH}/f77_wrap4.c
    )

  set(cfitsio_Headers
    ${CFITSIO_SRC_PATH}/cfortran.h
    ${CFITSIO_SRC_PATH}/crc32.h
    ${CFITSIO_SRC_PATH}/deflate.h
    ${CFITSIO_SRC_PATH}/drvrgsiftp.h
    ${CFITSIO_SRC_PATH}/drvrsmem.h
    ${CFITSIO_SRC_PATH}/eval_defs.h
    ${CFITSIO_SRC_PATH}/eval_tab.h
    ${CFITSIO_SRC_PATH}/f77_wrap.h
    ${CFITSIO_SRC_PATH}/fitsio2.h
    ${CFITSIO_SRC_PATH}/fitsio.h
    ${CFITSIO_SRC_PATH}/fpack.h
    ${CFITSIO_SRC_PATH}/group.h
    ${CFITSIO_SRC_PATH}/grparser.h
    ${CFITSIO_SRC_PATH}/inffast.h
    ${CFITSIO_SRC_PATH}/inffixed.h
    ${CFITSIO_SRC_PATH}/inflate.h
    ${CFITSIO_SRC_PATH}/inftrees.h
    ${CFITSIO_SRC_PATH}/longnam.h
    ${CFITSIO_SRC_PATH}/region.h
    ${CFITSIO_SRC_PATH}/trees.h
    ${CFITSIO_SRC_PATH}/zconf.h
    ${CFITSIO_SRC_PATH}/zlib.h
    ${CFITSIO_SRC_PATH}/zutil.h
    )

  add_library(${CFITSIO_LIB_NAME} ${cfitsio_Sources} ${cfitsio_Headers})
  if(UNIX)
    set_target_properties(${CFITSIO_LIB_NAME}
      PROPERTIES
      COMPILE_FLAGS -fPIC
      )
  endif(UNIX)
endfunction(build_cfitsio)

build_cfitsio()
