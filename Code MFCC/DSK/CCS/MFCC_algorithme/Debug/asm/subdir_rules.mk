################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
asm/addition.obj: ../asm/addition.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6700/dsplib/include" --include_path="C:/Users/Guill/Documents/GitHub/P1-S5/Code MFCC/DSK/CCS/MFCC_algorithme/header" --include_path="C:/ti/dsk6713/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="asm/addition.d" --obj_directory="asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

asm/divide.obj: ../asm/divide.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6700/dsplib/include" --include_path="C:/Users/Guill/Documents/GitHub/P1-S5/Code MFCC/DSK/CCS/MFCC_algorithme/header" --include_path="C:/ti/dsk6713/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="asm/divide.d" --obj_directory="asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

asm/encrypt.obj: ../asm/encrypt.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6700/dsplib/include" --include_path="C:/Users/Guill/Documents/GitHub/P1-S5/Code MFCC/DSK/CCS/MFCC_algorithme/header" --include_path="C:/ti/dsk6713/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="asm/encrypt.d" --obj_directory="asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

asm/multiply.obj: ../asm/multiply.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6700/dsplib/include" --include_path="C:/Users/Guill/Documents/GitHub/P1-S5/Code MFCC/DSK/CCS/MFCC_algorithme/header" --include_path="C:/ti/dsk6713/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="asm/multiply.d" --obj_directory="asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

asm/substraction.obj: ../asm/substraction.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6700/dsplib/include" --include_path="C:/Users/Guill/Documents/GitHub/P1-S5/Code MFCC/DSK/CCS/MFCC_algorithme/header" --include_path="C:/ti/dsk6713/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="asm/substraction.d" --obj_directory="asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


