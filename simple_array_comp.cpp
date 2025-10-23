#include "cldef.hpp"

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <array_size>" << std::endl;
    return 1;
  }

  const size_t array_size = std::stoul(argv[1]);

  // Select default platform and device
  cl::Platform default_platform = select_default_platform();
  cl::Device default_device = select_default_device(default_platform, CL_DEVICE_TYPE_GPU);

  // Create context and command queue
  cl::Context context(default_device);
  cl::CommandQueue queue(context, default_device);

  // Kernel code
  const char *kernel_code = R"(
    __kernel void square_array(__global const float* input, __global float* output) {
      int id = get_global_id(0);
      output[id] = input[id] * input[id];
    }
  )";

  // Creating arrays (host memory)
  float *input_array = new float[array_size];
  float *output_array = new float[array_size];

  // Initialize input array
  for (size_t i = 0; i < array_size; ++i)
  {
    input_array[i] = static_cast<float>(i);
  }

  // Create buffers (device memory)
  cl::Buffer input_buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, array_size * sizeof(float), input_array);
  cl::Buffer output_buffer(context, CL_MEM_WRITE_ONLY, array_size * sizeof(float));

  // Creating program and building it
  cl::Program program = create_and_build_program(context, default_device, std::string(kernel_code));

  // Create kernel
  cl::Kernel kernel(program, "square_array");

  // Set kernel arguments
  kernel.setArg(0, input_buffer);
  kernel.setArg(1, output_buffer);

  // Enqueue kernel execution
  cl::NDRange global(array_size);
  queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, cl::NullRange);

  // Read back the results
  queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, array_size * sizeof(float), output_array);

  // Print results
  for (size_t i = 0; i < array_size; ++i)
  {
    std::cout << "Input: " << input_array[i] << " -> Output: " << output_array[i] << std::endl;
  }

  // Clean up
  delete[] input_array;
  delete[] output_array;

  return 0;
}
