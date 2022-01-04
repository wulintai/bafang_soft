################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/func/CARMC_IO.obj: ../src/func/CARMC_IO.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml -mt --include_path="D:/pmsm/PMSM/src/common" --include_path="D:/pmsm/PMSM/src/func" --include_path="D:/ti/ccsv5/tools/compiler/c2000_6.2.0/include" --include_path="D:/pmsm/PMSM/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/func/CARMC_IO.pp" --obj_directory="src/func" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/func/CARMC_PWM.obj: ../src/func/CARMC_PWM.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml -mt --include_path="D:/pmsm/PMSM/src/common" --include_path="D:/pmsm/PMSM/src/func" --include_path="D:/ti/ccsv5/tools/compiler/c2000_6.2.0/include" --include_path="D:/pmsm/PMSM/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/func/CARMC_PWM.pp" --obj_directory="src/func" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


