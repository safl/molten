#include "rocksdb/env.h"

namespace rocksdb {

class NVMSequentialFile : public SequentialFile {
public:
  NVMSequentialFile(const std::string& fname,
                    const EnvOptions& options);
  NVMSequentialFile(void);
  ~NVMSequentialFile(void);

  Status Read(size_t n, Slice* result, char* scratch);

  Status Skip(uint64_t n);

  Status InvalidateCache(size_t offset, size_t length);

protected:
  std::string fname_;

};

class NVMRandomAccessFile : public RandomAccessFile {
 public:
  NVMRandomAccessFile(const std::string& fname,
                      const EnvOptions& options);
  NVMRandomAccessFile(void);
  ~NVMRandomAccessFile(void);

  Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const;

  bool ShouldForwardRawRequest(void) const;

  void EnableReadAhead(void);

  size_t GetUniqueId(char* id, size_t max_size) const;

  void Hint(AccessPattern pattern);

  Status InvalidateCache(size_t offset, size_t length);

protected:
  std::string fname_;
};

class NVMWritableFile {
public:
  NVMWritableFile(const std::string& fname,
                  const EnvOptions& options);

  NVMWritableFile(void);
  ~NVMWritableFile(void);

  bool UseOSBuffer(void) const;

  Status Append(const Slice& data);

  Status PositionedAppend(const Slice& data, uint64_t offset);

  Status Truncate(uint64_t size);

  Status Close();

  Status Flush();

  Status Sync();

  Status Fsync();

  bool IsSyncThreadSafe() const;

  bool UseDirectIO() const;

  size_t GetUniqueId(char* id, size_t max_size) const;

  Status InvalidateCache(size_t offset, size_t length);

  Status RangeSync(uint64_t offset, uint64_t nbytes);

  void PrepareWrite(size_t offset, size_t len);

protected:

  Status Allocate(uint64_t offset, uint64_t len);

private:

  NVMWritableFile(const WritableFile&);   // No copying allowed
  NVMWritableFile(const NVMWritableFile&);
  void operator=(const WritableFile&);
  void operator=(const NVMWritableFile&);

};

class NVMDirectory : public Directory {
public:

  NVMDirectory(const std::string& name);
  ~NVMDirectory() {}

  // Fsync directory. Can be called concurrently from multiple threads.
  Status Fsync();
};

class NVMLogger {
public:
  NVMLogger(const InfoLogLevel log_level = InfoLogLevel::INFO_LEVEL);
  ~NVMLogger();

  void LogHeader(const char* format, va_list ap);
  void Logv(const char* format, va_list ap);
  void Logv(const InfoLogLevel log_level, const char* format, va_list ap);

  size_t GetLogFileSize() const;
  InfoLogLevel GetInfoLogLevel() const;
  void SetInfoLogLevel(const InfoLogLevel log_level);

  void Flush();

private:
  // No copying allowed
  NVMLogger(const NVMLogger&);
  NVMLogger(const Logger&);
  void operator=(const NVMLogger&);
  void operator=(const Logger&);
};

class EnvNVM : public Env {

  Status NewSequentialFile(const std::string& fname,
                           unique_ptr<SequentialFile>* result,
                           const EnvOptions& options);

  Status NewRandomAccessFile(const std::string& fname,
                             unique_ptr<RandomAccessFile>* result,
                             const EnvOptions& options);

  Status NewWritableFile(const std::string& fname,
                         unique_ptr<WritableFile>* result,
                         const EnvOptions& options);

  Status NewDirectory(const std::string& name,
                      unique_ptr<Directory>* result);

  Status FileExists(const std::string& fname);

  Status GetChildren(const std::string& dir,
                     std::vector<std::string>* result);

  Status DeleteFile(const std::string& fname);

  Status CreateDir(const std::string& name);

  Status CreateDirIfMissing(const std::string& name);

  Status DeleteDir(const std::string& dirname);

  Status GetFileSize(const std::string& f, uint64_t* s);

  Status GetFileModificationTime(const std::string& fname,
                                 uint64_t* file_mtime);

  Status RenameFile(const std::string& s, const std::string& t);

  Status LockFile(const std::string& f, FileLock** l);

  Status UnlockFile(FileLock* l);

  void Schedule(void (*f)(void* arg), void* a, Priority pri,
                void* tag = nullptr, void (*u)(void* arg) = 0);

  void StartThread(void (*f)(void*), void* a);

  Status GetTestDirectory(std::string* path);

  Status NewLogger(const std::string& fname,
                   shared_ptr<Logger>* result);

  uint64_t NowMicros();

  void SleepForMicroseconds(int micros);

  Status GetHostName(char* name, uint64_t len);

  Status GetCurrentTime(int64_t* unix_time);

  Status GetAbsolutePath(const std::string& db_path,
                         std::string* output_path);

  void SetBackgroundThreads(int num, Priority pri);

  std::string TimeToString(uint64_t time);

  void IncBackgroundThreadsIfNeeded(int num, Priority pri);

};

}  // namespace rocksdb
