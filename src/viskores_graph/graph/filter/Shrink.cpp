// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/geometry_refinement/Shrink.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

ShrinkNode::ShrinkNode()
{
  addParameter({this, "shrinkFactor", ParameterType::BOUNDED_FLOAT, 0.5f})
      ->setMinMax<float>(0.f, 1.f, 0.5f);
}

const char *ShrinkNode::kind() const
{
  return "Shrink";
}

void ShrinkNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet ShrinkNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());

  filter::geometry_refinement::Shrink filter;
  filter.SetShrinkFactor(parameter("shrinkFactor")->valueAs<float>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
