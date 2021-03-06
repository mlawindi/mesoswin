# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# DEFINE DIRECTORY STRUCTURE FOR THIRD-PARTY LIBS.
##################################################
set(MESOS_3RDPARTY_SRC ${CMAKE_SOURCE_DIR}/3rdparty)
set(MESOS_3RDPARTY_BIN ${CMAKE_BINARY_DIR}/3rdparty)

EXTERNAL("zookeeper" ${ZOOKEEPER_VERSION} "${MESOS_3RDPARTY_BIN}")

# Intermediate convenience variables for oddly-structured directories.
set(ZOOKEEPER_C_ROOT ${ZOOKEEPER_ROOT}/src/c)
set(ZOOKEEPER_LIB    ${ZOOKEEPER_ROOT}-lib/lib)

# Convenience variables for include directories of third-party dependencies.
set(ZOOKEEPER_INCLUDE_GENDIR ${ZOOKEEPER_C_ROOT}/generated)
set(ZOOKEEPER_INCLUDE_DIR ${ZOOKEEPER_C_ROOT}/include)

# Convenience variables for `lib` directories of built third-party dependencies.
set(ZOOKEEPER_LIB_DIR ${ZOOKEEPER_LIB}/lib)

# Convenience variables for "lflags", the symbols we pass to CMake to generate
# things like `-L/path/to/glog` or `-lglog`.
set(ZOOKEEPER_LFLAG  zookeeper_mt)
