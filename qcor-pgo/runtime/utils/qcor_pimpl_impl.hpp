/*******************************************************************************
 * Copyright (c) 2018-, UT-Battelle, LLC.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the MIT License 
 * which accompanies this distribution. 
 *
 * Contributors:
 *   Alexander J. McCaskey - initial API and implementation
 *   Thien Nguyen - implementation
 *******************************************************************************/
#pragma once

#include <utility>

namespace qcor {
template <typename T>
qcor_pimpl<T>::qcor_pimpl() : m{new T{}} {}

template <typename T>
qcor_pimpl<T>::qcor_pimpl(const qcor_pimpl<T>& other)
    : m(std::make_unique<T>(other)) {}

template <typename T>
template <typename... Args>
qcor_pimpl<T>::qcor_pimpl(Args&&... args)
    : m{new T{std::forward<Args>(args)...}} {}

template <typename T>
qcor_pimpl<T>::~qcor_pimpl() {}

template <typename T>
T* qcor_pimpl<T>::operator->() {
  return m.get();
}
template <typename T>
T* qcor_pimpl<T>::operator->() const {
  return m.get();
}

template <typename T>
T& qcor_pimpl<T>::operator*() {
  return *m.get();
}

}  // namespace qcor