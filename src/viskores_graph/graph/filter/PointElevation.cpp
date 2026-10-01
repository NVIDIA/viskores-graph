// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/field_transform/PointElevation.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

PointElevationNode::PointElevationNode()
{
  addParameter({this, "axis", ParameterType::BOUNDED_INT, 2})
      ->setMinMax<int>(0, 2, 2);
  addParameter({this, "rangeLow", ParameterType::FLOAT, 0.f});
  addParameter({this, "rangeHigh", ParameterType::FLOAT, 1.f});
}

const char *PointElevationNode::kind() const
{
  return "PointElevation";
}

void PointElevationNode::parameterChanged(
    Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet PointElevationNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());

  // Elevation runs between the min and max of the bounds along 'axis'
  const int axis = parameter("axis")->valueAs<int>();
  const auto bounds = ds.GetCoordinateSystem().GetBounds();
  const Range axisRanges[] = {bounds.X, bounds.Y, bounds.Z};
  auto low = bounds.Center();
  auto high = low;
  low[axis] = axisRanges[axis].Min;
  high[axis] = axisRanges[axis].Max;

  filter::field_transform::PointElevation filter;
  filter.SetUseCoordinateSystemAsField(true);
  filter.SetLowPoint(low);
  filter.SetHighPoint(high);
  filter.SetRange(parameter("rangeLow")->valueAs<float>(),
      parameter("rangeHigh")->valueAs<float>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
