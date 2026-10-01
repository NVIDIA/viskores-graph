// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/field_transform/Warp.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

WarpNode::WarpNode()
{
  addParameter({this, "scaleFactor", ParameterType::FLOAT, 1.f});
  addParameter({this, "useScaleField", ParameterType::BOOL, true});
  addParameter({this, "directionX", ParameterType::FLOAT, 0.f});
  addParameter({this, "directionY", ParameterType::FLOAT, 0.f});
  addParameter({this, "directionZ", ParameterType::FLOAT, 1.f});
}

const char *WarpNode::kind() const
{
  return "Warp";
}

void WarpNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet WarpNode::execute()
{
  auto *inPort = datasetInput();
  auto ds = getDataSetFromPort(inPort);

  filter::field_transform::Warp filter;
  filter.SetConstantDirection(Vec3f(parameter("directionX")->valueAs<float>(),
      parameter("directionY")->valueAs<float>(),
      parameter("directionZ")->valueAs<float>()));
  filter.SetScaleFactor(parameter("scaleFactor")->valueAs<float>());

  // The selected field scales the displacement; it must be a point scalar
  const auto fieldName = selectedFieldName(inPort, ds);
  if (parameter("useScaleField")->valueAs<bool>() && !fieldName.empty()) {
    auto field = ds.GetField(fieldName);
    if (field.IsPointField()
        && field.GetData().GetNumberOfComponentsFlat() == 1)
      filter.SetScaleField(fieldName);
  }

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
