#include<stdio.h>
#include<math.h>	//fabs()

// OpenCl Headers
#include<cl/opencl.h>

#include"helper_timer.h"

// global variables
const int iNumberOfArrayElements = 11444777;

cl_platform_id oclPlatformID;
cl_device_id oclDeviceID;

cl_context oclContext;
cl_command_queue oclCommandQueue;

cl_program oclProgram;
cl_kernel oclKernel;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;
float* gold = NULL;

cl_mem deciceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem DeviceOutput = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

//OpenCl Kernel
const char* oclSourceCode = 
"__kernel void vecAddGPU(__gloabal float *in1, __gloabal float *in2, __gloabal float *out,int len)"\
"{"\
"int i=get_global_id(0);"\
"if(i<len)"\
"{"\
"out[i]in1[i]+in2[i];"\
"}"
"}";

// entry point Function
int main(void)
{
	// function declarations
	void fillFloatArrayWithRAndomNumbers(flaot * int);
	size_t roundGloabalSizeToNearestMultipleOfLocalSize(int, unsigned int);
	void VecAdd(const float*, const float*, int);
	void cleanup(void);

	// variable declarations 
	int size = iNumberOfArrayElements = sizeof(float);
	cl_int result;

	// code 
	// host Memory Alloacation
	hostInput1 = (float*)malloc(size);
	if (hostInput1 == NULL)
	{
		printf("Host Memory Allocation is failed for hostInput1 array.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	hostInput2 = (float*)malloc(size);
	if (hostInput2 == NULL)
	{
		printf("Host Memory Allocation is failed for hostInput2 array.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	hostOutput = (float*)malloc(size);
	if (hostOutput == NULL)
	{
		printf("Host Memory Allocation is failed for hostOutput array.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	gold = (float*)malloc(size);
	if (gold == NULL)
	{
		printf("Host Memory Allocation is failed for gold array.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// filling values into Host arrays
	fillFloatArrayWithRAndomNumbers(hostInput1, iNumberOfArrayElements);
	fillFloatArrayWithRAndomNumbers(hostInput2, iNumberOfArrayElements);

	// get OpenCl Supporting Platform id's
	result = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clGetPlatformIDs() Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// get openCl Supporting CPU Devices 
	result = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 1, &oclDeviceID, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clGetDeviceIDs() Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}


	// Create OpenCl Compute context
	oclContext = clCreateContext(NULL, 1, &oclDeviceID, NULL, NULL, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateContext() Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	//Create command queue
	oclCommandQueue = clCreateCommandQueue(oclContext, oclDeviceID, 0, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateCommandQueue() Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Create Opencl Program from .cl
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**)oclSourceCode, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateProgramWithSource() Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// build OpenCl program
	result = clBuildProgram(oclProgram, 0, NULL, NULL, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];
		clGetProgramBuildInfo(oclProgram, oclDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("program build log : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create opencl kernel by pssing kernel function name thart we used in .cl file
	oclKernel = clCreateKernel(oclProgram, "vecAddGPU", &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateProgramWithSource Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Device memory Aloocation
	DeviceInput1 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	DeviceInput2 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	DeviceOutput = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 0th argument i.e deviceInput1
	result = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&DeviceOutput);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg Failed for 1st argument : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 1th argument i.e deviceInput2
	result = clSetKernelArg(oclKernel, 1, sizeof(cl_mem), (void*)&DeviceOutput);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg Failed for 2nd argument : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 2nd argument i.e deviceOutput
	result = clSetKernelArg(oclKernel, 2, sizeof(cl_mem), (void*)&DeviceOutput);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg Failed for 3rd argument : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 3rd argument i.e len
	result = clSetKernelArg(oclKernel, 3, sizeof(cl_mem), (void*)&DeviceOutput);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg Failed for 4thargument : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// write above input device buffer to memory
	result = clEnqueueWriteBuffer(oclCommandQueue, DeviceInput1, CL_FALSE, 0, size, HostInput1, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriteBuffer Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// write above input device buffer to memory
	result = clEnqueueWriteBuffer(oclCommandQueue, DeviceInput2, CL_FALSE, 0, size, HostInput2, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriteBuffer Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// kernel configuration
	size_t loacalWorkSize = 5;
	size_t globalWorkSize = 256;
	globalWorkSize = roundGloabalSizeToNearestMultipleOfLocalSize(loacalWorkSize, iNumberOfArrayElements);

	// start timer 
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	result = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 1, NULL, &globalWorkSize, &loacalWorkSize, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueNDRangeKernel Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// finish OpenCl Command Queue
	clFinish(oclCommandQueue);

	// stop timer 
	sdkStopTimer(&timer);
	timeOnGPU = sdkGetTimerValue(&timer);
	
	sdkDeleteTimer(&timer);

	// read back result fromm device (i.e from device output) into cpu variable (i.e host output)
	result = clEnqueueReadBuffer(oclCommandQueue, DeviceOutput, CL_TRUE, 0, size, HostOutput, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueReadBuffer Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	// vector Addition On host 
	VecAddCPU(hostInput1, hostInput2, gold, iNumberOfArrayElements);

	// comparison
	const float epsilon = 0.000001f;
	int breakValue = -1;
	bool bAccuracy = true;
	for (int i = 0; i < iNumberofArrayelements; i++)
	{
		float val1 = gold[i];
		float val2 = hostoutput[i];
		if (fabs(val1 - val2) > epsilon)
		{
			bAccuracy = false;
			breakValue = i;
			break;
		}
	}

	char str[128];
	if (bAccuracy == false)
	{
		sprintf(str, "Comparison Of CPU and GPU Vector Addition is Not Within Accuracy of 0.000001 at array index %d", breakValue);
	}
	else
	{
		sprintf(str, "Comparison Of CPU and GPU Vector Addition is Not Within Accuracy of 0.000001 at array index %d");
	}
	// output 
	printf("Array1 begins from 0th index %.6f to %.dth index %.6f\n", hostInput1[0], iNumberOfArrayElements - 1, hostInput1[iNumberOfArrayElements - 1]);
	printf("Array2 begins from 0th index %.6f to %.dth index %.6f\n", hostInput2[0], iNumberOfArrayElements - 1, hostInput2[iNumberOfArrayElements - 1]);
	printf("OpenCl Kernel Global Work Size=%lu and local Work Size =  %lu\n", globalWorkSize, loacalWorkSize);
	printf("Output Array Begins From 0th index %.6f to %dth index %.6f\n", hostOutput[0], iNumberOfArrayElements - 1, hostOutput[iNumberOfArrayElements - 1]);
	printf("Time taken for Vector Addition On CPU = %.6f\n", timeOnCPU);
	printf("Time Take For Vector Addition On GPU = .6f\n", timeOnGPU);

	// cleanup
	cleanup();

	return 0;
}

void fillFloatArrayWithRandomNumbers(float* arr, int len)
{
	// code
	const float fscale = 1.0f / (float)RAND_MAX;
	for (int i = 0; i < len; i++)
	{
		arr[i] = fscale * rand();
	}
}

void vecAddCPU(const float* arr1, const float* arr2, float* out, int len)
{
	// code 
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (int i = 0; i < len; i++)
	{
		out[i] = arr[i] + arr2[i];
	}
	sdkStopTimer(&timer);
	timeOnCPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;
}

size_t roundGlobalSizeToNearestMultipleOfLocalSize(int local_size, unsigned int global_size)
{
	// code 
	unsigned int r = global_size % local_size;
	if (r == 0)
	{
		return global_size;
	}
	else
	{
		return global_size + local_size - r;
	}
}

void cleanup(void)
{
	// code
	if (DeviceOutput)
	{
		clReleaseMemObject(DeviceOutput);
		DeviceOutput = NULL;
	}

	if (DeviceInput1)
	{
		clReleaseMemObject(DeviceInput1);
		DeviceInput1 = NULL;
	}

	if (DeviceInput2)
	{
		clReleaseMemObject(DeviceInput2);
		DeviceInput2 = NULL;
	}

	if (oclKernel)
	{
		clReleaseKernel(oclKernel);
		oclKernel = NULL;
	}

	if (oclProgram)
	{
		clReleaseProgram(oclProgram);
		oclKernel = NULL;
	}

	if (oclCommandQueue)
	{
		clReleaseCommandQueue(oclCommandQueue);
		oclCommandQueue = NULL;
	}

	if (oclContext)
	{
		clReleaseContext(oclContext);
		oclContext = NULL;
	}

	if (HostOutput)
	{
		free(HostOutput);
		HostOutput = NULL;
	}

	if (HostInput1)
	{
		free(HostInput1);
		HostInput1 = NULL;
	}

	if (HostInput2)
	{
		free(HostInput2);
		HostInput2 = NULL;
	}
}
