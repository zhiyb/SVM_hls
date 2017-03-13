#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "testData.h"
#include "SV.h"
#include "alpha.h"
int main(){
	FILE *data;
	FILE *label;
	FILE *sv;
	FILE *svm_alpha;
	FILE *data_h;
	FILE *label_h;
	FILE *sv_h;
	// FILE *svm_alpha_h;
	uint32_t i = 0;
	uint16_t j = 0;

	data = fopen("formattedData.cpp", "w");
	label = fopen("refout.cpp", "w");
	sv = fopen("new_sv.cpp", "w");
	svm_alpha = fopen("alpha.cpp", "w");
	data_h = fopen("formattedData.h", "w");
	label_h = fopen("refout.h", "w");
	sv_h = fopen("new_sv.h", "w");
	// svm_alpha = fopen("alpha.h", "w");

	uint32_t data_size = sizeof(testData)/sizeof(testData[0])/16;
	// uint32_t label_size = sizeof(testSataLabel)/sizeof(testDataLabel[0]);
	uint32_t sv_size = sizeof(SVs)/sizeof(SVs[0])/16;
	
	fprintf(data, "#include \"formattedData.h\"\n\n");
	fprintf(label, "#include \"refout.h\"\n\n");
	fprintf(sv, "#include \"new_sv.h\"\n\n");
	fprintf(svm_alpha, "#include \"alpha.h\"\n\n");
	
	fprintf(data_h, "#ifndef FORMATTEDDATA_H_\n#define FORMATTEDDATA_H_\n\n");
	fprintf(label_h, "#ifndef REFOUT_H_\n#define REFOUT_H_\n\n");
	fprintf(sv_h, "#ifndef NEW_SV_H_\n#define NEW_SV_H_\n\n");
	// fprintf(svm_alpha_h, "#ifndef ALPHA_H_\n#define ALPHA_H_\n");

	fprintf(data_h, "extern const int testData[%d][16];\n\n#endif // FORMATTEDDATA_H_\n", data_size);
	fprintf(label_h, "extern const int refout[%d];\n\n#endif // REFOUT_H_\n", data_size);
	fprintf(sv_h, "extern const int sv_x[%d][16];\n\n#endif // NEW_SV_H_\n", sv_size);
	// fprintf(svm_alpha_h, "#ifndef ALPHA_H_\n#define ALPHA_H_\n");

	fclose(data_h);
	fclose(label_h);
	fclose(sv_h);
	
	fprintf(data, "const int testData[%d][16] = {", data_size);
	fprintf(label, "const int refout[%d] = {", data_size);
	fprintf(sv, "const int sv_x[%d][16] = {", sv_size);
	fprintf(svm_alpha, "const ap_fixed_type bias = %f; \n\n", bias);
	fprintf(svm_alpha, "const ap_fixed_type sv_alpha[%d] = {", sv_size);
	for(i = 0; i < data_size; i++)
	{
		for(j = 0; j < 16; j++)
			if(j == 0)
				fprintf(data, "{%d,\n", (int)round(testData[i * 16 + j] * 4096));
			else if(j == 15 && i != data_size - 1)
				fprintf(data, "%d},\n", (int)round(testData[i * 16 + j] * 4096));
			else if(j == 15 && i == data_size - 1)
				fprintf(data, "%d}};\n", (int)round(testData[i * 16 + j] * 4096));
			else
				fprintf(data, "%d,\n", (int)round(testData[i * 16 + j] * 4096));
		if(i != data_size - 1)
			fprintf(label, "%d,\n", testDataLabel[i]);
		else
			fprintf(label, "%d};\n", testDataLabel[i]);
	}
	// fprintf(label, "};");
	// fprintf(data, "};");
	fclose(data);
	fclose(label);
	for(i = 0; i < sv_size; i++)
	{
		for(j = 0; j < 16; j++)
			if(j == 0)
				fprintf(sv, "{%d,\n", (int)round(SVs[i * 16 + j] * 4096));
			else if(j == 15)
				fprintf(sv, "%d},\n", (int)round(SVs[i * 16 + j] * 4096));
			else
				fprintf(sv, "%d,\n", (int)round(SVs[i * 16 + j] * 4096));
		if(i != sv_size - 1)
			fprintf(svm_alpha, "%f,\n", alpha[i]);
		else
			fprintf(svm_alpha, "%f};\n", alpha[i]);
	}
	fprintf(sv, "};");
	fclose(sv);
	fclose(svm_alpha);

	return 0;
		
}
