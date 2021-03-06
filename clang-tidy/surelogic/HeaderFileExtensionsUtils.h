//===--- HeaderFileExtensionsUtils.h - clang-tidy----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_UTILS_HEADER_FILE_EXTENSIONS_UTILS_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_UTILS_HEADER_FILE_EXTENSIONS_UTILS_H

#include <string>

#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"

namespace clang {
namespace tidy {
namespace utils {

typedef llvm::SmallSet<llvm::StringRef, 5> HeaderFileExtensionsSet;

/// \brief Checks whether expansion location of Loc is in header file.
bool isExpansionLocInHeaderFile(
    SourceLocation Loc, const SourceManager &SM,
    const HeaderFileExtensionsSet &HeaderFileExtensions);

/// \brief Checks whether presumed location of Loc is in header file.
bool isPresumedLocInHeaderFile(
    SourceLocation Loc, SourceManager &SM,
    const HeaderFileExtensionsSet &HeaderFileExtensions);

/// \brief Checks whether spelling location of Loc is in header file.
bool isSpellingLocInHeaderFile(
    SourceLocation Loc, SourceManager &SM,
    const HeaderFileExtensionsSet &HeaderFileExtensions);

/// \brief Parses header file extensions from a semicolon-separated list.
bool parseHeaderFileExtensions(StringRef AllHeaderFileExtensions,
                               HeaderFileExtensionsSet &HeaderFileExtensions,
                               char delimiter);

} // namespace utils
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_UTILS_HEADER_FILE_EXTENSIONS_UTILS_H
