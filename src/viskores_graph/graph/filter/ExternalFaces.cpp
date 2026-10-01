// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/entity_extraction/ExternalFaces.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

ExternalFacesNode::ExternalFacesNode()
{
  addParameter({this, "compactPoints", ParameterType::BOOL, false});
  addParameter({this, "passPolyData", ParameterType::BOOL, true});
}

const char *ExternalFacesNode::kind() const
{
  return "ExternalFaces";
}

void ExternalFacesNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet ExternalFacesNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());

  filter::entity_extraction::ExternalFaces filter;
  filter.SetCompactPoints(parameter("compactPoints")->valueAs<bool>());
  filter.SetPassPolyData(parameter("passPolyData")->valueAs<bool>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
