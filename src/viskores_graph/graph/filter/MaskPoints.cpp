// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/entity_extraction/MaskPoints.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

MaskPointsNode::MaskPointsNode()
{
  addParameter({this, "stride", ParameterType::BOUNDED_INT, 2})
      ->setMinMax<int>(1, 64, 2);
  addParameter({this, "compactPoints", ParameterType::BOOL, true});
}

const char *MaskPointsNode::kind() const
{
  return "MaskPoints";
}

void MaskPointsNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet MaskPointsNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());

  filter::entity_extraction::MaskPoints filter;
  filter.SetStride(parameter("stride")->valueAs<int>());
  filter.SetCompactPoints(parameter("compactPoints")->valueAs<bool>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
