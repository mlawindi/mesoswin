// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __STOUT_OS_HPP__
#define __STOUT_OS_HPP__

#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <glog/logging.h>
#include <sys/types.h>

#include <list>
#include <queue>
#include <set>
#include <string>

#include <stout/bytes.hpp>
#include <stout/duration.hpp>
#include <stout/error.hpp>
#include <stout/foreach.hpp>
#include <stout/none.hpp>
#include <stout/nothing.hpp>
#include <stout/option.hpp>
#include <stout/path.hpp>
#include <stout/result.hpp>
#include <stout/strings.hpp>
#include <stout/try.hpp>
#include <stout/unreachable.hpp>
#include <stout/version.hpp>
#include <stout/exit.hpp>

#include <stout/os/bootid.hpp>
#include <stout/os/chdir.hpp>
#include <stout/os/chroot.hpp>
#include <stout/os/exists.hpp>
#include <stout/os/fcntl.hpp>
#include <stout/os/ls.hpp>
#include <stout/os/mkdir.hpp>
#include <stout/os/mkdtemp.hpp>
#include <stout/os/mktemp.hpp>
#include <stout/os/permissions.hpp>
#include <stout/os/process.hpp>
#include <stout/os/rm.hpp>
#include <stout/os/rmdir.hpp>
#include <stout/os/shell.hpp>
#include <stout/os/stat.hpp>
#include <stout/os/su.hpp>
#include <stout/os/touch.hpp>
#include <stout/os/utime.hpp>

#include <stout/os/config.hpp>

// For readability, we minimize the number of #ifdef blocks in the code by
// splitting platform specifc system calls into separate directories.
#ifdef __WINDOWS__
#include <stout/windows/os.hpp>
#else
#include <stout/posix/os.hpp>
#endif // __WINDOWS__

namespace os
{

inline Option<Process> process(
  pid_t pid,
  const std::list<Process>& processes)
{
  foreach(const Process& process, processes) {
    if (process.pid == pid) {
      return process;
    }
  }
  return None();
}

inline std::set<pid_t> children(
  pid_t pid,
  const std::list<Process>& processes,
  bool recursive = true)
{
  // Perform a breadth first search for descendants.
  std::set<pid_t> descendants;
  std::queue<pid_t> parents;
  parents.push(pid);

  do {
    pid_t parent = parents.front();
    parents.pop();

    // Search for children of parent.
    foreach(const Process& process, processes) {
      if (process.parent == parent) {
        // Have we seen this child yet?
        if (descendants.insert(process.pid).second) {
          parents.push(process.pid);
        }
      }
    }
  } while (recursive && !parents.empty());

  return descendants;
}

inline Try<std::set<pid_t> > children(pid_t pid, bool recursive = true)
{
  const Try<std::list<Process>> processes = os::processes();

  if (processes.isError()) {
    return Error(processes.error());
  }

  return children(pid, processes.get(), recursive);
}

} // namespace os {

#endif // __STOUT_OS_HPP__
