#include <stdio.h>
#include <xparameters.h> // Parameter definitions for processor periperals
#include <xscugic.h>     // Processor interrupt controller device driver
#include <xclassifier.h>
#include <xtime_l.h>
#include "data.h"
#include "testData.h"

#define ASIZE(a)	(sizeof(a) / sizeof((a)[0]))

static data_t testDataI[ASIZE(testData) / 16][16];

// HLS HW instance
XClassifier cls;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int Run = 0;
volatile static int ResultAvail = 0;

int setup_interrupt();

static int test_tanh()
{
	XTime itvl, ticks[3];
	return 0;
}

struct test_t {
	void (*pre)();
	int (*test)();
	const char *name;
} tests[] = {
	{0, test_tanh, "double precision tanh()"},
	{0, 0, 0},
};

void cls_isr(void *InstancePtr)
{
}

int main()
{
	print("\r\nBuild @ " __DATE__ " " __TIME__ "\r\n");

	XTime itvl;
	int status;

	status = XClassifier_Initialize(&cls, XPAR_CLASSIFIER_0_DEVICE_ID);
	if (status != XST_SUCCESS) {
		print("HLS peripheral setup failed\r\n");
		return XST_FAILURE;
	}

	status = setup_interrupt();
	if (status != XST_SUCCESS) {
		print("Interrupt setup failed\r\n");
		return XST_FAILURE;
	}

	if (XClassifier_IsReady(&cls))
		print("HLS peripheral is ready\r\n");
	else {
		print("HLS peripheral is *NOT* ready\r\n");
		return XST_FAILURE;
	}

	print("Converting data...\r\n");
	XTime_SetTime(0);
	double *pf = &testData[0];
	data_t *p = &testDataI[0][0];
	for (size_t i = ASIZE(testData); i != 0; i--)
		*p++ = FtoI(*pf++);
	XTime_GetTime(&itvl);
	printf("Conversion finished, %llu ticks\r\n", itvl);

	struct test_t *pt = &tests[0];
	while (pt->test) {
		printf("\r\nExecuting test <%s>\r\n", pt->name);
		if (pt->pre)
			pt->pre();

		XTime_SetTime(0);
		pt->test();
		XTime_GetTime(&itvl);
		printf("Finished <%s>, %llu ticks\r\n", pt->name, itvl);
		pt++;
	}

	return 0;
}

int setup_interrupt()
{
	int status;

	// Setup the interrupt on the ARM
	XScuGic_Config *pCfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (pCfg == NULL) {
		print("Interrupt Configuration Lookup Failed\r\n");
		return XST_FAILURE;
	}

	status = XScuGic_CfgInitialize(&ScuGic, pCfg, pCfg->CpuBaseAddress);
	if (status != XST_SUCCESS)
		return status;

	// self test
	status = XScuGic_SelfTest(&ScuGic);
	if (status != XST_SUCCESS)
		return status;

	// Initialise the exception handler
	Xil_ExceptionInit();

	// Register the exception handler
	//print("Register the exception handler\n\r");
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler, &ScuGic);

	//Enable the exception handler
	Xil_ExceptionEnable();

	// Connect the Adder ISR to the exception table
	//print("Connect the Adder ISR to the Exception handler table\n\r");
	status = XScuGic_Connect(&ScuGic, XPAR_FABRIC_CLASSIFIER_0_INTERRUPT_INTR,
			(Xil_InterruptHandler)cls_isr, &cls);
	if (status != XST_SUCCESS)
		return status;

	//print("Enable the Adder ISR\n\r");
	XScuGic_Enable(&ScuGic, XPAR_FABRIC_CLASSIFIER_0_INTERRUPT_INTR);
	return XST_SUCCESS;
}
