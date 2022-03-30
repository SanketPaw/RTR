//header files
//standard headers
#include<stdio.h>
#include<stdlib.h>	

//OpenCl headers
#include<CL/opencl.h>

//Global Variables 
const int iNumberOfArrayElements = 5;

cl_platform_id oclPlatformId;	// Class of GPU Id
cl_device_id oclDeviceID;		// GPU Compute ID

cl_context oclContext;			// A Structure Which Tells state of technology
cl_command_queue oclCommandQueue;	// Opencl Sends the command through queue to compute device

cl_program oclProgram;	//Program
cl_kernel oclKernel;	//kernel

float* HostInput1 = NULL;
float* HostInput2 = NULL;
float* HostOutput = NULL;

cl_mem DeviceInput1 = NULL;
cl_mem DeviceInput2 = NULL; 
cl_mem DeviceOutput = NULL;

// OpenCl Kernel
const char* oclSourceCode =
"__kernel void vecAdd(__gloabal float *in1,__global float *in2,__gloabalfloat*out,int len)"	\
"{" \
"int i=get_gloabal_id(0);" \
"if(i<len)" \
"{""out[i]=in1[i]+in2[i];" \
"}" \
"}";

//entry point functions 
int main(void)
{
	//function decalration
	void cleanup(void);

	//variable declaration
	int size = iNumberOfArrayElements * sizeof(float);
	cl_int result;
	
	//code
	//host memory allocation
	HostInput1 = (float*)malloc(size);
	if (HostInput1 == NULL)
	{
		printf("Host Memoryc Allocation is failed for hostInput1 Array.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	HostInput2 = (float*)malloc(size);
	if (HostInput2 == NULL)
	{
		printf("Host Memoryc Allocation is failed for hostInput2 Array.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	HostOutput = (float*)malloc(size);
	if (HostOutput == NULL)
	{
		printf("Host Memoryc Allocation is failed for hostOutput Array.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	//filling values into host Arrays
	HostInput1[0] = 101.0;
	HostInput1[1] = 102.0;
	HostInput1[2] = 103.0;
	HostInput1[3] = 104.0;
	HostInput1[4] = 105.0;
	
	HostInput2[0] = 101.0;
	HostInput2[1] = 202.0;
	HostInput2[2] = 203.0;
	HostInput2[3] = 204.0;
	HostInput2[4] = 205.0;

	// get Opencl supporting platform ids
	result = clGetPlatformIDs(1, &oclPlatformId, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clGetPlatformIDs Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// get OpenCl Supporting Cpu device's Id
	result = clGetDeviceIDs(oclPlatformId, CL_DEVICE_TYPE, 1, &oclDeviceID, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clGetDeviceIDs Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create opncl compute context
	oclContext = clCreateContext(NULL, 1, &oclDeviceID, NULL, NULL, &result); 
	if (result != CL_SUCCESS)
	{
		printf("clCreateContext Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	//create Command Queue
	oclCommandQueue = clCreateCommandQueueWithProperties(oclContext, oclDeviceID, 0, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateCommandQueue Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create opncl program from .cl
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**)&oclSourceCode, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateProgramWithSource Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// build Opencl Program
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

	// kernel configurations
	size_t global_size = 5;		// 1-D 5 element array operation
	result = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 1, NULL, &global_size , NULL, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueNDRangeKernel Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	//finish opencl command queue
	clFinish(oclCommandQueue);

	// read back result from the device into cpu variable
	result = clEnqueueReadBuffer(oclCommandQueue, DeviceOutput, CL_TRUE, 0, size, HostOutput, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueReadBuffer Failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	//display results
	int i;
	for (i = 0; i < iNumberOfArrayElements; i++)
	{
		printf("%f+%f=%f\n", HostInput1[i], HostInput2[i], HostOutput[i]);
	}

	//cleanup
	cleanup();
	return 0;
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
