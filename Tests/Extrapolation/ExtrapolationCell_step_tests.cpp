// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///  Boost include(s)
#define BOOST_TEST_MODULE ExtrapolationCell Tests
#include "ACTS/Extrapolation/ExtrapolationCell.hpp"
#include <boost/test/included/unit_test.hpp>
#include "ACTS/EventData/NeutralParameters.hpp"
#include "ACTS/Material/Material.hpp"
#include "ACTS/Material/MaterialProperties.hpp"
#include "ACTS/Utilities/Units.hpp"

namespace Acts {

namespace Test {

  /// This tests emulates a propagation
  /// start - post material update
  /// passive surface - full material update
  /// desintaion - pre material update
  BOOST_AUTO_TEST_CASE(
      ExtrapolationCell_start_passive_destination_material_test)
  {

    // start vertex and direction
    Vector3D vertex(0., 0., 0);
    Vector3D direction(1., 0., 0.);
    // some plane somewhere
    Vector3D plane(5., 0., 0.);
    // final destination
    Vector3D destination(10., 0., 0.);

    // (A) start
    // (1) create start parameters - neutral/charged doesn't matter
    NeutralCurvilinearParameters sParameters(nullptr, vertex, direction);

    // (B) propagation step
    // create the ExtrapolationCell from the start parameters
    ExtrapolationCell<NeutralParameters> ecc(sParameters);
    // now emulate a propagator
    auto pParameters = std::make_unique<const NeutralCurvilinearParameters>(
        nullptr, plane, direction);
    double pathLength = 5.;
    // cache the pointer result
    const NeutralCurvilinearParameters* pParametersPtr = pParameters.get();
    // emulate the step
    ecc.stepTransport(
        std::move(pParameters), {ExtrapolationMode::Propagation}, pathLength);

    // let's do a bunch of checks
    // there should be one step
    BOOST_CHECK_EQUAL(1ul, ecc.extrapolationSteps.size());
    // the leadParameters should be the pParameters
    BOOST_CHECK_EQUAL(pParametersPtr, ecc.leadParameters);
    // the lastLeadParameters should be the sParameters
    BOOST_CHECK_EQUAL(&sParameters, ecc.lastLeadParameters);
    /// the step length should be the pathLength
    BOOST_CHECK_EQUAL(pathLength, ecc.pathLength);

    // (C) destination
    // emulate propagation to final surface
    auto dParameters = std::make_unique<const NeutralCurvilinearParameters>(
        nullptr, destination, direction);
    // cache the pointer result
    const NeutralCurvilinearParameters* dParametersPtr = dParameters.get();
    // emulate the step
    ecc.stepTransport(
        std::move(dParameters), {ExtrapolationMode::Destination}, pathLength);

    // let's do a bunch of checks
    // there should be one step
    BOOST_CHECK_EQUAL(2ul, ecc.extrapolationSteps.size());
    // the leadParameters should be the pParameters
    BOOST_CHECK_EQUAL(dParametersPtr, ecc.leadParameters);
    // the lastLeadParameters should be the sParameters
    BOOST_CHECK_EQUAL(pParametersPtr, ecc.lastLeadParameters);
    /// the step length should be the pathLength
    BOOST_CHECK_EQUAL(2 * pathLength, ecc.pathLength);

    // this was the last step
    // the step parameters should be a nullptr
    // BOOST_CHECK_EQUAL(nullptr,nullptr);
    const NeutralCurvilinearParameters* nPtr = nullptr;
    BOOST_CHECK_EQUAL(nPtr, ecc.extrapolationSteps.back().parameters.get());
    // the end parameters should be the the dParametersPtr
    BOOST_CHECK_EQUAL(ecc.endParameters.get(), dParametersPtr);
  }

