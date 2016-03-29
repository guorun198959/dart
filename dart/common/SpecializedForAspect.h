/*
 * Copyright (c) 2015, Georgia Tech Research Corporation
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

#ifndef DART_COMMON_SPECIALIZEDFORASPECT_H_
#define DART_COMMON_SPECIALIZEDFORASPECT_H_

#include "dart/common/Composite.h"
#include "dart/common/CompositeJoiner.h"
#include "dart/common/Virtual.h"

namespace dart {
namespace common {

/// Declaration of the variadic template
template <class... OtherSpecAspects>
class SpecializedForAspect { };

//==============================================================================
/// SpecializedForAspect allows classes that inherit Composite to have
/// constant-time access to a specific type of Aspect
template <class SpecAspect>
class SpecializedForAspect<SpecAspect> : public virtual Composite
{
public:

  /// Default Constructor
  SpecializedForAspect();

  /// Check if this Composite currently has a certain type of Aspect
  template <class T>
  bool has() const;

  /// Get a certain type of Aspect from this Composite
  template <class T>
  T* get();

  /// Get a certain type of Aspect from this Composite
  template <class T>
  const T* get() const;

  /// Make a clone of the aspect and place the clone into this Composite. If
  /// an Aspect of the same type already exists in this Composite, the
  /// existing Aspect will be destroyed.
  template <class T>
  void set(const T* aspect);

  /// Use move semantics to place an aspect into this Composite. If an Aspect
  /// of the same type already exists in this Composite, the existing Aspect
  /// will be destroyed.
  template <class T>
  void set(std::unique_ptr<T>&& aspect);

  /// Construct an Aspect inside of this Composite
  template <class T, typename ...Args>
  T* create(Args&&... args);

  /// Remove an Aspect from this Composite
  template <class T>
  void erase();

  /// Remove an Aspect from this Composite, but return its unique_ptr instead
  /// of letting it be deleted. This allows you to safely use move semantics to
  /// transfer an Aspect between two Composites.
  template <class T>
  std::unique_ptr<T> release();

  /// Check if this Composite is specialized for a specific type of Aspect
  template <class T>
  static constexpr bool isSpecializedFor();

protected:

  /// Redirect to Composite::has()
  template <class T>
  bool _has(type<T>) const;

  /// Specialized implementation of has()
  bool _has(type<SpecAspect>) const;

  /// Redirect to Composite::get()
  template <class T>
  T* _get(type<T>);

  /// Specialized implementation of get()
  SpecAspect* _get(type<SpecAspect>);

  /// Redirect to Composite::get()
  template <class T>
  const T* _get(type<T>) const;

  /// Specialized implementation of get()
  const SpecAspect* _get(type<SpecAspect>) const;

  /// Redirect to Composite::set()
  ///
  /// Using the type<T> tag for this is not be necessary, but it helps to avoid
  /// confusion between the set() versus _set() function.
  template <class T>
  void _set(type<T>, const T* aspect);

  /// Specialized implementation of set()
  void _set(type<SpecAspect>, const SpecAspect* aspect);

  /// Redirect to Composite::set()
  ///
  /// Using the type<T> tag for this is not be necessary, but it helps to avoid
  /// confusion between the set() versus _set() function.
  template <class T>
  void _set(type<T>, std::unique_ptr<T>&& aspect);

  /// Specialized implementation of set()
  void _set(type<SpecAspect>, std::unique_ptr<SpecAspect>&& aspect);

  /// Redirect to Composite::create()
  template <class T, typename ...Args>
  T* _create(type<T>, Args&&... args);

  /// Specialized implementation of create()
  template <typename ...Args>
  SpecAspect* _create(type<SpecAspect>, Args&&... args);

  /// Redirect to Composite::erase()
  template <class T>
  void _erase(type<T>);

  /// Specialized implementation of erase()
  void _erase(type<SpecAspect>);

  /// Redirect to Composite::release()
  template <class T>
  std::unique_ptr<T> _release(type<T>);

  /// Specialized implementation of release()
  std::unique_ptr<SpecAspect> _release(type<SpecAspect>);

  /// Return false
  template <class T>
  static constexpr bool _isSpecializedFor(type<T>);

  /// Return true
  static constexpr bool _isSpecializedFor(type<SpecAspect>);

  /// Iterator that points to the Aspect of this SpecializedForAspect
  Composite::AspectMap::iterator mSpecAspectIterator;

};

//==============================================================================
/// This is the variadic version of the SpecializedForAspect class which
/// allows you to include arbitrarily many specialized types in the
/// specialization.
template <class SpecAspect1, class... OtherSpecAspects>
class SpecializedForAspect<SpecAspect1, OtherSpecAspects...> :
    public CompositeJoiner< Virtual< SpecializedForAspect<SpecAspect1> >,
                               Virtual< SpecializedForAspect<OtherSpecAspects...> > > { };

} // namespace common
} // namespace dart

#include "dart/common/detail/SpecializedForAspect.h"

#endif // DART_COMMON_SPECIALIZEDFORASPECT_H_