// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/mesh_info/CellMeasures.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

CellMeasuresNode::CellMeasuresNode()
{
  addParameter({this, "arcLength", ParameterType::BOOL, true});
  addParameter({this, "area", ParameterType::BOOL, true});
  addParameter({this, "volume", ParameterType::BOOL, true});
}

const char *CellMeasuresNode::kind() const
{
  return "CellMeasures";
}

void CellMeasuresNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet CellMeasuresNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());

  using filter::mesh_info::IntegrationType;
  auto measure = IntegrationType::None;
  if (parameter("arcLength")->valueAs<bool>())
    measure = measure | IntegrationType::ArcLength;
  if (parameter("area")->valueAs<bool>())
    measure = measure | IntegrationType::Area;
  if (parameter("volume")->valueAs<bool>())
    measure = measure | IntegrationType::Volume;

  if (measure == IntegrationType::None)
    return ds;

  filter::mesh_info::CellMeasures filter;
  filter.SetMeasure(measure);
  filter.SetCellMeasureName("measure");

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
