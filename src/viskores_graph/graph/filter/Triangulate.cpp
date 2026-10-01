// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/geometry_refinement/Triangulate.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

const char *TriangulateNode::kind() const
{
  return "Triangulate";
}

cont::DataSet TriangulateNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());
  return filter::geometry_refinement::Triangulate().Execute(ds);
}

} // namespace graph
} // namespace viskores
