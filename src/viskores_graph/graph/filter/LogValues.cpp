// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/field_transform/LogValues.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

// Indexed by the "base" parameter
static const char *g_logBaseNames[] = {"ln", "log2", "log10"};

LogValuesNode::LogValuesNode()
{
  addParameter({this, "base", ParameterType::BOUNDED_INT, 0})
      ->setMinMax<int>(0, 2, 0);
}

const char *LogValuesNode::kind() const
{
  return "LogValues";
}

void LogValuesNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet LogValuesNode::execute()
{
  auto *inPort = datasetInput();
  auto ds = getDataSetFromPort(inPort);

  const auto fieldName = selectedFieldName(inPort, ds);
  if (fieldName.empty())
    return {};

  using LogBase = filter::field_transform::LogValues::LogBase;
  const LogBase bases[] = {LogBase::E, LogBase::TWO, LogBase::TEN};
  const int base = parameter("base")->valueAs<int>();

  filter::field_transform::LogValues filter;
  filter.SetActiveField(fieldName);
  filter.SetBaseValue(bases[base]);
  filter.SetOutputFieldName(
      std::string(g_logBaseNames[base]) + "(" + fieldName + ")");

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
