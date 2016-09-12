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

class EnvNVM : public Env {
  public:
  EnvNVM(void);

  Status NewLogger(const std::string& fname,
                   shared_ptr<Logger>* result) override;

  Status NewDirectory(const std::string& dname,
                      unique_ptr<Directory>* result) override;

  Status NewSequentialFile(const std::string& fname,
                           unique_ptr<SequentialFile>* result,
                           const EnvOptions& options) override;

  Status NewRandomAccessFile(const std::string& fname,
                             unique_ptr<RandomAccessFile>* result,
                             const EnvOptions& options) override;

  Status NewWritableFile(const std::string& fname,
                         unique_ptr<WritableFile>* result,
                         const EnvOptions& options) override;

  Status GetChildren(const std::string& dname,
                     std::vector<std::string>* result) override;

  Status GetTestDirectory(std::string* dname) override;

  Status DeleteFile(const std::string& fname) override;

  Status CreateDir(const std::string& dname) override;

  Status CreateDirIfMissing(const std::string& dname) override;

  Status DeleteDir(const std::string& dname) override;

  Status FileExists(const std::string& fname) override;

  Status GetFileSize(const std::string& fname, uint64_t* s) override;

  Status GetFileModificationTime(const std::string& fname,
                                 uint64_t* file_mtime) override;

  Status RenameFile(const std::string& fname_s, const std::string& fname_t) override;

  Status LockFile(const std::string& fname, FileLock** lock) override;

  Status UnlockFile(FileLock* lock) override;

  void Schedule(void (*f)(void* arg), void* a, Priority pri,
                void* tag = nullptr, void (*u)(void* arg) = 0) override;

  void StartThread(void (*f)(void*), void* a) override;

  void SleepForMicroseconds(int micros) override;

  void SetBackgroundThreads(int num, Priority pri) override;

  void IncBackgroundThreadsIfNeeded(int num, Priority pri) override;

  Status GetHostName(char* name, uint64_t len) override;

  Status GetCurrentTime(int64_t* unix_time) override;

  Status GetAbsolutePath(const std::string& path,
                         std::string* output_path) override;

  uint64_t NowMicros(void) override;

  std::string TimeToString(uint64_t time) override;

private:
  NVMFile* find(const std::string& fname);

  NVMDirectory* fs_;

};

}  // namespace rocksdb
