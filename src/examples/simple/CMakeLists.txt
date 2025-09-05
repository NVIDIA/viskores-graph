## Copyright 2023-2025 NVIDIA Corporation
## SPDX-License-Identifier: Apache-2.0

project(example_graph)
project_add_executable(graph.cpp)
project_link_libraries(PRIVATE glm::glm viskores_graph stb_image)
