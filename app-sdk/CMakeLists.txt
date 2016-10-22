cmake_minimum_required(VERSION 2.8)

project(HOS)

set(PROVIDEO_DEPENDS_FOLDER "D:/External" CACHE PATH "Proline Video Analytics Dependency Folder Path")

# caffe
set(BOOST_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/boost_1_61_0)
set(BOOST_LIBS ${PROVIDEO_DEPENDS_FOLDER}/boost_1_61_0/lib64-msvc-12.0)
set(GFLAGS_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/gflags/include)
set(GLOG_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/google-glog/include)
set(OPENBLAS_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/xianyi-OpenBLAS-3f6398a)
set(CAFFE_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/caffe/include)
set(CAFFE_LIBS ${PROVIDEO_DEPENDS_FOLDER}/caffe/lib)
set(CAFFE_BINS ${PROVIDEO_DEPENDS_FOLDER}/caffe/bin)

set(CAFFE_LINK_DEBUG debug "caffe-d" debug "proto-d" debug "libboost_system-vc120-mt-gd-1_61" debug "libboost_thread-vc120-mt-gd-1_61" debug "libboost_filesystem-vc120-mt-gd-1_61" debug "libboost_chrono-vc120-mt-gd-1_61" debug "libboost_date_time-vc120-mt-gd-1_61")
set(CAFFE_LINK_RELEASE optimized "caffe" optimized "proto" optimized "libboost_system-vc120-mt-1_61" optimized "libboost_thread-vc120-mt-1_61" optimized "libboost_filesystem-vc120-mt-1_61" optimized "libboost_chrono-vc120-mt-1_61" optimized "libboost_date_time-vc120-mt-1_61")
# set(CAFFE_LINK_DEBUG caffe-d proto-d)
# set(CAFFE_LINK_RELEASE caffe proto)

# zeromq
set(ZEROMQ_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/zeromq_secure/x64/include)
set(ZEROMQ_LIBS ${PROVIDEO_DEPENDS_FOLDER}/zeromq_secure/x64/lib)
set(ZEROMQ_BINS ${PROVIDEO_DEPENDS_FOLDER}/zeromq_secure/x64/bin)
set(ZEROMQ_LINK libzmq)
set(ZEROMQ_DLL "${ZEROMQ_BINS}/libzmq.dll" "${ZEROMQ_BINS}/libsodium.dll")

# sodium
set(SODIUM_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/libsodium/x64/include)
set(SODIUM_LIBS ${PROVIDEO_DEPENDS_FOLDER}/libsodium/x64/staticlib)
set(SODIUM_BINS ${PROVIDEO_DEPENDS_FOLDER}/libsodium/x64/bin)
set(SODIUM_LINK optimized libsodium-x64-static debug libsodium-x64-static-d)

# opencv
set(OPENCV_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/include)
set(OPENCV_LIBS ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/x64/vc12/lib)	
set(OPENCV_BINS ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/x64/vc12/bin)	
set(OPENCV_LINK_DEBUG debug "opencv_calib3d248d" debug "opencv_contrib248d" debug "opencv_core248d" debug "opencv_features2d248d" debug "opencv_flann248d" debug "opencv_gpu248d" debug "opencv_highgui248d" debug "opencv_imgproc248d" debug "opencv_legacy248d" debug "opencv_ml248d" debug "opencv_nonfree248d" debug "opencv_objdetect248d" debug "opencv_photo248d" debug "opencv_stitching248d" debug "opencv_superres248d" debug "opencv_ts248d" debug "opencv_ts248d" debug "opencv_video248d" debug "opencv_videostab248d")
set(OPENCV_LINK_RELEASE optimized "opencv_calib3d248" optimized "opencv_contrib248" optimized "opencv_core248" optimized "opencv_features2d248" optimized "opencv_flann248" optimized "opencv_gpu248" optimized "opencv_highgui248" optimized "opencv_imgproc248" optimized "opencv_legacy248" optimized "opencv_ml248" optimized "opencv_nonfree248" optimized "opencv_objdetect248" optimized "opencv_photo248" optimized "opencv_stitching248" optimized "opencv_superres248" optimized "opencv_ts248" optimized "opencv_ts248" optimized "opencv_video248" optimized "opencv_videostab248")

# tbb
set(TBB_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/tbb/include)
set(TBB_LIBS ${PROVIDEO_DEPENDS_FOLDER}/tbb/lib/intel64/vc12)
set(TBB_BINS ${PROVIDEO_DEPENDS_FOLDER}/tbb/bin/intel64/vc12)
set(TBB_LINK optimized "tbb" debug "tbb_debug")

# protocol buffers
set(PROTOBUF_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/protobuf/include)
set(PROTOBUF_LIBS ${PROVIDEO_DEPENDS_FOLDER}/protobuf/lib/x64/Md)
set(PROTOBUF_BIN ${PROVIDEO_DEPENDS_FOLDER}/protobuf/bin)
set(PROTOBUF_LINK optimized "libprotobuf" debug "libprotobuf-d")

# openssl
set(OPEN_SSL_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/openssl-1.0.1h-vs2013/include)
set(OPEN_SSL_LIBS ${PROVIDEO_DEPENDS_FOLDER}/openssl-1.0.1h-vs2013/lib64)
set(OPEN_SSL_BINS ${PROVIDEO_DEPENDS_FOLDER}/openssl-1.0.1h-vs2013/bin64)
set(OPEN_SSL_LINK debug "libeay32MTd" optimized "libeay32MT" debug "ssleay32MTd" optimized "ssleay32MT")

add_subdirectory (supervisor)
add_subdirectory (common)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)