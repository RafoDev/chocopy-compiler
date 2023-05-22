#include "MToken.h"

std::ostream &operator<<(std::ostream &os, const MToken &token)
{
  os << "Type: " << token.type << "\n";
  os << "Value: " << token.value << "\n";
  os << "Line: " << token.line << "\n";
  return os;
}