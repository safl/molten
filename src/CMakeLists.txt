cmake_minimum_required(VERSION 2.8.3)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(FeatureSummary)
project(nvmenv CXX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNVM_DEBUG_ENABLED")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-memcmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROCKSDB_FALLOCATE_PRESENT")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGFLAGS=gflags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DZLIB")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROCKSDB_MALLOC_USABLE_SIZE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROCKSDB_PTHREAD_ADAPTIVE_MUTEX")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROCKSDB_BACKTRACE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROCKSDB_PLATFORM_POSIX")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROCKSDB_LIB_IO_POSIX")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX")

set(HEADER_FILES
	env_nvm.h)

set(SOURCE_FILES
	e01.cc
	env_nvm.cc
	env_nvm_file.cc
	env_nvm_rfile.cc
	env_nvm_sfile.cc
	env_nvm_wfile.cc)

add_executable(e01 ${HEADER_FILES} ${SOURCE_FILES})
include_directories(/home/safl/git/rocksdb/include)
target_link_libraries(e01 pthread rt gflags z /home/safl/git/rocksdb/librocksdb.a pthread)

install(TARGETS e01 DESTINATION bin)
