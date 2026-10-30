#include "Arduino.h"
namespace sim { void registerBoard(int, void(*)(), void(*)()); int run(int, char**); }
namespace board0 {
BOARD_API(0)
#include "/Users/blinkyy/textbooks/embedded-systems/kit/build/sk_258c916256_b0.ino.cpp"
}
int main(int argc, char** argv) {
  sim::registerBoard(0, board0::setup, board0::loop);
  return sim::run(argc, argv);
}
