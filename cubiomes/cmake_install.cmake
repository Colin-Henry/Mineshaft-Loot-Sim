# Install script for directory: /mnt/d/Seedfinding/MineshaftTesting3/cubiomes

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcubiomes.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcubiomes.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcubiomes.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/libcubiomes.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcubiomes.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcubiomes.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcubiomes.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/libcubiomes_static.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/finders.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/generator.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/layers.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/biomenoise.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/biomes.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/noise.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/terrainnoise.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/rng.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/util.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/quadbase.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/xrms.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/features/stronghold.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/items.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/logging.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_functions.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_table_context.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_table_parser.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/mc_loot.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_bridge_1_16_1.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_bridge_1_16_5.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_bridge_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_other_1_16_1.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_other_1_16_5.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_other_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_other_1_21_1.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/bastion_other_1_21_9.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/buried_treasure_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/buried_treasure_1_18.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/buried_treasure_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/desert_pyramid_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/desert_pyramid_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/desert_pyramid_1_21_6.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/desert_pyramid_1_21_9.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/desert_pyramid_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/end_city_treasure_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/end_city_treasure_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/end_city_treasure_1_21_9.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/end_city_treasure_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/igloo_chest_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/jungle_temple_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/jungle_temple_1_14.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/jungle_temple_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/jungle_temple_1_21_6.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/jungle_temple_1_21_9.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/jungle_temple_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/jungle_temple_dispenser_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/nether_bridge_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/nether_bridge_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/nether_bridge_1_21_9.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/pillager_outpost_1_14.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/pillager_outpost_1_19_2.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/pillager_outpost_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/pillager_outpost_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/ruined_portal_1_16_1.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/ruined_portal_1_21_5.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_map_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_map_1_18.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_map_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_map_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_supply_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_supply_1_14.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_supply_1_17.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_supply_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_supply_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_treasure_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_treasure_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/shipwreck_treasure_1_21_11.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_corridor_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_corridor_1_18.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_corridor_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_corridor_1_21_6.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_corridor_1_21_9.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_crossing_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_library_1_13.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/loot_tables/stronghold_library_1_20.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/loot/cjson/cJSON.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/features/mineshaft.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/features/piece.h"
    "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/features/stronghold.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/mnt/d/Seedfinding/MineshaftTesting3/cubiomes/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
