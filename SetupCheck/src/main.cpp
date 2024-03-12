#include <setupcheck/pch/precomp.h>

constexpr auto DataPath = "data";
constexpr auto TeamFile = "equipe.yaml";

[[nodiscard]] bool ValidateVulkan();

[[nodiscard]] static bool ValidateTeam()
{
  std::filesystem::path filePath = DataPath;
  filePath.append(TeamFile);

  std::ifstream data(filePath, std::ios::in);
  if (!data.is_open())
  {
    fmt::print(stderr, "Échec d'ouverture de {}\n", TeamFile);
    return false;
  }

  YAML::Node root = YAML::Load(data);
  auto equipeNode = root["equipe"];
  if (equipeNode.Type() != YAML::NodeType::Map || std::size(equipeNode) == 0)
  {
    fmt::print(stderr, "N'oubliez pas d'inscrire les membres de votre équipe dans le fichier data/equipe.yaml!\n");
    return false;
  }

  std::regex codeMatch("^[A-Z]{4}[0-9]{8}$");

  for (const auto item : equipeNode)
  {
    auto code = item.first.as<std::string>();
    auto name = item.second.as<std::string>();
    if (!std::regex_match(code, codeMatch))
    {
      fmt::print(stderr, "Code permanent invalide: {}!\n", code);
      return false;
    }
  }

  return true;
}

[[nodiscard]] static bool ValidateCompiler()
{
#if defined(__MINGW32__) || defined(__CYGWIN__)
  fmt::print(stderr, "Vous devez utiliser un compilateur supportant l'ABI MSVC pour Windows\n");
  return false;
#else
  return true;
#endif
}

[[nodiscard]] static bool Validate64Bits()
{
  constexpr auto _64Bits = 8;

  size_t addrSize = sizeof(void*);
  if (addrSize != _64Bits)
  {
    fmt::print(stderr, "Vous devez compiler pour une architecture 64 bits (ex.: amd64)\n");
    return false;
  }

  return true;
}

#ifdef WIN32
extern "C"
{
  __declspec(dllexport) DWORD NvOptimusEnablement = 1;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char* argv[])
{
  bool valid = true;
  valid &= ValidateTeam();

  if (!ValidateVulkan())
  {
    fmt::print(stderr, "Échec à la validation de Vulkan\n");
    valid = false;
  }

#ifdef _WIN32
  valid &= ValidateCompiler();
  valid &= Validate64Bits();
#endif

  if (valid)
  {
    fmt::print("Votre installation est validée.\n");
  }

  assert(valid);

  return valid ? 0 : -1;
}