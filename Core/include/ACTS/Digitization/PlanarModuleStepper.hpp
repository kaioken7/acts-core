// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ACTS_DIGITIZATION_PLANARMODULESTEPPER_H
#define ACTS_DIGITIZATION_PLANARMODULESTEPPER_H

#include <memory>
#include <vector>
#include "ACTS/Digitization/DigitizationCell.hpp"
#include "ACTS/Utilities/Definitions.hpp"
#include "ACTS/Utilities/Logger.hpp"

namespace Acts {

class DigitizationModule;

/// @class PlanarModuleStepper
///
/// Module for fast, geometric digitization
/// this is a planar module stepper that calculates the step length
/// in given segmentations and retrunes digitisation steps

class PlanarModuleStepper
{
public:
  ///  @struct Config
  ///  Configuration for the planar module stepper
  struct Config
  {
    // standard constructor
    Config() {}
  };

  /// Constructor
  ///
  /// @param pmsConfig is the configuration 
  /// @param logger is the logging istance
  PlanarModuleStepper(const Config&                 pmsConfig,
                      std::unique_ptr<const Logger> logger
                      = getDefaultLogger("PlanarModuleStepper", Logging::INFO));

  /// Destructor
  ~PlanarModuleStepper() = default;

  /// Calculate the steps caused by this track - full simulation interface
  /// 
  /// @param dmodule is the digitization module
  /// @param startPosition is the starting position of the stepping
  /// @param endPosition is the end postion of the stepping
  ///
  /// @return is the vector of digitization steps
  std::vector<DigitizationStep>
  cellSteps(const DigitizationModule& dmodule,
            const Vector3D&           startPosition,
            const Vector3D&           endPosition) const;

  /// Calculate the steps caused by this track - fast simulation interface 
  /// 
  /// @param dmodule is the digitization module
  /// @param intersection is the 2d intersection at the module surface
  /// @param direction is the track direction at the instersection
  ///
  /// @return is the vector of digitization steps
  std::vector<DigitizationStep>
  cellSteps(const DigitizationModule& dmodule,
            const Vector2D&           intersection,
            const Vector3D&           direction) const;

  /// Set logging instance
  ///
  /// @param logger is the logging instance to be set
  void
  setLogger(std::unique_ptr<const Logger> logger)
  {
    m_logger = std::move(logger);
  }

private:
  /// Private access method to the logging instance
  const Logger&
  logger() const
  {
    return *m_logger;
  }

  /// the config class
  Config m_cfg;

  /// logging instance
  std::unique_ptr<const Logger> m_logger;
};

}  // end of namespace

#endif  // ACTS_DIGITIZATION_PLANARMODULESTEPPER_H
