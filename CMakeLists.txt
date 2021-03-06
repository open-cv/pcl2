### ---[ PCL global CMake
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "possible configurations" FORCE)
# In case the user does not setup CMAKE_BUILD_TYPE, assume it's RelWithDebInfo
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "build type default to RelWithDebInfo, set to Release to improve performance" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")

project(PCL)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

# ---[ Release/Debug specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_definitions("-DBOOST_DISABLE_ASSERTS -DEIGEN_NO_DEBUG")
endif()
if(WIN32 AND NOT DEFINED CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-gd")
endif()

# ---[ Android check
if (ANDROID_NDK)
  set (PCL_SHARED_LIBS OFF)
  message ("PCL shared libs on Android must be: ${PCL_SHARED_LIBS}")
endif()

include(${PCL_SOURCE_DIR}/cmake/pcl_options.cmake)

# ---[ Unix/Darwin/Windows specific flags
if(UNIX OR APPLE)
  add_definitions("-Wall -Wl,--no-undefined -fno-strict-aliasing -pthread")
  if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions("-Wno-unknown-pragmas")
  endif(CMAKE_COMPILER_IS_GNUCXX)
else(UNIX OR APPLE)
  if(MSVC)
    add_definitions ("-W0 -DBOOST_ALL_NO_LIB -D_SCL_SECURE_NO_WARNINGS /bigobj -DNOMINMAX")
    if(MSVC90 OR MSVC10)
      add_definitions("/MP")
    endif(MSVC90 OR MSVC10)
  else(MSVC)
    add_definitions("-Wall -Wl,--no-undefined -fno-strict-aliasing")
    if(PCL_SHARED_LIBS)
      add_definitions("-Wl,--export-all-symbols -Wl,--enable-auto-import")
    else(PCL_SHARED_LIBS)
      add_definitions("-DBOOST_LIB_DIAGNOSTIC -DBOOST_THREAD_USE_LIB")
    endif(PCL_SHARED_LIBS)
  endif(MSVC)
endif()
if (__COMPILER_PATHSCALE)
  add_definitions("-Wno-uninitialized -zerouv -pthread -mp")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions("-std=gnu++0x")
endif()

# ---[ Project folders
option(USE_PROJECT_FOLDERS "Use folders to organize PCL projects in an IDE." OFF)
mark_as_advanced(USE_PROJECT_FOLDERS)
if(USE_PROJECT_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif(USE_PROJECT_FOLDERS)

include(${PCL_SOURCE_DIR}/cmake/pcl_utils.cmake)
set(PCL_VERSION 2.0.0 CACHE STRING "PCL version")
DISSECT_VERSION()
GET_OS_INFO()
SET_INSTALL_DIRS()

if(WIN32)
  set(PCL_RESOURCES_DIR ${PCL_SOURCE_DIR}/resources)
  set(PCL_POINTCLOUDS_DIR ${PCL_RESOURCES_DIR}/pointclouds)
endif(WIN32)

set(PCL_OUTPUT_LIB_DIR ${PCL_BINARY_DIR}/${LIB_INSTALL_DIR})
set(PCL_OUTPUT_BIN_DIR ${PCL_BINARY_DIR}/${BIN_INSTALL_DIR})
make_directory(${PCL_OUTPUT_LIB_DIR})
make_directory(${PCL_OUTPUT_BIN_DIR})
if(WIN32)
  foreach(config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${config} CONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "${PCL_OUTPUT_LIB_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${PCL_OUTPUT_BIN_DIR}")
    # ---[ Windows requires DLLs (shared libraries) to be installed in the same directory as executables
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "${PCL_OUTPUT_BIN_DIR}")
  endforeach(config)
else(WIN32)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PCL_OUTPUT_LIB_DIR}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PCL_OUTPUT_BIN_DIR}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PCL_OUTPUT_LIB_DIR}")
endif(WIN32)

# Add an "uninstall" target
configure_file("${PCL_SOURCE_DIR}/cmake/uninstall_target.cmake.in"
               "${PCL_BINARY_DIR}/uninstall_target.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P
                  "${PCL_BINARY_DIR}/uninstall_target.cmake")

###
# this is copy paste form http://www.itk.org/Wiki/CMake_RPATH_handling
# in order to always make a full statement RPATH
###
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}" is_system_dir)
IF("${is_system_dir}" STREQUAL "-1")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
ENDIF("${is_system_dir}" STREQUAL "-1")

