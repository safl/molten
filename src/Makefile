CXX?=g++
RDB_LIB?=/home/safl/git/rocksdb/librocksdb.a
RDB_INC?=/home/safl/git/rocksdb/include
RDB_FLAGS?=-DROCKSDB_FALLOCATE_PRESENT -DGFLAGS=gflags -DZLIB \
	   -DROCKSDB_MALLOC_USABLE_SIZE -DROCKSDB_PTHREAD_ADAPTIVE_MUTEX \
	   -DROCKSDB_BACKTRACE -DROCKSDB_PLATFORM_POSIX -DROCKSDB_LIB_IO_POSIX \
	   -DOS_LINUX
CXX_LIBS=-lpthread -lrt -lgflags -lz -lpthread
CXX_FLAGS=-std=c++11 -fno-builtin-memcmp -march=native

all: clean e01 run

e01:
	@mkdir -p bin
	${CXX} env_nvm.cc e01.cc -o bin/e01 \
		${RDB_LIB} -I${RDB_INC} ${RDB_FLAGS} ${CXX_LIBS} ${CXX_FLAGS}

clean:
	@rm -r bin || true

run:
	./bin/e01
