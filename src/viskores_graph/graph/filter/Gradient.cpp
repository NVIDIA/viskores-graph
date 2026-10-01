// Copyright 2023-2025 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../FilterNode.h"
// viskores
#include <viskores/filter/vector_analysis/Gradient.h>

namespace viskores {
namespace graph {

GradientNode::GradientNode()
{
  addParameter({this, "computeGradient", ParameterType::BOOL, true});
  addParameter({this, "pointGradient", ParameterType::BOOL, false});
  addParameter({this, "divergence", ParameterType::BOOL, false});
  addParameter({this, "vorticity", ParameterType::BOOL, false});
  addParameter({this, "qCriterion", ParameterType::BOOL, false});
}

const char *GradientNode::kind() const
{
  return "Gradient";
}

void GradientNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet GradientNode::execute()
{
  auto *inPort = datasetInput();
  auto ds = getDataSetFromPort(inPort);

  filter::vector_analysis::Gradient filter;
  filter.SetFieldsToPass(filter::FieldSelection::Mode::None);
  const auto fieldName = selectedFieldName(inPort, ds);
  if (fieldName.empty())
    filter.SetUseCoordinateSystemAsField(true);
  else
    filter.SetActiveField(fieldName);
  filter.SetOutputFieldName("Gradient");
  filter.SetComputeGradient(parameter("computeGradient")->valueAs<bool>());
  filter.SetComputePointGradient(parameter("pointGradient")->valueAs<bool>());
  filter.SetComputeDivergence(parameter("divergence")->valueAs<bool>());
  filter.SetComputeVorticity(parameter("vorticity")->valueAs<bool>());
  filter.SetComputeQCriterion(parameter("qCriterion")->valueAs<bool>());

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
