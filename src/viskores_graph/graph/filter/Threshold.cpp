// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/entity_extraction/Threshold.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

ThresholdNode::ThresholdNode()
{
  addParameter({this, "lower", ParameterType::BOUNDED_FLOAT, 0.f});
  addParameter({this, "upper", ParameterType::BOUNDED_FLOAT, 0.f});
  addParameter({this, "allInRange", ParameterType::BOOL, false});
  addParameter({this, "invert", ParameterType::BOOL, false});
}

const char *ThresholdNode::kind() const
{
  return "Threshold";
}

void ThresholdNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet ThresholdNode::execute()
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

  auto *lower = parameter("lower");
  auto *upper = parameter("upper");
  lower->setMinMax<float>(range.Min, range.Max, range.Min);
  upper->setMinMax<float>(range.Min, range.Max, range.Max);

  filter::entity_extraction::Threshold filter;
  filter.SetActiveField(fieldName);
  filter.SetThresholdBetween(lower->valueAs<float>(), upper->valueAs<float>());
  filter.SetAllInRange(parameter("allInRange")->valueAs<bool>());
  filter.SetInvert(parameter("invert")->valueAs<bool>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
