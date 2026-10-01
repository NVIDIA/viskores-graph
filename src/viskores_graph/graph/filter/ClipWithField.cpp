// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/contour/ClipWithField.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

ClipWithFieldNode::ClipWithFieldNode()
{
  addParameter({this, "clipValue", ParameterType::BOUNDED_FLOAT, 0.f});
  addParameter({this, "invert", ParameterType::BOOL, false});
}

const char *ClipWithFieldNode::kind() const
{
  return "ClipWithField";
}

void ClipWithFieldNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet ClipWithFieldNode::execute()
{
  auto *inPort = datasetInput();
  auto ds = getDataSetFromPort(inPort);

  const auto fieldName = selectedFieldName(inPort, ds);
  if (fieldName.empty())
    return {};

  auto field = ds.GetField(fieldName);
  if (field.GetData().GetNumberOfComponentsFlat() != 1)
    return {};

  Range range;
  field.GetRange(&range);

  auto *p = parameter("clipValue");
  p->setMinMax<float>(range.Min, range.Max, range.Center());

  filter::contour::ClipWithField filter;
  filter.SetActiveField(fieldName);
  filter.SetClipValue(p->valueAs<float>());
  filter.SetInvertClip(parameter("invert")->valueAs<bool>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