### ---[ Find universal dependencies
# SSE flags
include(${PCL_SOURCE_DIR}/cmake/pcl_find_sse.cmake)
PCL_CHECK_FOR_SSE()
# Boost (required)
include(${PCL_SOURCE_DIR}/cmake/pcl_find_boost.cmake)
# Eigen (required)
find_package(Eigen REQUIRED)
include_directories(${EIGEN_INCLUDE_DIRS})
add_definitions(-DEIGEN_USE_NEW_STDVECTOR
                -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET)

# PCL 1.1
find_package(PCL 1.1 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS} include/)
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# FLANN (required)
find_package(Flann REQUIRED)
include_directories(${FLANN_INCLUDE_DIRS})
# OpenMP
include(${PCL_SOURCE_DIR}/cmake/pcl_openmp.cmake)
# OpenNI
find_package(OpenNI)
# Qhull
#find_package(Qhull)
# Find VTK
find_package(VTK)
if(VTK_FOUND)
  set(VTK_FOUND TRUE)
  message(STATUS "VTK found (include: ${VTK_INCLUDE_DIRS}, lib: ${VTK_LIBRARY_DIRS})")
  link_directories(${VTK_LIBRARY_DIRS})
  set(HAVE_VTK ON)
endif()
# Find MPI
find_package(MPI)
if(MPI_CXX_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
endif(MPI_CXX_FOUND)
# Find QT4
find_package(Qt4)
if (QT4_FOUND)
  include(${QT_USE_FILE})
endif (QT4_FOUND)
#Find Doxygen and html help compiler if any
find_package(Doxygen)
if(DOXYGEN_FOUND)
  find_package(HTMLHelp)
endif(DOXYGEN_FOUND)

### ---[ Create the config.h file
set(pcl_config_h_in "${CMAKE_CURRENT_SOURCE_DIR}/pcl2_config.h.in")
set(pcl_config_h "${CMAKE_CURRENT_BINARY_DIR}/include/pcl/pcl2_config.h")
configure_file(${pcl_config_h_in} ${pcl_config_h})
PCL_ADD_INCLUDES(common "" ${pcl_config_h})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

### ---[ Set up for tests
include(${PCL_SOURCE_DIR}/cmake/pcl_tests.cmake)

### ---[ Set up for examples
include(${PCL_SOURCE_DIR}/cmake/pcl_examples.cmake)

### ---[ Add the libraries subdirectories
include(${PCL_SOURCE_DIR}/cmake/pcl_targets.cmake)

collect_subproject_directory_names(${PCL_SOURCE_DIR} "CMakeLists.txt" PCL_MODULES_NAMES PCL_MODULES_DIRS doc)
set(PCL_MODULES_NAMES_UNSORTED ${PCL_MODULES_NAMES})
topological_sort(PCL_MODULES_NAMES PCL_ _DEPENDS)
sort_relative(PCL_MODULES_NAMES_UNSORTED PCL_MODULES_NAMES PCL_MODULES_DIRS)
foreach(subdir ${PCL_MODULES_DIRS})
  add_subdirectory(${PCL_SOURCE_DIR}/${subdir})
endforeach(subdir)

### ---[ Documentation
#add_subdirectory(doc)

### ---[ Configure PCLConfig.cmake
include(${PCL_SOURCE_DIR}/cmake/pcl_pclconfig.cmake)

### ---[ Package creation
include(${PCL_SOURCE_DIR}/cmake/pcl_all_in_one_installer.cmake)
include(${PCL_SOURCE_DIR}/cmake/pcl_cpack.cmake)

if(CPACK_GENERATOR)
  message(STATUS "Found CPack generators: ${CPACK_GENERATOR}")
  PCL_MAKE_CPACK_INPUT()
  set(CPACK_PROJECT_CONFIG_FILE "${PCL_CPACK_CFG_FILE}")
  include(CPack)
endif(CPACK_GENERATOR)
### ---[ Make a pretty picture of the dependency graph
include(${PCL_SOURCE_DIR}/cmake/dep_graph.cmake)
MAKE_DEP_GRAPH()

### ---[ Finish up
PCL_WRITE_STATUS_REPORT()
PCL_RESET_MAPS()
