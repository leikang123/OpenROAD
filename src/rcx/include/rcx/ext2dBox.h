///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019-2023, Nefelus Inc, Google LLC
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <array>
#include <cstdio>

namespace rcx {

class ext2dBox  // assume cross-section on the z-direction
{
 public:
  // Implementation note: a default constructor is necessary because these are
  // AthPool allocated, and it requires the type to be default constructible.
  // We use placement new (with the full constructor below) for initialization
  // after pool allocation.
  ext2dBox() = default;

  ext2dBox(std::array<int, 2> ll,
           std::array<int, 2> ur,
           unsigned int met,
           unsigned int id,
           unsigned int map,
           bool dir);

  void rotate();
  void printGeoms3D(FILE* fp,
                    double h,
                    double t,
                    const std::array<int, 2>& orig) const;
  unsigned int length() const;
  unsigned int width() const;
  int loX() const;
  int loY() const;
  unsigned int id() const;

  unsigned int met() const { return _met; }
  unsigned int map() const { return _map; }
  bool dir() const { return _dir; }

  int ur0() const { return _ur[0]; }
  int ur1() const { return _ur[1]; }
  int ll0() const { return _ll[0]; }
  int ll1() const { return _ll[1]; }

 private:
  std::array<int, 2> _ll;
  std::array<int, 2> _ur;
  unsigned int _met;
  unsigned int _id;
  unsigned int _map;
  bool _dir;
};

}  // namespace rcx