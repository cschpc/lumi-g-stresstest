//author T.Lunttila, CSC


#include "hip/hip_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <chrono>
#include <iostream>

using namespace std;

constexpr int error_exit_code = -1;
#define HIP_CHECK(condition)                                                          \
  {                                                                                   \
    const hipError_t error = condition;                                               \
    if(error != hipSuccess)                                                           \
    {                                                                                 \
      std::cerr << "An error encountered: \"" << hipGetErrorString(error) << "\" at " \
                << __FILE__ << ':' << __LINE__ << std::endl;                          \
      std::exit(error_exit_code);                                                     \
    }                                                                                 \
  }


__global__ void busy_loop(double *array, double *out, uint64_t max, uint32_t repeat) {

  uint64_t idx = threadIdx.x + blockIdx.x * blockDim.x;
 
  double temp = 0;
  for (uint32_t j=0; j<repeat; ++j) { 
    temp+=sin(array[(idx+j)%max])*cos(array[(idx+j)%max]);
  }
  out[idx] = temp;
}


int main(int argc, char* argv[])
{
  int rank, wsize, namelen;
  char nodename[MPI_MAX_PROCESSOR_NAME];
  char busID[80];
  uint32_t repeat = 64; 
  uint64_t size = static_cast<uint64_t>(256) * 1024 * 1024;
  std::vector<double> array(size);
  double *d_array, *d_out;
  hipDevice_t deviceID;

  for (int i=0;i<size;++i) array[i] = 1.0*i/size;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &wsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(nodename, &namelen);

  if (argc>1) repeat = atoi(argv[1]);

  HIP_CHECK(hipMalloc(&d_array, size * sizeof(double)));
  HIP_CHECK(hipMalloc(&d_out, size * sizeof(double)));
  HIP_CHECK(hipMemcpy(d_array, array.data(), size * sizeof(uint64_t), hipMemcpyHostToDevice));

  auto start = std::chrono::high_resolution_clock::now();

  hipLaunchKernelGGL(busy_loop, dim3(size/256), dim3(256), 0, hipStreamDefault, d_array, d_out, size, repeat);
  HIP_CHECK(hipGetLastError());
  HIP_CHECK(hipStreamSynchronize(hipStreamDefault));
 
  HIP_CHECK(hipMemcpy(array.data(), d_out, size * sizeof(double), hipMemcpyDeviceToHost));
  HIP_CHECK(hipFree(d_array));
  HIP_CHECK(hipFree(d_out));

  HIP_CHECK(hipDeviceGet(&deviceID, 0));
  HIP_CHECK(hipDeviceGetPCIBusId(busID, 80, deviceID));

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  
  std::cout << "node: " << nodename << " GPU Bus ID: " << busID << " time: " << duration.count()/1000.0 << std::endl;
  MPI_Finalize();
}
