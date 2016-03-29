/*
 * Copyright (c) 2016, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Michael X. Grey <mxgrey@gatech.edu>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DART_COMMON_EMBEDDEDASPECT_H_
#define DART_COMMON_EMBEDDEDASPECT_H_

#include "dart/common/detail/EmbeddedAspect.h"

namespace dart {
namespace common {

//==============================================================================
template <class DerivedT,
          typename StateT,
          class CompositeT,
          void (*setEmbeddedState)(DerivedT*, const StateT&) =
              &detail::DefaultSetEmbeddedState<DerivedT, StateT>,
          const StateT& (*getEmbeddedState)(const DerivedT*) =
              &detail::DefaultGetEmbeddedState<DerivedT, StateT> >
using EmbeddedStateAspect =
    detail::EmbeddedStateAspect<CompositeTrackingAspect<CompositeT>, DerivedT,
                                StateT, setEmbeddedState, getEmbeddedState>;

//==============================================================================
template <class DerivedT,
          typename PropertiesT,
          class CompositeT,
          void (*setEmbeddedProperties)(DerivedT*, const PropertiesT&) =
              &detail::DefaultSetEmbeddedProperties<DerivedT, PropertiesT>,
          const PropertiesT& (*getProperties)(const DerivedT*) =
              &detail::DefaultGetEmbeddedProperties<DerivedT, PropertiesT> >
using EmbeddedPropertiesAspect =
    detail::EmbeddedPropertiesAspect<
        CompositeTrackingAspect<CompositeT>, DerivedT,
        PropertiesT, setEmbeddedProperties, getProperties>;

//==============================================================================
template <class DerivedT,
          typename StateT,
          typename PropertiesT,
          class CompositeT,
          void (*setEmbeddedState)(DerivedT*, const StateT&) =
              &detail::DefaultSetEmbeddedState<DerivedT, StateT>,
          const StateT& (*getEmbeddedState)(const DerivedT*) =
              &detail::DefaultGetEmbeddedState<DerivedT, StateT>,
          void (*setEmbeddedProperties)(DerivedT*, const PropertiesT&) =
              &detail::DefaultSetEmbeddedProperties<DerivedT, PropertiesT>,
          const PropertiesT& (*getEmbeddedProperties)(const DerivedT*) =
              &detail::DefaultGetEmbeddedProperties<DerivedT, PropertiesT> >
class EmbeddedStateAndPropertiesAspect :
    public detail::EmbeddedPropertiesAspect<
        EmbeddedStateAspect<DerivedT, StateT, CompositeT,
                            setEmbeddedState, setEmbeddedProperties>,
        DerivedT, PropertiesT, setEmbeddedProperties, getEmbeddedProperties>
{
public:

  using Derived = DerivedT;
  using State = StateT;
  using Properties = PropertiesT;
  using CompositeType = CompositeT;

  using AspectStateImpl = EmbeddedStateAspect<
      Derived, State, CompositeType, setEmbeddedState, getEmbeddedState>;

  using AspectPropertiesImpl = detail::EmbeddedPropertiesAspect<
      AspectStateImpl, Derived, Properties,
      setEmbeddedProperties, getEmbeddedProperties>;

  using AspectImpl = EmbeddedStateAndPropertiesAspect<
      Derived, State, Properties, CompositeType,
      setEmbeddedState, getEmbeddedState,
      setEmbeddedProperties, getEmbeddedProperties>;

  EmbeddedStateAndPropertiesAspect() = delete;
  EmbeddedStateAndPropertiesAspect(
      const EmbeddedStateAndPropertiesAspect&) = delete;

  /// Construct using a State and Properties instance
  EmbeddedStateAndPropertiesAspect(
      common::Composite* comp,
      const State& state = State(),
      const Properties& properties = Properties())
    : AspectPropertiesImpl(comp, properties, state)
  {
    // Do nothing
  }

  /// Construct using a Properties and State instance
  EmbeddedStateAndPropertiesAspect(
      common::Composite* comp,
      const Properties& properties,
      const State& state = State())
    : AspectPropertiesImpl(comp, properties, state)
  {
    // Do nothing
  }

  // Documentation inherited
  std::unique_ptr<Aspect> cloneAspect(Composite* newComposite) const override
  {
    return make_unique<Derived>(
          newComposite, this->getState(), this->getProperties());
  }

};

} // namespace common
} // namespace dart

#endif // DART_COMMON_EMBEDDEDASPECT_H_