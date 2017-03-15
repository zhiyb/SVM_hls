#include <stdio.h>
#include <xparameters.h> // Parameter definitions for processor periperals
#include <xscugic.h>     // Processor interrupt controller device driver
#include <xclassifier.h>
#include <xtime_l.h>
#include "data.h"
#include "test.h"
#include "testData.h"
#include "alpha.h"
#include "SV.h"

// HLS HW instance
XClassifier cls;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int Run = 0;
volatile static int ResultAvail = 0;

data_t testDataI[ASIZE(testData) / N][N];
data_t SVsI[ASIZE(SVs) / N][N];
data_t alphaI[ASIZE(SVsI)];
data_t biasI;

struct test_t {
	void (*pre)();
	unsigned int (*test)();
	const char *name;
} tests[] = {
	{0, test_cls_double, "double precision classifier"},
	{0, test_cls_fixed, "fixed point classifier"},
	{0, 0, 0},
};

int setup_interrupt();

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
	data_t *pi = &testDataI[0][0];
	for (size_t i = ASIZE(testData); i != 0; i--)
		*pi++ = FtoI(*pf++);
	pf = &SVs[0];
	pi = &SVsI[0][0];
	for (size_t i = ASIZE(SVs); i != 0; i--)
		*pi++ = FtoI(*pf++);
	pf = &alpha[0];
	pi = &alphaI[0];
	for (size_t i = ASIZE(alpha); i != 0; i--)
		*pi++ = FtoI(*pf++);
	biasI = FtoI(bias);
	XTime_GetTime(&itvl);
	printf("Conversion finished, %llu ticks\r\n", itvl);

	struct test_t *pt = &tests[0];
	while (pt->test) {
		printf("<%s> starting...\r\n", pt->name);
		if (pt->pre)
			pt->pre();

		XTime_SetTime(0);
		unsigned int err = pt->test();
		XTime_GetTime(&itvl);
		printf("<%s> finished, %llu ticks, error count %u\r\n", pt->name, itvl, err);
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
