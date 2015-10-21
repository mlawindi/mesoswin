/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __STOUT_OS_WINDOWS_MKDTEMP_HPP__
#define __STOUT_OS_WINDOWS_MKDTEMP_HPP__

#include <time.h>

#include <stout/try.hpp>
#include <stout/nothing.hpp>
#include <stout/strings.hpp>

#include <stout/os/mkdir.hpp>


namespace os {

// Creates a temporary directory using the specified path
// template. The template may be any path with _6_ `Xs' appended to
// it, for example /tmp/temp.XXXXXX. The trailing `Xs' are replaced
// with a unique alphanumeric combination.
inline Try<std::string> mkdtemp(const std::string& path = "/tmp/XXXXXX")
{
  // NOTE: we'd like to avoid reallocating `postfixTemplate` and `alphabet`,
  // and to avoid  recomputing their sizes on each call to `mkdtemp`, so we
  // make them `static const` and use the slightly awkward `sizeof` trick to
  // compute their sizes once instead of calling `strlen` for each call.
  static const char postfixTemplate[] = "XXXXXX";
  static const size_t postfixSize = sizeof(postfixTemplate) - 1;

  if (!strings::endsWith(path, postfixTemplate)) {
    return Error(
      "Invalid template passed to `os::mkdtemp`: template '" + path +
      "' should end with 6 'X' characters");
  }

  static const char alphabet[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

  // NOTE: the maximum addressable index in a string is the total length of the
  // string minus 1; but C strings have an extra null character at the end, so
  // the size of the array is actually one more than the length of the string,
  // which is why we're subtracting 2 here.
  static const size_t maxAlphabetIndex = sizeof(alphabet) - 2;

  // NOTE: Generating postfix with good-enough, not-cryptographically-secure,
  // plain-old calls to `rand`.
  std::string postfix(postfixTemplate);
  srand(time(NULL));

  for (int i = 0; i < postfixSize; ++i) {
    int index = rand() % maxAlphabetIndex;
    postfix[i] = alphabet[index];
  }

  // Replace template, make directory.
  std::string tempPath = path
    .substr(0, path.length() - postfixSize)
    .append(postfix);

  Try<Nothing> tmpDirCreated = os::mkdir(tempPath);

  if (tmpDirCreated.isError()) {
    return Error(tmpDirCreated.error());
  }

  return tempPath;
}

} // namespace os {


#endif // __STOUT_OS_WINDOWS_MKDTEMP_HPP__
