# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/DanmaKuItDesktop_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/DanmaKuItDesktop_autogen.dir/ParseCache.txt"
  "DanmaKuItDesktop_autogen"
  )
endif()
