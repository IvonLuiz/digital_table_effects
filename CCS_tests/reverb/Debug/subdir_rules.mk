################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
reverbTest.obj: ../reverbTest.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/darth/workspace_v7/reverb" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="reverbTest.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

reverberation.obj: ../reverberation.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/darth/workspace_v7/reverb" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="reverberation.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


