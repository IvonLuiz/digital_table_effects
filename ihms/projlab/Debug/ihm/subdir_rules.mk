################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ihm/%.obj: ../ihm/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"" -v5502 --memory_model=large -g --include_path="C:/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/Users/palom/Projetos/digital_table_effects/ihms/projlab" --include_path="/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="ihm/$(basename $(<F)).d_raw" --obj_directory="ihm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


