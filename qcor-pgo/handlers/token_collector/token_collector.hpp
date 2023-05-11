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
#ifndef QCOR_HANDLERS_TOKENCOLLECTOR_HPP_
#define QCOR_HANDLERS_TOKENCOLLECTOR_HPP_

#include "Identifiable.hpp"
#include "clang/Parse/Parser.h"
#include <sstream>

namespace qcor {
class TokenCollector : public xacc::Identifiable {
public:
  virtual void collect(clang::Preprocessor &PP, clang::CachedTokens &Toks,
                       std::vector<std::string> bufferNames,
                       std::stringstream &ss,
                       const std::string &kernel_name = "") = 0;
};

} // namespace qcor

#endif