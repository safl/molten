#include "rocksdb/env.h"

#ifndef __NVM_DEBUG_H
#define __NVM_DEBUG_H

#include <stdio.h>

#ifdef NVM_DEBUG_ENABLED
	#define NVM_DEBUG(x, ...) printf("%s:%s-%d: " x "\n", __FILE__, \
		__FUNCTION__, __LINE__, ##__VA_ARGS__);fflush(stdout);
#else
	#define NVM_DEBUG(x, ...)
#endif
#endif /* __NVM_DEBUG_H */

namespace rocksdb {

class EnvNVM;
class NVMDirectory;
class NVMFile;
class NVMSequentialFile;
class NVMWritableFile;
class NVMRandomAccessFile;

class NVMFile {
 public:
  explicit NVMFile(Env* env, const std::string& name);

  Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const;
  Status Append(const Slice& data);
  Status PositionedAppend(const Slice& data, uint64_t offset);
  Status Truncate(uint64_t size);
  Status Close(void);
  Status Sync(void);
  Status Fsync(void);

  uint64_t ModifiedTime(void) const;
  uint64_t GetFileSize();
  uint64_t Size(void) const;

  bool IsSyncThreadSafe() const;
  void SetIOPriority(Env::IOPriority pri);
  Env::IOPriority GetIOPriority();

  void SetPreallocationBlockSize(size_t size);
  void GetPreallocationStatus(size_t* block_size, size_t* last_allocated_block);
  size_t GetUniqueId(char* id, size_t max_size) const;
  Status InvalidateCache(size_t offset, size_t length);

  void PrepareWrite(size_t offset, size_t len);

  void Ref(void);
  void Unref(void);

 private:
  // Private since only Unref() should be used to delete it.
  ~NVMFile();
  // No copying allowed.
  NVMFile(const NVMFile&);
  void operator=(const NVMFile&);

  Env* env_;
  const std::string name_;
  int refs_;
};

class NVMDirectory : public Directory {
 public:
  explicit NVMDirectory(std::string name);
  NVMDirectory(void);
  ~NVMDirectory(void);
  Status Fsync(void);
  std::string getName(void);

 private:
  std::string name_;
  std::vector<NVMFile> files_;
  std::vector<NVMDirectory> dirs_;
};

class NVMLogger : public Logger {
 public:
  explicit NVMLogger(const InfoLogLevel log_level = InfoLogLevel::INFO_LEVEL)
      : Logger(log_level) {}

  // Brings overloaded Logv()s into scope so they're not hidden when we override
  // a subset of them.
  using Logger::Logv;

  virtual void Logv(const char* format, va_list ap) override {
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
  }
};

class NVMSequentialFile : public SequentialFile {
public:
  NVMSequentialFile(const std::string& fname, const EnvOptions& options);
  NVMSequentialFile(void);
  ~NVMSequentialFile(void);

  Status Read(size_t n, Slice* result, char* scratch);

  Status Skip(uint64_t n);

  Status InvalidateCache(size_t offset, size_t length);

protected:
  NVMFile *file_;
};

class NVMRandomAccessFile : public RandomAccessFile {
 public:
  NVMRandomAccessFile(const std::string& fname, const EnvOptions& options);
  NVMRandomAccessFile(void);
  ~NVMRandomAccessFile(void);

  Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const;

  bool ShouldForwardRawRequest(void) const;

  void EnableReadAhead(void);

  size_t GetUniqueId(char* id, size_t max_size) const;

  void Hint(AccessPattern pattern);

  Status InvalidateCache(size_t offset, size_t length);

protected:
  NVMFile *file_;
};

class NVMWritableFile : public WritableFile {
public:
  NVMWritableFile(const std::string& fname, const EnvOptions& options);
  NVMWritableFile(void);
  ~NVMWritableFile(void);

  bool UseOSBuffer(void) const override;

  Status Append(const Slice& data) override;

  Status PositionedAppend(const Slice& data, uint64_t offset) override;

  Status Truncate(uint64_t size) override;

  Status Close(void) override;

  Status Flush(void) override;

  Status Sync(void) override;

  Status Fsync(void) override;

  bool IsSyncThreadSafe(void) const override;

  bool UseDirectIO(void) const override;

  size_t GetUniqueId(char* id, size_t max_size) const override;

  Status InvalidateCache(size_t offset, size_t length) override;

  Status RangeSync(uint64_t offset, uint64_t nbytes) override;

  void PrepareWrite(size_t offset, size_t len) override;

protected:

  Status Allocate(uint64_t offset, uint64_t len) override;

private:

