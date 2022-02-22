// Copyright 2017-2020 The Verible Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "verilog/tools/kythe/kythe_facts.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

#include "absl/strings/escaping.h"
#include "absl/strings/string_view.h"
#include "common/util/spacer.h"

namespace verilog {
namespace kythe {

std::string Signature::ToString() const {
  std::string signature;
  for (absl::string_view name : names_) {
    if (name.empty()) continue;
    absl::StrAppend(&signature, name, "#");
  }
  return signature;
}

std::string Signature::ToBase64() const {
  return absl::Base64Escape(ToString());
}

bool VName::operator==(const VName& other) const {
  return path == other.path && root == other.root && corpus == other.corpus &&
         signature == other.signature && language == other.language;
}

std::ostream& VName::FormatJSON(std::ostream& stream, bool debug,
                                int indentation) const {
  // Output new line only in debug mode.
  const std::string separator = debug ? "\n" : "";
  stream << "{" << separator;
  const verible::Spacer idt(indentation + 2);
  stream << idt << "\"signature\": \""
         << (debug ? signature.ToString() : signature.ToBase64()) << "\","
         << separator << idt << "\"path\": \"" << path << "\"," << separator
         << idt << "\"language\": \"" << language << "\"," << separator << idt
         << "\"root\": \"" << root << "\"," << separator << idt
         << "\"corpus\": \"" << corpus << "\"" << separator
         << verible::Spacer(indentation) << "}";
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const VName& vname) {
  return vname.FormatJSON(stream, /*debug=*/true);
}

bool Fact::operator==(const Fact& other) const {
  return fact_value == other.fact_value && fact_name == other.fact_name &&
         node_vname == other.node_vname;
}

std::ostream& Fact::FormatJSON(std::ostream& stream, bool debug,
                               int indentation) const {
  // Output new line only in debug mode.
  const std::string separator = debug ? "\n" : "";
  // Indent entries in debug mode
  const int indent_more = debug ? indentation + 2 : 0;
  const verible::Spacer idt(indent_more);
  stream << "{" << separator;
  {
    stream << idt << "\"source\": ";
    node_vname.FormatJSON(stream, debug, indent_more) << "," << separator;
  }
  stream << idt << "\"fact_name\": \"" << fact_name << "\"," << separator  //
         << idt << "\"fact_value\": \""
         << (debug ? fact_value : absl::Base64Escape(fact_value)) << "\""
         << separator  //
         << verible::Spacer(indentation) << "}";
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const Fact& fact) {
  return fact.FormatJSON(stream, /*debug=*/true);
}

bool Edge::operator==(const Edge& other) const {
  return edge_name == other.edge_name && source_node == other.source_node &&
         target_node == other.target_node;
}

std::ostream& Edge::FormatJSON(std::ostream& stream, bool debug,
                               int indentation) const {
  // Output new line only in debug mode.
  const std::string separator = debug ? "\n" : "";
  // Indent entries in debug mode
  const int indent_more = debug ? indentation + 2 : 0;
  const verible::Spacer idt(indent_more);
  stream << "{" << separator;
  {
    stream << idt << "\"source\": ";
    source_node.FormatJSON(stream, debug, indent_more) << "," << separator;
  }
  { stream << idt << "\"edge_kind\": \"" << edge_name << "\"," << separator; }
  {
    stream << idt << "\"target\": ";
    target_node.FormatJSON(stream, debug, indent_more) << "," << separator;
  }
  { stream << idt << "\"fact_name\": \"/\"" << separator; }
  return stream << verible::Spacer(indentation) << "}";
}

std::ostream& operator<<(std::ostream& stream, const Edge& edge) {
  return edge.FormatJSON(stream, /*debug=*/true);
}

}  // namespace kythe
}  // namespace verilog
