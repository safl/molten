#include <iostream>
#include <string>
#include <chrono>
#include <linux/limits.h>
#include "env_nvm.h"

namespace rocksdb {

EnvNVM::EnvNVM(void) : Env(), fs_(NULL) {}

Status EnvNVM::NewSequentialFile(
  const std::string& fname,
  unique_ptr<SequentialFile>* result,
  const EnvOptions& options
) {
   NVM_DEBUG("fname(%s), result(?), options(?)\n", fname.c_str());

  return Status::IOError();
}

Status EnvNVM::NewRandomAccessFile(
  const std::string& fname,
  unique_ptr<RandomAccessFile>* result,
  const EnvOptions& options
) {
  NVM_DEBUG("fname(%s), result(?), options(?)\n", fname.c_str());

  return Status::IOError();
}

Status EnvNVM::NewWritableFile(
  const std::string& fname,
  unique_ptr<WritableFile>* result,
  const EnvOptions& options
) {
  NVM_DEBUG("fname(%s), result(?), options(?)\n", fname.c_str());

  return Status::IOError();
}

Status EnvNVM::ReuseWritableFile(
  const std::string& fname,
  const std::string& old_fname,
  unique_ptr<WritableFile>* result,
  const EnvOptions& options
) {
  NVM_DEBUG("fname(%s), old_fname(%s), result(?), options(?)\n",
            fname.c_str(), old_fname.c_str());

  return Status::IOError();
}

Status EnvNVM::NewDirectory(
  const std::string& name,
  unique_ptr<Directory>* result
) {
  NVM_DEBUG("name(%s), result(?)\n", name.c_str());

  return Status::IOError();
}

Status EnvNVM::FileExists(const std::string& fname) {
  NVM_DEBUG("fname(%s)\n", fname.c_str());

  return Status::NotFound();
}

Status EnvNVM::GetChildren(
  const std::string& dir,
  std::vector<std::string>* result
) {
  NVM_DEBUG("dir(%s), result(?)\n", dir.c_str());

  return Status::IOError();
}

Status EnvNVM::GetChildrenFileAttributes(
  const std::string& dir,
  std::vector<FileAttributes>* result
) {
  NVM_DEBUG("dir(%s), result(?)\n", dir.c_str());

  return Status::IOError();
}

Status EnvNVM::DeleteFile(const std::string& fname) {
  NVM_DEBUG("fname(%s)\n", fname.c_str());

  return Status::NotFound();
}

Status EnvNVM::CreateDir(const std::string& name) {
  NVM_DEBUG("name(%s)\n", name.c_str());

  return Status::IOError();
}

Status EnvNVM::CreateDirIfMissing(const std::string& name) {
  NVM_DEBUG("name(%s)\n", name.c_str());

  return Status::IOError();
}

Status EnvNVM::DeleteDir(const std::string& dirname) {
  NVM_DEBUG("dirname(%s)\n", dirname.c_str());

  return Status::IOError();
}

Status EnvNVM::GetFileSize(const std::string& f, uint64_t* s) {
  NVM_DEBUG("f(%s), s(%lu)\n", f.c_str(), *s);

  return Status::IOError();
}

Status EnvNVM::GetFileModificationTime(
  const std::string& fname,
  uint64_t* file_mtime
) {
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status EnvNVM::RenameFile(const std::string& s, const std::string& t) {
  NVM_DEBUG("s(%s), t(%s)\n", s.c_str(), t.c_str());

  return Status::NotFound();
}

Status EnvNVM::LockFile(const std::string& fname, FileLock** lock) {
  NVM_DEBUG(" f(%s), l(?)\n", fname.c_str());

  return Status::NotFound();
}

Status EnvNVM::UnlockFile(FileLock* lock) {
  delete lock;

  return Status::IOError();
}

void EnvNVM::Schedule(
  void (*function)(void* arg),
  void* arg,
  Priority pri,
  void* tag,
  void (*unschedFunction)(void* arg)
) {
  NVM_DEBUG("function(%p), arg(%p), pri(%d), tag(%p), unschedFunction(%p)\n",
            function, arg, pri, tag, unschedFunction);
}

int EnvNVM::UnSchedule(void* arg, Priority pri) {
  NVM_DEBUG("arg(%p), pri(%d)\n", arg, pri);

  return 0;
}

void EnvNVM::StartThread(
  void (*f)(void*),
  void* a
) {
  NVM_DEBUG("\n");

  return;
}

void EnvNVM::WaitForJoin(void) {
  NVM_DEBUG("\n");
}

unsigned int EnvNVM::GetThreadPoolQueueLen(Priority pri) const {
  NVM_DEBUG("pri(%d)\n", pri);

  return 0;
}

uint64_t EnvNVM::GetThreadID(void) const {
  return 0;
}

void EnvNVM::LowerThreadPoolIOPriority(Priority pool) {
  NVM_DEBUG("pool(%d)\n", pool);
}

uint64_t EnvNVM::NowNanos(void) {
  NVM_DEBUG("\n");

  return 0;
};

std::string EnvNVM::GenerateUniqueId(void) {
  return "foo";
};

Status EnvNVM::GetTestDirectory(std::string* path) {
  NVM_DEBUG("\n");

  return Status::NotFound();
}

Status EnvNVM::NewLogger(
  const std::string& fname,
  shared_ptr<Logger>* result
) {
  NVMLogger *logger = new NVMLogger(InfoLogLevel::INFO_LEVEL);

  NVM_DEBUG(" fname(%s), result(%p)\n", fname.c_str(), logger);

  result->reset(logger);

  return Status::OK();
}

uint64_t EnvNVM::NowMicros(void) {
  NVM_DEBUG("\n");

  auto now = std::chrono::high_resolution_clock::now();
  auto duration = now.time_since_epoch();
  auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration);

  return micros.count();
}

void EnvNVM::SleepForMicroseconds(int micros) {
  NVM_DEBUG("micros(%d)\n", micros);

  return;
}

Status EnvNVM::GetHostName(char* name, uint64_t len) {
  NVM_DEBUG("name(%s), len(%lu)\n", name, len);

  return Status::IOError();
}

Status EnvNVM::GetCurrentTime(int64_t* unix_time) {
  NVM_DEBUG("unix_time(%ld)\n", *unix_time);

  *unix_time = time(0);

  return Status::OK();
}

std::string EnvNVM::TimeToString(uint64_t stamp) {
  NVM_DEBUG("stamp(%lu)\n", stamp);

  const int BUF_LEN = 100;
  char buf [BUF_LEN];

  time_t raw = stamp;
  struct tm *inf = localtime(&raw);
  strftime(buf, BUF_LEN, "%Y-%m-%d %H:%M:%S", inf);

  return std::string(buf);
}

Status EnvNVM::GetAbsolutePath(const std::string& db_path,
                       std::string* output_path) {
  NVM_DEBUG("db_path(%s)\n", db_path.c_str());

  char buf[PATH_MAX];

  if (realpath(db_path.c_str(), buf)) {
    output_path->assign(buf);
    return Status::OK();
  }

  return Status::IOError("Failed retrieving absolute path");
}

void EnvNVM::SetBackgroundThreads(int num, Priority pri) {
  NVM_DEBUG(" num(%d), pri(%d)\n", num, pri);

  return;
}

void EnvNVM::IncBackgroundThreadsIfNeeded(int num, Priority pri) {
  NVM_DEBUG(" num(%d), pri(%d)\n", num, pri);

  return;
}

}       // namespace rocksdb