  NVMWritableFile(const WritableFile&);   // No copying allowed
  NVMWritableFile(const NVMWritableFile&);
  void operator=(const WritableFile&);
  void operator=(const NVMWritableFile&);

protected:
  NVMFile *file_;

};

/**
 * Environment for non-volatile memory, specifically OpenChannelSSDs accessed
 * via liblightnvm.
 *
 * References:
 *  - include/rocksdb/env.h
 *  - include/rocksdb/utilities/env_librados.h
 *  - include/rocksdb/utilities/env_mirror.h
 *  - hdfs/env_hdfs.h
 */
class EnvNVM : public Env {
 public:
  EnvNVM(void);

  // Create a logger
  //
  // On success, stores a pointer to a new Logger instance in *result
  // and returns OK. On failure stores nullptr in *result and returns non-OK.
  virtual Status NewLogger(
    const std::string& fname,
    shared_ptr<Logger>* result
  ) override;

  // Open a file with sequential read-only access.
  //
  // On success, stores a pointer to a new SequentialFile instance in *result
  // and returns OK. On failure stores nullptr in *result and returns non-OK.
  //
  // NOTE: Single-threaded access is assumed
  virtual Status NewSequentialFile(
    const std::string& fname,
    unique_ptr<SequentialFile>* result,
    const EnvOptions& options
  ) override;

  // Open a file with random read-only access to an existing file.
  //
  // On success, stores a pointer to a new RandomAccessFile instance in *result
  // and returns OK. On failure stores nullptr in *result and returns non-OK.
  //
  // NOTE: Multi-threaded access is assumed
  virtual Status NewRandomAccessFile(
    const std::string& fname,
    unique_ptr<RandomAccessFile>* result,
    const EnvOptions& options
  ) override;

  // Create and open a file with write-access
  //
  // On success, stores a pointer to a new WritableFile instance in *result and
  // returns OK.  On failure stores nullptr in *result and returns non-OK.
  //
  // NOTE: Single-threaded access is assumed.
  // NOTE: If a file with the same name already exists, then the existing file
  // is deleted prior to creation.
  virtual Status NewWritableFile(
    const std::string& fname,
    unique_ptr<WritableFile>* result,
    const EnvOptions& options
  ) override;

  // Rename and open a file with write-access
  //
  // On success, stores a pointer to a new WritableFile instance in *result and
  // returns OK. On failure stores nullptr in *result and returns non-OK.
  //
  // NOTE: Single-threaded access is assumed?
  // NOTE: If a file with the same name already exists, then the existing file
  // is deleted prior to creation.
  virtual Status ReuseWritableFile(
    const std::string& fname,
    const std::string& old_fname,
    unique_ptr<WritableFile>* result,
    const EnvOptions& options
  ) override;

  // Open a directory
  //
  // On success, stores a pointer to a new Directory instance in *result and
  // returns OK. On failure stores nullptr in *result and returns non-OK.
  virtual Status NewDirectory(
    const std::string& name,
    unique_ptr<Directory>* result
  ) override;

  // Returns OK if the named file exists.
  //         NotFound if the named file does not exist,
  //                  the calling process does not have permission to determine
  //                  whether this file exists, or if the path is invalid.
  //         IOError if an IO Error was encountered
  virtual Status FileExists(const std::string& fname) override;

  // Store in *result the names of the children of the specified directory.
  // The names are relative to "dir".
  // Original contents of *results are dropped.
  virtual Status GetChildren(
    const std::string& dname,
    std::vector<std::string>* result
  ) override;

  // Store in *result the attributes of the children of the specified directory.
  // In case the implementation lists the directory prior to iterating the files
  // and files are concurrently deleted, the deleted files will be omitted from
  // result.
  // The name attributes are relative to "dir".
  // Original contents of *results are dropped.
  virtual Status GetChildrenFileAttributes(
    const std::string& dir,
    std::vector<FileAttributes>* result
  ) override;

  // Delete the named file.
  virtual Status DeleteFile(const std::string& fname) override;

  // Create the specified directory. Returns error if directory exists.
  virtual Status CreateDir(const std::string& dirname) override;

  // Creates directory if it does not exist
  // Returns Ok if it exists, or successfully created. Non-OK otherwise.
  virtual Status CreateDirIfMissing(const std::string& dirname) override;

  // Delete the specified directory.
  virtual Status DeleteDir(const std::string& dirname) override;

  // Store the size of fname in *file_size.
  virtual Status GetFileSize(
    const std::string& fname,
    uint64_t* file_size
  ) override;

  // Store the last modification time of fname in *file_mtime.
  virtual Status GetFileModificationTime(
    const std::string& fname,
    uint64_t* file_mtime) override;

  // Rename file src to target.
  virtual Status RenameFile(const std::string& src,
                            const std::string& target) override;

  // Hard Link file src to target.
  virtual Status LinkFile(
    const std::string& src,
    const std::string& target
  ) override {
    return Status::NotSupported("LinkFile is not supported for this Env");
  }

