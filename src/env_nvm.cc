#include <iostream>
#include <string>
#include <chrono>
#include <linux/limits.h>
#include <iostream>
#include <fstream>
#include "env_nvm.h"

namespace rocksdb {

EnvNVM::EnvNVM(void) : Env(), fs_() {}

EnvNVM::~EnvNVM(void) {
  for (auto dit = fs_.begin(); dit != fs_.end(); ++dit) {
    NVM_DEBUG("dname(%s)\n", dit->first.c_str());
    for (auto fit = dit->second.begin(); fit != dit->second.end(); ++fit) {
      NVM_DEBUG("fname(%s)\n", (*fit)->GetName().c_str());
    }
  }
}

std::pair<std::string, std::string> EnvNVM::SplitPath(const std::string& path) {

  char sep = '/';
  int sep_idx = path.find_last_of(sep, path.length());

  return std::pair<std::string, std::string>(
      path.substr(0, sep_idx),
      path.substr(sep_idx+1)
  );
}

Status EnvNVM::NewDirectory(
  const std::string& name,
  unique_ptr<Directory>* result
) {
  NVM_DEBUG("name(%s), result(?)\n", name.c_str());

  result->reset(new NVMDirectory());

  return Status::OK();
}

Status EnvNVM::NewSequentialFile(
  const std::string& fpath,
  unique_ptr<SequentialFile>* result,
  const EnvOptions& options
) {
  NVM_DEBUG("fpath(%s), result(?), options(?)\n", fpath.c_str());

  NVMFile *file = FindFile(fpath);
  if (!file) {
    return Status::NotFound();
  }

  result->reset(new NVMSequentialFile(file, options));

  return Status::OK();
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

  std::pair<std::string, std::string> fparts = SplitPath(fname);

  NVM_DEBUG("dname(%s), fname(%s)\n",
            fparts.first.c_str(), fparts.second.c_str());

  NVMFile *file = FindFile(fname);
  if (file) {
    DeleteFile(fname);
  }

  file = new NVMFile(this, fparts.first, fparts.second);
  AddFile(file);

  result->reset(new NVMWritableFile(file, options));

  return Status::OK();
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

Status EnvNVM::FileExists(const std::string& fname) {
  NVM_DEBUG("fname(%s)\n", fname.c_str());

  if (FindFile(fname)) {
    return Status::OK();
  }

  return Status::NotFound();
}

Status EnvNVM::GetChildren(
  const std::string& dname,
  std::vector<std::string>* result
) {
  NVM_DEBUG("dir(%s), result(?)\n", dname.c_str());

  result->clear();

  auto dir = fs_.find(dname);

  if (dir == fs_.end())
    return Status::IOError();

  for (auto it = dir->second.begin(); it != dir->second.end(); ++it) {
    result->push_back((*it)->GetName());
  }

  return Status::OK();
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

Status EnvNVM::CreateDirIfMissing(const std::string& name) {
  NVM_DEBUG("name(%s)\n", name.c_str());

  auto ret = fs_.insert(std::pair<std::string, std::vector<NVMFile*> >(
    name, std::vector<NVMFile*>()
  ));

  NVM_DEBUG("ret.second(%d)\n", ret.second);

  return Status::OK();
}

Status EnvNVM::RemoveFile(const std::string& dname, const std::string& fname) {
  NVM_DEBUG("dname(%s), fname(%s)\n", dname.c_str(), fname.c_str());

  auto dit = fs_.find(dname);
  if (dit == fs_.end()) {
    NVM_DEBUG("Dir NOT found\n");
    return Status::NotFound();
  }

  for (auto it = dit->second.begin(); it != dit->second.end(); ++it) {
    if ((*it)->IsNamed(fname)) {
      NVM_DEBUG("File found -- erasing\n");
      dit->second.erase(it);
      return Status::OK();
    }
  }

  NVM_DEBUG("File NOT found\n");
  return Status::NotFound();
}

Status EnvNVM::RemoveFile(const std::string& path) {
  NVM_DEBUG("path(%s)\n", path.c_str());

  std::pair<std::string, std::string> parts = SplitPath(path);

  return RemoveFile(parts.first, parts.second);
}

Status EnvNVM::AddFile(NVMFile *file) {
  NVM_DEBUG("file(%p), dname(%s), fname(%s)\n",
            file, file->GetDir().c_str(), file->GetName().c_str());

  if (FindFile(file->GetDir(), file->GetName())) {
    NVM_DEBUG("File exists.\n");
    return Status::IOError();
  }

  fs_[file->GetDir()].push_back(file);

  NVM_DEBUG("File added.\n");
  return Status::OK();
}

NVMFile* EnvNVM::FindFile(const std::string& dname, const std::string& fname) {
  NVM_DEBUG("dname(%s), fname(%s)\n", dname.c_str(), fname.c_str());

  auto dit = fs_.find(dname);
  if (dit == fs_.end()) {
    NVM_DEBUG("!found\n");
    return NULL;
  }

  for (auto it = dit->second.begin(); it != dit->second.end(); ++it) {
    if ((*it)->IsNamed(fname)) {
      NVM_DEBUG("found\n");
      return *it;
    }
  }

  NVM_DEBUG("!found\n");
  return NULL;
}

NVMFile* EnvNVM::FindFile(const std::string& path) {
  std::pair<std::string, std::string> parts = SplitPath(path);

  return FindFile(parts.first, parts.second);
}

Status EnvNVM::CreateDir(const std::string& name) {
  NVM_DEBUG("name(%s)\n", name.c_str());

  auto it = fs_.find(name);
  if (it != fs_.end()) {
    return Status::IOError();
  }

  return CreateDirIfMissing(name);
}

Status EnvNVM::DeleteDir(const std::string& dirname) {
  NVM_DEBUG("dirname(%s)\n", dirname.c_str());

  return Status::IOError();
}

Status EnvNVM::GetFileSize(const std::string& fpath, uint64_t* fsize) {
  NVM_DEBUG("fpath(%s)\n", fpath.c_str());

  NVMFile *file = FindFile(fpath);
  if (!file)
    return Status::IOError("File not not found");

  *fsize = file->GetFileSize();

  return Status::OK();
}

Status EnvNVM::GetFileModificationTime(
  const std::string& fname,
  uint64_t* file_mtime
) {
  NVM_DEBUG("\n");

  return Status::IOError();
}

Status EnvNVM::RenameFile(
  const std::string& fpath_s,
  const std::string& fpath_t
) {
  NVM_DEBUG("fpath_s(%s), fpath_t(%s)\n", fpath_s.c_str(), fpath_t.c_str());

  NVMFile *file = FindFile(fpath_s);            // Find file to rename
  if (!file) {
    NVM_DEBUG("Cannot find file to rename\n");
    return Status::NotFound();
  }

  NVMFile *file_target = FindFile(fpath_t);     // Delete possibly existing file
  if (file_target) {
    NVM_DEBUG("Target exists.. deleting it...\n");
    DeleteFile(fpath_t);
  }

  auto parts = SplitPath(fpath_t);

  if (file->GetDir().compare(parts.first)) {
    NVM_DEBUG("Changing directory is not supported\n");
    return Status::IOError();
  }
  file->Rename(parts.second);                   // Do the actual renaming

  return Status::OK();
}

Status EnvNVM::LockFile(const std::string& fname, FileLock** lock) {
  NVM_DEBUG("fname(%s), lock(%p)\n", fname.c_str(), *lock);

  *lock = new FileLock;

  NVM_DEBUG("created lock(%p)\n", *lock);

  return Status::OK();
}

Status EnvNVM::UnlockFile(FileLock* lock) {
  NVM_DEBUG("lock(%p)\n", lock);

  delete lock;

  return Status::OK();
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
  NVM_DEBUG("f(%p), a(%p)\n", f, a);

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

  std::ifstream uuid_file("/proc/sys/kernel/random/uuid");
  std::string line;

  if (uuid_file.is_open()) {
    getline(uuid_file, line);
    uuid_file.close();
    NVM_DEBUG("generated(%s)\n", line.c_str());
    return line;
  }

  return "foo-bar-not-unique";
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

Status EnvNVM::GetAbsolutePath(
  const std::string& db_path,
  std::string* output_path
) {
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
