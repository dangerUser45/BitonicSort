#include <fstream>
#include <sstream>
#include <filesystem>

#include "utils.hpp"

namespace bitsort {

std::string file_to_str(const std::filesystem::path& source_path)
{
    std::ifstream source_file(source_path);
    if(!source_file) {
        throw std::runtime_error("Cannot open file: '" + source_path.string() + "'");
    }

    std::ostringstream buffer;
    buffer << source_file.rdbuf();

    return buffer.str();
}

} // namespace bitsort
