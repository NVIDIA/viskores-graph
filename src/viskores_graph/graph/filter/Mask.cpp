// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/entity_extraction/Mask.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

MaskNode::MaskNode()
{
  addParameter({this, "stride", ParameterType::BOUNDED_INT, 2})
      ->setMinMax<int>(1, 64, 2);
  addParameter({this, "compactPoints", ParameterType::BOOL, false});
}

const char *MaskNode::kind() const
{
  return "Mask";
}

void MaskNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet MaskNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());

  filter::entity_extraction::Mask filter;
  Id stride = parameter("stride")->valueAs<int>();
  filter.SetStride(stride);
  filter.SetCompactPoints(parameter("compactPoints")->valueAs<bool>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
