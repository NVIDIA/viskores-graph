// Copyright 2026 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0

#include <viskores/filter/mesh_info/MeshQuality.h>
#include "../FilterNode.h"

namespace viskores {
namespace graph {

// Indexed by the "metric" parameter, matching filter::mesh_info::CellMetric
static const char *g_metricNames[] = {"Area",
    "AspectGamma",
    "AspectRatio",
    "Condition",
    "DiagonalRatio",
    "Dimension",
    "Jacobian",
    "MaxAngle",
    "MaxDiagonal",
    "MinAngle",
    "MinDiagonal",
    "Oddy",
    "RelativeSizeSquared",
    "ScaledJacobian",
    "Shape",
    "ShapeAndSize",
    "Shear",
    "Skew",
    "Stretch",
    "Taper",
    "Volume",
    "Warpage"};

static constexpr int g_numMetrics =
    sizeof(g_metricNames) / sizeof(g_metricNames[0]);

MeshQualityNode::MeshQualityNode()
{
  addParameter({this, "metric", ParameterType::BOUNDED_INT, 0})
      ->setMinMax<int>(0, g_numMetrics - 1, 0);
}

const char *MeshQualityNode::kind() const
{
  return "MeshQuality";
}

void MeshQualityNode::parameterChanged(Parameter *p, ParameterChangeType type)
{
  if (type == ParameterChangeType::NEW_VALUE)
    markChanged();
}

cont::DataSet MeshQualityNode::execute()
{
  auto ds = getDataSetFromPort(datasetInput());

  const int metric = parameter("metric")->valueAs<int>();

  filter::mesh_info::MeshQuality filter;
  filter.SetMetric(static_cast<filter::mesh_info::CellMetric>(metric));
  filter.SetOutputFieldName(g_metricNames[metric]);

  return filter.Execute(ds);
}

} // namespace graph
} // namespace viskores