  // This tests emulates a propagation
  /// start - passive navigation surface - destination
  /// without material interaction
  BOOST_AUTO_TEST_CASE(ExtrapolationCell_start_navigation_destination_test)
  {

    // start vertex and direction
    Vector3D vertex(0., 0., 0);
    Vector3D direction(1., 0., 0.);
    // some plane somewhere
    Vector3D plane(5., 0., 0.);
    // final destination
    Vector3D destination(10., 0., 0.);

    // all surfaces have 0.25 um of material
    // arranged as post, full, pre
    double thickness = 250 * units::_um;
    double X0        = 9.37 * units::_cm;
    double L0        = 46.5 * units::_cm;
    double A         = 28;
    double Z         = 14;
    double rho = 0.002329 * units::_g / (units::_mm * units::_mm * units::_mm);
    Material           silicon(X0, L0, A, Z, rho);
    MaterialProperties siliconProperties(silicon, thickness);

    // (A) start
    // create start parameters - neutral/charged doesn't matter
    NeutralCurvilinearParameters sParameters(nullptr, vertex, direction);
    ExtrapolationCell<NeutralParameters> ecc(sParameters);

    // (A1) post-update emulation, creating new parameters
    auto sMatParameters = std::make_unique<const NeutralCurvilinearParameters>(
        nullptr, vertex, direction);
    const NeutralCurvilinearParameters* sMatParametersPtr
        = sMatParameters.get();

    // fill the material step
    ecc.stepMaterial(std::move(sMatParameters),
                     sParameters.position(),
                     sParameters.referenceSurface(),
                     1.,
                     &siliconProperties);

    // first round of checks - size of extrapolation steps
    BOOST_CHECK_EQUAL(1ul, ecc.extrapolationSteps.size());
    // the leadParameters should be the pParameters
    BOOST_CHECK_EQUAL(sMatParametersPtr, ecc.leadParameters);
    // the lastLeadParameters are the sParameters
    BOOST_CHECK_EQUAL(&sParameters, ecc.lastLeadParameters);
    // the path length is still 0
    BOOST_CHECK_EQUAL(0., ecc.pathLength);
    // the thickness in X0 is thickness/X0
    BOOST_CHECK_CLOSE(thickness / X0, ecc.materialX0, 0.001);

    // (B) propagation step
    // create the ExtrapolationCell from the start parameters

    // now emulate a propagator
    auto pParameters = std::make_unique<const NeutralCurvilinearParameters>(
        nullptr, plane, direction);
    double pathLength = 5.;
    // cache the pointer result
    const NeutralCurvilinearParameters* pParametersPtr = pParameters.get();
    // emulate the step
    ecc.stepTransport(
        std::move(pParameters), {ExtrapolationMode::Propagation}, pathLength);

    // second round of checks
    // there should be one step
    BOOST_CHECK_EQUAL(2ul, ecc.extrapolationSteps.size());
    // the leadParameters should be the pParameters
    BOOST_CHECK_EQUAL(pParametersPtr, ecc.leadParameters);
    // the lastLeadParameters should be the sParameters
    BOOST_CHECK_EQUAL(sMatParametersPtr, ecc.lastLeadParameters);
    /// the step length should be the pathLength
    BOOST_CHECK_EQUAL(pathLength, ecc.pathLength);
    // the thickness in X0 is thickness/X0
    BOOST_CHECK_CLOSE(thickness / X0, ecc.materialX0, 0.001);

    // (B1) full-update emulation, creating new parameters
    auto pMatParameters = std::make_unique<const NeutralCurvilinearParameters>(
        nullptr, plane, direction);
    const NeutralCurvilinearParameters* pMatParametersPtr
        = pMatParameters.get();

    // fill the material step
    ecc.stepMaterial(std::move(pMatParameters),
                     plane,
                     pMatParametersPtr->referenceSurface(),
                     1.,
                     &siliconProperties);

    // third round of checks
    // there should be three steps
    BOOST_CHECK_EQUAL(3ul, ecc.extrapolationSteps.size());
    // the leadParameters should be the pParameters
    BOOST_CHECK_EQUAL(pMatParametersPtr, ecc.leadParameters);
    // the lastLeadParameters should be the sParameters
    BOOST_CHECK_EQUAL(pParametersPtr, ecc.lastLeadParameters);
    /// the step length should be the pathLength
    BOOST_CHECK_EQUAL(pathLength, ecc.pathLength);
    // the thickness in X0 is two times thickness/X0
    BOOST_CHECK_CLOSE(2. * thickness / X0, ecc.materialX0, 0.001);

    // (C) destination
    // emulate propagation to final surface
    auto dParameters = std::make_unique<const NeutralCurvilinearParameters>(
        nullptr, destination, direction);
    // cache the pointer result
    const NeutralCurvilinearParameters* dParametersPtr = dParameters.get();
    // emulate the step
    ecc.stepTransport(
        std::move(dParameters), {ExtrapolationMode::Destination}, pathLength);
    // the step parameters should be a nullptr
    const NeutralCurvilinearParameters* nPtr = nullptr;
    // fourth round of checks
    BOOST_CHECK_EQUAL(4ul, ecc.extrapolationSteps.size());
    // the leadParameters should be the pParameters
    BOOST_CHECK_EQUAL(dParametersPtr, ecc.leadParameters);
    // the lastLeadParameters should be the sParameters
    BOOST_CHECK_EQUAL(pMatParametersPtr, ecc.lastLeadParameters);
    /// the step length should be the pathLength
    BOOST_CHECK_EQUAL(2 * pathLength, ecc.pathLength);
    // the thickness in X0 is still two times thickness/X0
    BOOST_CHECK_CLOSE(2. * thickness / X0, ecc.materialX0, 0.001);
    // it *thinks* it would be the last step so
    BOOST_CHECK_EQUAL(nPtr, ecc.extrapolationSteps.back().parameters.get());
    // the end parameters should be the the dParametersPtr
    BOOST_CHECK_EQUAL(ecc.endParameters.get(), dParametersPtr);

    // however, after the final propagtion we find out, there's still
    // material to be handled
    // (C1) pre-updated for final material
    auto dMatParameters = std::make_unique<const NeutralCurvilinearParameters>(
        nullptr, destination, direction);
    const NeutralCurvilinearParameters* dMatParametersPtr
        = dMatParameters.get();
    // fill the material step
    ecc.stepMaterial(std::move(dMatParameters),
                     destination,
                     dMatParametersPtr->referenceSurface(),
                     1.,
                     &siliconProperties);
    // this final step overwrites the formal destination step
    // fifth round of checks
    BOOST_CHECK_EQUAL(5ul, ecc.extrapolationSteps.size());
    // the 4th step parameters should now be filled
    BOOST_CHECK_EQUAL(ecc.extrapolationSteps[3].parameters.get(),
                      dParametersPtr);
    // the leadParameters should be the pParameters
    BOOST_CHECK_EQUAL(dMatParametersPtr, ecc.leadParameters);
    // the lastLeadParameters should be the sParameters
    BOOST_CHECK_EQUAL(dParametersPtr, ecc.lastLeadParameters);
    /// the step length should be the pathLength
    BOOST_CHECK_EQUAL(2 * pathLength, ecc.pathLength);
    // the thickness in X0 is still two times thickness/X0
    BOOST_CHECK_CLOSE(3. * thickness / X0, ecc.materialX0, 0.001);
    // it *thinks* it would be the last step so
    BOOST_CHECK_EQUAL(nPtr, ecc.extrapolationSteps.back().parameters.get());
    // the end parameters should be the the dParametersPtr
    BOOST_CHECK_EQUAL(ecc.endParameters.get(), dMatParametersPtr);
  }
}  // end of namespace Test
}  // end of namespace Acts