  // Lock the specified file.  Used to prevent concurrent access to
  // the same db by multiple processes.  On failure, stores nullptr in
  // *lock and returns non-OK.
  //
  // On success, stores a pointer to the object that represents the
  // acquired lock in *lock and returns OK.  The caller should call
  // UnlockFile(*lock) to release the lock.  If the process exits,
  // the lock will be automatically released.
  //
  // If somebody else already holds the lock, finishes immediately
  // with a failure.  I.e., this call does not wait for existing locks
  // to go away.
  //
  // May create the named file if it does not already exist.
  virtual Status LockFile(const std::string& fname, FileLock** lock) override;

  // Release the lock acquired by a previous successful call to LockFile.
  // REQUIRES: lock was returned by a successful LockFile() call
  // REQUIRES: lock has not already been unlocked.
  virtual Status UnlockFile(FileLock* lock) override;

  // Arrange to run "(*function)(arg)" once in a background thread, in
  // the thread pool specified by pri. By default, jobs go to the 'LOW'
  // priority thread pool.

  // "function" may run in an unspecified thread.  Multiple functions
  // added to the same Env may run concurrently in different threads.
  // I.e., the caller may not assume that background work items are
  // serialized.
  // When the UnSchedule function is called, the unschedFunction
  // registered at the time of Schedule is invoked with arg as a parameter.
  virtual void Schedule(void (*function)(void* arg), void* arg,
                        Priority pri = LOW, void* tag = nullptr,
                        void (*unschedFunction)(void* arg) = 0) override;

  // Arrange to remove jobs for given arg from the queue_ if they are not
  // already scheduled. Caller is expected to have exclusive lock on arg.
  virtual int UnSchedule(void* arg, Priority pri) override;

  // Start a new thread, invoking "function(arg)" within the new thread.
  // When "function(arg)" returns, the thread will be destroyed.
  virtual void StartThread(void (*function)(void* arg), void* arg) override;

  // Wait for all threads started by StartThread to terminate.
  virtual void WaitForJoin(void) override;

  // Get thread pool queue length for specific thrad pool.
  virtual unsigned int GetThreadPoolQueueLen(Priority pri = LOW) const override;

  // The number of background worker threads of a specific thread pool
  // for this environment. 'LOW' is the default pool.
  // default number: 1
  virtual void SetBackgroundThreads(int number, Priority pri = LOW) override;

  // Enlarge number of background worker threads of a specific thread pool
  // for this environment if it is smaller than specified. 'LOW' is the default
  // pool.
  virtual void IncBackgroundThreadsIfNeeded(int number, Priority pri) override;

  // Lower IO priority for threads from the specified pool.
  virtual void LowerThreadPoolIOPriority(Priority pool = LOW) override;

  // Sleep/delay the thread for the perscribed number of micro-seconds.
  virtual void SleepForMicroseconds(int micros) override;

  // Returns the status of all threads that belong to the current Env.
  virtual Status GetThreadList(
    std::vector<ThreadStatus>* thread_list
  ) override {
    return Status::NotSupported("Not supported.");
  }

  // Returns the pointer to ThreadStatusUpdater.  This function will be
  // used in RocksDB internally to update thread status and supports
  // GetThreadList().
  virtual ThreadStatusUpdater* GetThreadStatusUpdater() const override {
    return thread_status_updater_;
  }

  // Returns the ID of the current thread.
  virtual uint64_t GetThreadID() const override;

  // Returns the number of micro-seconds since some fixed point in time. Only
  // useful for computing deltas of time.
  // However, it is often used as system time such as in GenericRateLimiter
  // and other places so a port needs to return system time in order to work.
  virtual uint64_t NowMicros(void) override;

  // Returns the number of nano-seconds since some fixed point in time. Only
  // useful for computing deltas of time in one run.
  // Default implementation simply relies on NowMicros
  virtual uint64_t NowNanos() override;

  // Get the number of seconds since the Epoch, 1970-01-01 00:00:00 (UTC).
  virtual Status GetCurrentTime(int64_t* unix_time) override;

  // Converts seconds-since-Jan-01-1970 to a printable string
  virtual std::string TimeToString(uint64_t time) override;

  // Get the current host name.
  virtual Status GetHostName(char* name, uint64_t len) override;

  // Get full directory name for this db.
  virtual Status GetAbsolutePath(const std::string& db_path,
      std::string* output_path) override;

  // Generates a unique id that can be used to identify a db
  virtual std::string GenerateUniqueId(void) override;

  // *path is set to a temporary directory that can be used for testing. It may
  // or many not have just been created. The directory may or may not differ
  // between runs of the same process, but subsequent calls will return the
  // same directory.
  virtual Status GetTestDirectory(std::string* path) override;

 private:
  // No copying allowed
  EnvNVM(const Env&);
  void operator=(const Env&);

  NVMFile* find(const std::string& fname);

  NVMDirectory* fs_;

};

}  // namespace rocksdb
