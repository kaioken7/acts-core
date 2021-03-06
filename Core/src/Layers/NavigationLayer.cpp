// This file is part of the Acts project.
//
// Copyright (C) 2016-2018 Acts project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// NavigationLayer.cpp, Acts project
///////////////////////////////////////////////////////////////////

#include "Acts/Layers/NavigationLayer.hpp"
#include "Acts/Surfaces/Surface.hpp"

Acts::NavigationLayer::NavigationLayer(
    std::shared_ptr<const Surface> surfaceRepresentation,
    double                         thickness)
  : Acts::Layer(nullptr)
  , m_surfaceRepresentation(std::move(surfaceRepresentation))
{
  m_layerThickness = thickness;
  m_layerType      = navigation;
}

Acts::NavigationLayer::~NavigationLayer() = default;
