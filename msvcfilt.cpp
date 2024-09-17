#include <array>
#include <cstring>
#include <iostream>
#include <optional>
#include <print>
#include <string>
#include <string_view>

#define WIN32_LEAN_AND_MEAN
// clang-format off
#include <Windows.h>
#include <DbgHelp.h>
// clang-format on

class SymbolHandler {
public:
  SymbolHandler() : hProc_(GetCurrentProcess()) {
    if (SymInitialize(hProc_, NULL, FALSE)) {
      initialized_ = true;
    }
  }

  ~SymbolHandler() {
    if (initialized_) {
      SymCleanup(hProc_);
    }
  }

  /// @param symbol The symbol to undecorate - this view must be null terminated
  std::optional<std::string_view> undecorateSymbol(std::string_view symbol) {
    auto res = UnDecorateSymbolName(symbol.data(), undecorateBuffer_.data(),
                                    undecorateBuffer_.size(), UNDNAME_COMPLETE);
    if (res) {
      return this->undecorateBuffer_.data();
    }

    return std::nullopt;
  }

  bool initialized() const { return this->initialized_; }

private:
  /// True if the instance was successfully initialized
  bool initialized_ = false;
  /// Windows handle for the current process
  HANDLE hProc_;
  /// Internal buffer that receives the undecorated symbols
  std::array<char, MAX_SYM_NAME> undecorateBuffer_;
};

std::string_view skipFilename(std::string_view input) {
  auto colonIt = input.find(':');
  if (colonIt == std::string_view::npos) {
    return input;
  }
  return input.substr(colonIt + 1);
}

int main(int argc, char **argv) {
  SymbolHandler handler;
  if (!handler.initialized()) {
    std::println(stderr, "Failed to initialize with SymInitialize");
    return 1;
  }

  while (std::cin) {
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) {
      if (std::cin) {
        std::println("?"); // not 100% what to return here...
      }
      continue;
    }
    std::string_view input = skipFilename(line);

    auto undecorated = handler.undecorateSymbol(input);

    if (!undecorated) {
      std::println(stderr, "Failed to undecorate {}", line);
      return 1;
    }
    if (input.data() == line.data()) {
      std::println("{}", *undecorated);
    } else {
      std::println("{}{}", std::string_view{line.data(), input.data()},
                   *undecorated);
    }
  }
}
