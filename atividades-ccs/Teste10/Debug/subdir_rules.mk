################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
aic3204.obj: ../aic3204.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/Heverton Reis/Documents/LAB03/Teste10" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="aic3204.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd.obj: ../lcd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/Heverton Reis/Documents/LAB03/Teste10" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="lcd.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

led.obj: ../led.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/Heverton Reis/Documents/LAB03/Teste10" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="led.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

oled.obj: ../oled.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/Heverton Reis/Documents/LAB03/Teste10" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="oled.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

principal.obj: ../principal.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/Heverton Reis/Documents/LAB03/Teste10" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --include_path="C:/ti/ezdsp5502_v1/include" --include_path="C:/ti/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="principal.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


