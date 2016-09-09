#include <iostream>
#include <string>
#include <chrono>
#include <linux/limits.h>
#include "env_nvm.h"

namespace rocksdb {

EnvNVM::EnvNVM(void) : Env(), files_(), dirs_() {}

Status EnvNVM::NewRandomAccessFile(const std::string& fname,
                                   unique_ptr<RandomAccessFile>* result,
                                   const EnvOptions& options) {
  NVM_DEBUG("\n");

  unique_ptr<NVMRandomAccessFile> file(new NVMRandomAccessFile(fname, options));

  *result = std::move(file);

  return Status::OK();
}

Status EnvNVM::NewWritableFile(const std::string& fname,
                       unique_ptr<WritableFile>* result,
                       const EnvOptions& options) {
  NVM_DEBUG("fname(%s), result(?), options(?)\n", fname.c_str());

  unique_ptr<NVMWritableFile> file(new NVMWritableFile(fname, options));

  *result = std::move(file);

  return Status::OK();
}

Status EnvNVM::NewDirectory(const std::string& name,
                    unique_ptr<Directory>* result) {
  NVM_DEBUG(" name(%s), result(?)\n", name.c_str());

  NVMDirectory *dir = new NVMDirectory(name);
  dirs_.push_back(*dir);

  result->reset(dir);

  return Status::OK();
}

Status EnvNVM::FileExists(const std::string& fname) {
  NVM_DEBUG(" fname(%s)\n", fname.c_str());

  for (auto it = files_.begin(); it != files_.end(); ++it) {
    if (0 == it->compare(fname)) {
      NVM_DEBUG("OK");
      return Status::OK();
    }
  }

  NVM_DEBUG("NotFound");
  return Status::NotFound();
}

Status EnvNVM::GetChildren(const std::string& dir,
                   std::vector<std::string>* result) {
  NVM_DEBUG(" dir(%s), result(?)\n", dir.c_str());

  for (auto it = files_.begin(); it != files_.end(); ++it) {
    if (dir.size() >= it->size())
      continue;
    if (0 == strncmp(it->c_str(), dir.c_str(), dir.size())) {
      result->push_back(*it);
    }
  }

  for (auto it = result->begin(); it != result->end(); ++it) {
    std::cout << *it << std::endl;
  }

  return Status::OK();
}

Status EnvNVM::DeleteFile(const std::string& fname) {
  NVM_DEBUG("fname(%s)\n", fname.c_str());

  for (auto it = files_.begin(); it != files_.end(); ++it) {
    if (0 == it->compare(fname)) {
      NVM_DEBUG("OK");
      return Status::OK();
    }
  }

  return Status::OK();
}

Status EnvNVM::CreateDir(const std::string& name) {
  NVM_DEBUG("name(%s)\n", name.c_str());

  for(auto it = dirs_.begin(); it != dirs_.end(); ++it) {
    if (!name.compare(it->getName())) {

      NVM_DEBUG("Skipping creation it exists..");
      return Status::IOError();
    }
  }

  NVM_DEBUG("Creating dir.\n");
  dirs_.push_back(NVMDirectory(name));

  return Status::OK();
}

Status EnvNVM::CreateDirIfMissing(const std::string& name) {
  NVM_DEBUG("name(%s)\n", name.c_str());

  for (auto it = dirs_.begin(); it != dirs_.end(); it++) {
    if (!name.compare(it->getName())) {
      NVM_DEBUG("Skipping creation it exists..");
      return Status::OK();
    }
  }

  NVM_DEBUG("Creating dir.\n");

  dirs_.push_back(NVMDirectory(name));

  return Status::OK();
}

Status EnvNVM::DeleteDir(const std::string& dirname) {
  NVM_DEBUG("\n");

  return Status::OK();
}

Status EnvNVM::GetFileSize(const std::string& f, uint64_t* s) {
  NVM_DEBUG("\n");

  return Status::OK();
}

Status EnvNVM::GetFileModificationTime(const std::string& fname,
                               uint64_t* file_mtime) {
  NVM_DEBUG("\n");

  return Status::OK();
}

Status EnvNVM::RenameFile(const std::string& s, const std::string& t) {
  NVM_DEBUG("s(%s), t(%s)\n", s.c_str(), t.c_str());

  for (auto it = files_.begin(); it != files_.end(); ++it) {
    if (!it->compare(s)) {
      files_.erase(it);
      files_.push_back(t);
      NVM_DEBUG("OK\n");
      return Status::OK();
    }
  }

  NVM_DEBUG("NotFound\n");
  return Status::NotFound();
}

Status EnvNVM::LockFile(const std::string& fname, FileLock** lock) {
  NVM_DEBUG(" f(%s), l(?)\n", fname.c_str());

  *lock = new FileLock;
  NVM_DEBUG("created lock(%p)\n", *lock);

  return Status::OK();
}

Status EnvNVM::UnlockFile(FileLock* lock) {
  delete lock;
  return Status::OK();
}

void EnvNVM::Schedule(void (*f)(void* arg), void* a, Priority pri,
              void* tag, void (*u)(void* arg)) {
  NVM_DEBUG("\n");

  return;
}

void EnvNVM::StartThread(void (*f)(void*), void* a) {
  NVM_DEBUG("\n");

  return;
}

Status EnvNVM::GetTestDirectory(std::string* path) {

  NVM_DEBUG("\n");

  *path = "/tmp/test";
  return Status::OK();
}

Status EnvNVM::NewLogger(const std::string& fname,
                 shared_ptr<Logger>* result) {

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
  NVM_DEBUG("\n");

  return;
}

Status EnvNVM::GetHostName(char* name, uint64_t len) {
  NVM_DEBUG("\n");

  return Status::OK();
}

Status EnvNVM::GetCurrentTime(int64_t* unix_time) {
  NVM_DEBUG("\n");

  *unix_time = time(0);

  return Status::OK();
}

std::string EnvNVM::TimeToString(uint64_t stamp) {
  NVM_DEBUG("\n");

  const int BUF_LEN = 100;
  char buf [BUF_LEN];

  time_t raw = stamp;
  struct tm *inf = localtime(&raw);
  strftime(buf, BUF_LEN, "%Y-%m-%d %H:%M:%S", inf);

  return std::string(buf);
}

Status EnvNVM::GetAbsolutePath(const std::string& db_path,
                       std::string* output_path) {
  NVM_DEBUG(" db_path(%s)\n", db_path.c_str());

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

