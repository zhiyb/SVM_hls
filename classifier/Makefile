CFLAGS	+= -g -Wall -Werror
LDFLAGS	+= -lm
CC	:= g++

.PHONY:	all
all:	cordic_test classifier_test test_classification classifier.golden.out classifier.out

.PHONY:	hls
hls: svm_data.h classifier.golden.out
	find \( -name '*.cpp' -o -name '*.h' -o -name '*.golden.*' \) -exec bash -c "cat {} > ../hls/{}" \;

.DELETE_ON_ERROR:

cordic.h:	data.h
cordic.o:	cordic.h
cordic_test.o:	cordic.h
cordic_test:	cordic_test.o cordic.o
CLEAN_FILES	+= cordic.o cordic_test.o cordic_test

classifier.h:	data.h
classifier.o:	classifier.h svm_data.h cordic.h
classifier_test.o:	classifier.h alpha.h svm_data.h testData.h
classifier_test:	classifier_test.o classifier.o cordic.o
classifier_ref:	classifier_ref.o classifier.o cordic.o
CLEAN_FILES	+= classifier.o classifier_ref.o classifier_test.o classifier_test classifier_ref

dataconv.o:	data.h alpha.h SV.h
dataconv:	dataconv.o
CLEAN_FILES	+= dataconv.o dataconv

test_classification:	test_classification.o
CLEAN_FILES	+= test_classification.o test_classification

svm_data.h:	dataconv
	./$< > $@

classifier.golden.out:	classifier_ref
	./$< > $@

classifier.out:	classifier_test classifier.golden.out
	./$<

CLEAN_FILES	+= svm_data.h classifier.golden.out classifier.out

clean:
	$(RM) $(CLEAN_FILES)
