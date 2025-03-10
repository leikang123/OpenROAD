///////////////////////////////////////////////////////////////////////////
//
// BSD 3-Clause License
//
// Copyright (c) 2022, The Regents of the University of California
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
//
///////////////////////////////////////////////////////////////////////////////

#include "Hypergraph.h"

#include <iostream>
#include <string>

#include "Utilities.h"
#include "utl/Logger.h"

namespace par {

Hypergraph::Hypergraph(
    const int vertex_dimensions,
    const int hyperedge_dimensions,
    const int placement_dimensions,
    const std::vector<std::vector<int>>& hyperedges,
    const std::vector<std::vector<float>>& vertex_weights,
    const std::vector<std::vector<float>>& hyperedge_weights,
    // fixed vertices
    const std::vector<int>& fixed_attr,  // the block id of fixed vertices.
    // community attribute
    const std::vector<int>& community_attr,
    // placement information
    const std::vector<std::vector<float>>& placement_attr,
    utl::Logger* logger)
{
  vertex_dimensions_ = vertex_dimensions;
  hyperedge_dimensions_ = hyperedge_dimensions;
  num_vertices_ = static_cast<int>(vertex_weights.size());
  num_hyperedges_ = static_cast<int>(hyperedge_weights.size());

  vertex_weights_ = vertex_weights;
  hyperedge_weights_ = hyperedge_weights;

  // add hyperedge
  // hyperedges: each hyperedge is a set of vertices
  eind_.clear();
  eptr_.clear();
  eptr_.push_back(static_cast<int>(eind_.size()));
  for (const auto& hyperedge : hyperedges) {
    eind_.insert(eind_.end(), hyperedge.begin(), hyperedge.end());
    eptr_.push_back(static_cast<int>(eind_.size()));
  }

  // add vertex
  // create vertices from hyperedges
  std::vector<std::vector<int>> vertices(num_vertices_);
  for (int e = 0; e < num_hyperedges_; e++) {
    for (auto v : hyperedges[e]) {
      vertices[v].push_back(e);  // e is the hyperedge id
    }
  }
  vind_.clear();
  vptr_.clear();
  vptr_.push_back(static_cast<int>(vind_.size()));
  for (const auto& vertex : vertices) {
    vind_.insert(vind_.end(), vertex.begin(), vertex.end());
    vptr_.push_back(static_cast<int>(vind_.size()));
  }

  // fixed vertices
  fixed_vertex_flag_ = (static_cast<int>(fixed_attr.size()) == num_vertices_);
  if (fixed_vertex_flag_ == true) {
    fixed_attr_ = fixed_attr;
  }

  // community information
  community_flag_ = (static_cast<int>(community_attr.size()) == num_vertices_);
  if (community_flag_ == true) {
    community_attr_ = community_attr;
  }

  // placement information
  placement_flag_
      = (placement_dimensions > 0
         && static_cast<int>(placement_attr.size()) == num_vertices_);
  if (placement_flag_ == true) {
    placement_dimensions_ = placement_dimensions;
    placement_attr_ = placement_attr;
  } else {
    placement_dimensions_ = 0;
  }

  // logger
  logger_ = logger;
}

Hypergraph::Hypergraph(
    const int vertex_dimensions,
    const int hyperedge_dimensions,
    const int placement_dimensions,
    const std::vector<std::vector<int>>& hyperedges,
    const std::vector<std::vector<float>>& vertex_weights,
    const std::vector<std::vector<float>>& hyperedge_weights,
    // fixed vertices
    const std::vector<int>& fixed_attr,  // the block id of fixed vertices.
    // community attribute
    const std::vector<int>& community_attr,
    // placement information
    const std::vector<std::vector<float>>& placement_attr,
    // the type of each vertex
    const std::vector<VertexType>&
        vertex_types,  // except the original timing graph,
                       // users do not need to specify this
    // slack information
    const std::vector<float>& hyperedges_slack,
    const std::vector<std::set<int>>& hyperedges_arc_set,
    const std::vector<TimingPath>& timing_paths,
    utl::Logger* logger)
{
  vertex_dimensions_ = vertex_dimensions;
  hyperedge_dimensions_ = hyperedge_dimensions;
  num_vertices_ = static_cast<int>(vertex_weights.size());
  num_hyperedges_ = static_cast<int>(hyperedge_weights.size());

  vertex_weights_ = vertex_weights;
  hyperedge_weights_ = hyperedge_weights;

  // add hyperedge
  // hyperedges: each hyperedge is a set of vertices
  eind_.clear();
  eptr_.clear();
  eptr_.push_back(static_cast<int>(eind_.size()));
  for (const auto& hyperedge : hyperedges) {
    eind_.insert(eind_.end(), hyperedge.begin(), hyperedge.end());
    eptr_.push_back(static_cast<int>(eind_.size()));
  }

  // add vertex
  // create vertices from hyperedges
  std::vector<std::vector<int>> vertices(num_vertices_);
  for (int e = 0; e < num_hyperedges_; e++) {
    for (auto v : hyperedges[e]) {
      vertices[v].push_back(e);  // e is the hyperedge id
    }
  }
  vind_.clear();
  vptr_.clear();
  vptr_.push_back(static_cast<int>(vind_.size()));
  for (const auto& vertex : vertices) {
    vind_.insert(vind_.end(), vertex.begin(), vertex.end());
    vptr_.push_back(static_cast<int>(vind_.size()));
  }

  // fixed vertices
  fixed_vertex_flag_ = (static_cast<int>(fixed_attr.size()) == num_vertices_);
  if (fixed_vertex_flag_ == true) {
    fixed_attr_ = fixed_attr;
  }

  // community information
  community_flag_ = (static_cast<int>(community_attr.size()) == num_vertices_);
  if (community_flag_ == true) {
    community_attr_ = community_attr;
  }

  // placement information
  placement_flag_
      = (placement_dimensions > 0
         && static_cast<int>(placement_attr.size()) == num_vertices_);
  if (placement_flag_ == true) {
    placement_dimensions_ = placement_dimensions;
    placement_attr_ = placement_attr;
  } else {
    placement_dimensions_ = 0;
  }

  // add vertex types
  vertex_types_ = vertex_types;

  // slack information
  if (static_cast<int>(hyperedges_slack.size()) == num_hyperedges_
      && static_cast<int>(hyperedges_arc_set.size()) == num_hyperedges_) {
    timing_flag_ = true;
    num_timing_paths_ = static_cast<int>(timing_paths.size());
    hyperedge_timing_attr_ = hyperedges_slack;
    hyperedge_arc_set_ = hyperedges_arc_set;
    // create the vertex Matrix which stores the paths incident to vertex
    std::vector<std::vector<int>> incident_paths(num_vertices_);
    vptr_p_.push_back(static_cast<int>(vind_p_.size()));
    eptr_p_.push_back(static_cast<int>(eind_p_.size()));
    for (int path_id = 0; path_id < num_timing_paths_; path_id++) {
      // view each path as a sequence of vertices
      const auto& timing_path = timing_paths[path_id].path;
      vind_p_.insert(vind_p_.end(), timing_path.begin(), timing_path.end());
      vptr_p_.push_back(static_cast<int>(vind_p_.size()));
      for (const int v : timing_path) {
        incident_paths[v].push_back(path_id);
      }
      // view each path as a sequence of hyperedge
      const auto& timing_arc = timing_paths[path_id].arcs;
      eind_p_.insert(eind_p_.end(), timing_arc.begin(), timing_arc.end());
      eptr_p_.push_back(static_cast<int>(eind_p_.size()));
      // add the timing attribute
      path_timing_attr_.push_back(timing_paths[path_id].slack);
    }
    pptr_v_.push_back(static_cast<int>(pind_v_.size()));
    for (auto& paths : incident_paths) {
      pind_v_.insert(pind_v_.end(), paths.begin(), paths.end());
      pptr_v_.push_back(static_cast<int>(pind_v_.size()));
    }
  }
  // logger
  logger_ = logger;
}

std::vector<float> Hypergraph::GetTotalVertexWeights() const
{
  std::vector<float> total_weight(vertex_dimensions_, 0.0);
  for (auto& weight : vertex_weights_) {
    total_weight = total_weight + weight;
  }
  return total_weight;
}

// Get the vertex balance constraint
std::vector<std::vector<float>> Hypergraph::GetVertexBalance(
    int num_parts,
    float ub_factor) const
{
  std::vector<float> vertex_balance = GetTotalVertexWeights();
  vertex_balance = MultiplyFactor(
      vertex_balance, ub_factor * 0.01 + 1.0 / static_cast<float>(num_parts));
  return std::vector<std::vector<float>>(num_parts, vertex_balance);
}

// Get the vertex balance constraint (upper bound)
std::vector<std::vector<float>> Hypergraph::GetUpperVertexBalance(
    int num_parts,
    float ub_factor) const
{
  std::vector<float> vertex_balance = GetTotalVertexWeights();
  vertex_balance = MultiplyFactor(
      vertex_balance, ub_factor * 0.01 + 1.0 / static_cast<float>(num_parts));
  return std::vector<std::vector<float>>(num_parts, vertex_balance);
}

// Get the vertex balance constraint (lower bound)
std::vector<std::vector<float>> Hypergraph::GetLowerVertexBalance(
    int num_parts,
    float ub_factor) const
{
  std::vector<float> vertex_balance = GetTotalVertexWeights();
  ub_factor = std::max(
      -1.0 * ub_factor * 0.01 + 1.0 / static_cast<float>(num_parts), 0.0);
  vertex_balance = MultiplyFactor(vertex_balance, ub_factor);
  return std::vector<std::vector<float>>(num_parts, vertex_balance);
}

}  // namespace par
