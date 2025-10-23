#pragma once

#define OPENCL_VERSION 200 // We want to use OpenCL 2.0

#define CL_TARGET_OPENCL_VERSION OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION OPENCL_VERSION
#define CL_HPP_ENABLE_EXCEPTIONS

#include <CL/opencl.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>

inline cl::Platform select_default_platform()
{
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  if (platforms.empty())
  {
    std::cerr << "No OpenCL platforms found." << std::endl;
    throw std::runtime_error("No OpenCL platforms found.");
  }

  return platforms[0];
}

inline cl::Device select_default_device(const cl::Platform &platform, cl_device_type device_type = CL_DEVICE_TYPE_ALL)
{
  std::vector<cl::Device> devices;
  platform.getDevices(device_type, &devices);

  if (devices.empty())
  {
    std::cerr << "No OpenCL devices found for the specified platform." << std::endl;
    throw std::runtime_error("No OpenCL devices found.");
  }

  return devices[0];
}

inline cl::Program create_and_build_program(const cl::Context &context, const cl::Device &device, const std::string &source_code)
{
  cl::Program program(context, source_code);

  try
  {
    program.build(device);
  }
  catch (const cl::BuildError &err)
  {
    std::cerr << "OpenCL Program Build Error" << std::endl;
    std::cerr << "Device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    std::cerr << "Build Log:" << std::endl;
    std::cerr << err.getBuildLog().front().second << std::endl;

    throw std::runtime_error("Failed to build OpenCL program.");
  }

  return program;
}