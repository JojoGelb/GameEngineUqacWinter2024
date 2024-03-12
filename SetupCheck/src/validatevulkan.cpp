#include <setupcheck/pch/precomp.h>

namespace
{
  constexpr auto ValidationLayers = std::array{"VK_LAYER_KHRONOS_validation"};
  constexpr auto DeviceExtensions = std::array{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  struct QueueFamilyIndices
  {
    std::optional<std::uint32_t> graphicsFamily;
    std::optional<std::uint32_t> presentFamily;

    [[nodiscard]] bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    [[nodiscard]] std::uint32_t GetGraphicsFamily() const
    {
      Expects(graphicsFamily.has_value());
      return graphicsFamily.value();
    }
    [[nodiscard]] std::uint32_t GetPresentFamily() const
    {
      Expects(presentFamily.has_value());
      return presentFamily.value();
    }
  };

  struct SwapChainSupportDetails
  {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
  };

  GLFWwindow* window{};
  vk::Instance instance{};
  vk::SurfaceKHR surface{};
  vk::PhysicalDevice physicalDevice{};

  [[nodiscard]] bool InitSubsystem()
  {
    if (glfwInit() != GLFW_TRUE)
    {
      fmt::print(stderr, "Échec à l'initialisation de GLFW\n");
      return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    return true;
  }

  [[nodiscard]] bool CreateWindowGLFW()
  {
    window = glfwCreateWindow(800, 600, "SetupCheck", nullptr, nullptr);
    if (window == nullptr)
    {
      fmt::print(stderr, "Impossible de créer une fenêtre valide.\n");
      return false;
    }

    return true;
  }

  [[nodiscard]] bool CheckValidationLayerSupport()
  {
    auto availableLayers = vk::enumerateInstanceLayerProperties();
    for (const auto* layerName : ValidationLayers)
    {
      if (!std::any_of(availableLayers.begin(), availableLayers.end(),
                       [layerName](const auto& l) { return std::strcmp(layerName, l.layerName); }))
      {
        return false;
      }
    }

    return true;
  }

  [[nodiscard]] bool CreateInstance()
  {
    if (!CheckValidationLayerSupport())
    {
      fmt::print(stderr, "Couche de validation Vulkan introuvable\n");
      return false;
    }

    if (!CreateWindowGLFW())
    {
      return false;
    }

    vk::ApplicationInfo appInfo{
        .pApplicationName = "SetupCheck",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "SetupCheck",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    std::uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    auto glfwExtensionsSpan = std::span{glfwExtensions, glfwExtensionCount};

    std::vector<const char*> requiredExtensions;
    requiredExtensions.insert(requiredExtensions.end(), glfwExtensionsSpan.begin(), glfwExtensionsSpan.end());

    for (const auto& ext : vk::enumerateInstanceExtensionProperties())
    {
      if (std::strcmp(ext.extensionName, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) == 0)
      {
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
      }
    }

    vk::InstanceCreateInfo createInfo{
        .flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = std::size(ValidationLayers),
        .ppEnabledLayerNames = ValidationLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(std::size(requiredExtensions)),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };

    instance = vk::createInstance(createInfo, nullptr);
    return true;
  }

  [[nodiscard]] bool CreateSurface()
  {
    VkSurfaceKHR raw_surface = VK_NULL_HANDLE;
    VkResult err = glfwCreateWindowSurface(instance, window, nullptr, &raw_surface);
    surface = raw_surface;
    if (err != VK_SUCCESS)
    {
      const char* error_msg{};
      int ret = glfwGetError(&error_msg);
      if (ret != 0)
      {
        fmt::print(stderr, "Échec à la création de la surface ({}). Error: {}\n", ret, error_msg);
      }
      surface = nullptr;
    }

    return !!surface;
  }

  [[nodiscard]] QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device)
  {
    QueueFamilyIndices indices;

    int i = 0;
    for (const auto& queueFamily : device.getQueueFamilyProperties())
    {
      if (device.getSurfaceSupportKHR(i, surface) == VK_TRUE)
      {
        indices.presentFamily = i;
      }

      if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
      {
        indices.graphicsFamily = i;
      }

      i++;
    }

    return indices;
  }

  [[nodiscard]] bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device)
  {
    auto availableExtensions = device.enumerateDeviceExtensionProperties();
    for (const auto* extensionName : DeviceExtensions)
    {
      if (!std::any_of(availableExtensions.begin(), availableExtensions.end(),
                       [extensionName](const auto& e) { return std::strcmp(extensionName, e.extensionName); }))
      {
        return false;
      }
    }

    return true;
  }

  [[nodiscard]] SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& device)
  {
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);

    return details;
  }

  [[nodiscard]] int RateDeviceSuitability(const vk::PhysicalDevice& device)
  {
    constexpr auto Best = 1000;
    constexpr auto Worst = 1;
    constexpr auto Invalid = -1;

    auto indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
      SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if (!indices.IsComplete() || !extensionsSupported || !swapChainAdequate)
    {
      return Invalid;
    }

    auto properties = device.getProperties();
    auto features = device.getFeatures();

    if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
    {
      return Best;
    }

    return Worst;
  }

  [[nodiscard]] bool PickPhysicalDevice()
  {
    auto physicalDevices = instance.enumeratePhysicalDevices();
    std::multimap<int, vk::PhysicalDevice> candidates;

    std::transform(physicalDevices.begin(), physicalDevices.end(), std::inserter(candidates, candidates.begin()),
                   [](const auto& device) { return std::make_pair(RateDeviceSuitability(device), device); });

    if (!candidates.empty() && candidates.rbegin()->first > 0)
    {
      physicalDevice = candidates.rbegin()->second;
      auto properties = physicalDevice.getProperties();
      fmt::print("Le meilleur périphérique trouvé est {}\n", properties.deviceName);
    }
    else
    {
      fmt::print(stderr, "Échec à trouver un périphérique Vulkan\n");
      return false;
    }

    return true;
  }

  [[nodiscard]] bool Cleanup()
  {
    physicalDevice = nullptr;

    instance.destroySurfaceKHR(surface);
    instance.destroy();

    glfwDestroyWindow(window);
    window = nullptr;

    glfwTerminate();

    return true;
  }
} // namespace

[[nodiscard]] bool ValidateVulkan()
{
  return InitSubsystem() && CreateInstance() && CreateSurface() && PickPhysicalDevice() && Cleanup();
}