#include "cldef.hpp"
#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> split_string(const std::string &s, char delimiter)
{
  std::vector<std::string> tokens;
  std::string token;
  std::stringstream ss(s);

  while (std::getline(ss, token, delimiter))
  {
    tokens.push_back(token);
  }

  return tokens;
}

int main(int argc, char const *argv[])
{
  try
  {
    // Get all available platforms
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (const cl::Platform &platform : platforms)
    {
      std::cout << "Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

      // Get all GPUs in the platform
      std::vector<cl::Device> devices;
      platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

      // For each device, get its extensions
      for (const cl::Device &device : devices)
      {
        std::cout << " * Device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;

        std::string extensions = device.getInfo<CL_DEVICE_EXTENSIONS>();
        std::vector<std::string> extension_list = split_string(extensions, ' ');

        std::cout << "  - Extensions:" << std::endl;
        for (const std::string &ext : extension_list)
        {
          std::cout << "    -> " << ext << std::endl;
        }
      }
    }
  }
  catch (cl::Error &e)
  {
    std::cerr << "OpenCL Error: " << e.what() << " returned " << e.err() << std::endl;
    return 1;
  }
  catch (std::exception &e)
  {
    std::cerr << "Standard Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
