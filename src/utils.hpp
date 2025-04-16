#include <cstdint>
#include <string>

namespace owo {

uint8_t input_get_uint8_t(const std::string& prompt, uint8_t defaultValue);

uint8_t input_get_uint8_t_and_confirm(const std::string& prompt, uint8_t defaultValue);

}