# Helper macros to convert spaces into question marks and back again
e :=
sp := $(e) $(e)
qs = $(subst ?,$(sp),$1)
sq = $(subst $(sp),?,$1)

#board name
board_name := STM32F769I-DISCO
platform := cortex_m7

# Get name of this Makefile (avoid getting word 0 and a starting space)
makefile_name := $(wordlist 1,1000,$(MAKEFILE_LIST))

# Get path of this Makefile
makefile_path := $(call qs,$(dir $(call sq,$(abspath $(call sq,$(makefile_name))))))

# Get path where the Application is
#application_path := $(call qs,$(abspath $(call sq,$(makefile_path)..)))
application_path = $(makefile_path)

# Change makefile_name to a relative path
makefile_name := $(subst $(call sq,$(application_path))/,,$(call sq,$(abspath $(call sq,$(makefile_name)))))

# Get relative path to makefile from application_path
makefile_path_relative := $(subst $(call sq,$(application_path))/,,$(call sq,$(abspath $(call sq,$(makefile_path)))))

# Get path to Middlewares
#Middlewares_path := Middlewares

#Get BSP path
bsp_path := BSP/$(board_name)

# Get path to Drivers
Drivers_path := Drivers

# Get OS path
os_path := Middlewares/Third_Party/FreeRTOS

#Get user application path
user_app_path := App

#Get linker script path
linker_script_path := $(bsp_path)

# corrects TouchGFX Path
#touchgfx_path := ${subst ../,,$(touchgfx_path)}
#touchgfx_path := $(subst $(call sq,$(makefile_path))/,,$(call sq,$(abspath $(call sq,$(touchgfx_path)))))
touchgfx_path := TouchGFX
touchgfx_mk := $(touchgfx_path)/config/gcc/app.mk

#include application specific configuration
#include $(application_path)/TouchGFX/config/gcc/app.mk
include $(touchgfx_mk)

#Usb device
usb_lib_path := Middlewares/ST/STM32_USB_Device_Library
usb_device_path := USB_DEVICE

# Get identification of this system
ifeq ($(OS),Windows_NT)
UNAME := MINGW32_NT-6.2
else
UNAME := $(shell uname -s)
endif

.PHONY: all clean assets flash intflash

all: $(filter clean,$(MAKECMDGOALS))

all clean assets:
	@cd "$(application_path)" && "$(MAKE)" -r -f $(makefile_name) -s $(MFLAGS) _$@_

flash intflash: all
	@cd "$(application_path)" && "$(MAKE)" -r -f $(makefile_name) -s $(MFLAGS) _$@_

test:
	@echo ""; $(foreach d, $(c_source_files), echo $(d);)
	@echo ""; $(foreach d, $(cpp_source_files), echo $(d);)

# Directories containing application-specific source and header files.
# Additional components can be added to this list. make will look for
# source files recursively in comp_name/src and setup an include directive
# for comp_name/include.
components := $(touchgfx_path)/gui $(touchgfx_path)/target $(touchgfx_path)/generated/gui_generated

# Location of folder containing bmp/png files.
#asset_images_input  := TouchGFX/assets/images
asset_images_input := $(touchgfx_path)/assets/images

# Location of folder to search for ttf font files
#asset_fonts_input  := TouchGFX/assets/fonts
asset_fonts_input  := $(touchgfx_path)/assets/fonts

# Location of folder where the texts.xlsx is placed
#asset_texts_input  := TouchGFX/assets/texts
asset_texts_input  := $(touchgfx_path)/assets/texts

#build_root_path := TouchGFX/build
build_root_path := build
object_output_path := $(build_root_path)/$(board_name)
binary_output_path := $(build_root_path)/bin

# Location of output folders where autogenerated code from assets is placed
asset_root_path := $(touchgfx_path)/generated
asset_images_output := $(asset_root_path)/images
asset_fonts_output := $(asset_root_path)/fonts
asset_texts_output := $(asset_root_path)/texts


os_source_files := $(os_path)/Source/croutine.c \
                   $(os_path)/Source/list.c \
                   $(os_path)/Source/queue.c \
                   $(os_path)/Source/tasks.c \
                   $(os_path)/Source/timers.c \
                   $(os_path)/Source/portable/MemMang/heap_4.c \
                   $(os_path)/Source/portable/GCC/ARM_CM7/r0p1/port.c

os_include_paths := $(os_path)/Source/include \
                    $(os_path)/Source/portable/GCC/ARM_CM7/r0p1 \
									  $(os_path)/Source/CMSIS_RTOS

os_wrapper := $(touchgfx_path)/os/OSWrappers.cpp

### END OF USER SECTION. THE FOLLOWING SHOULD NOT BE MODIFIED ###

ifeq ($(UNAME), Linux)
imageconvert_executable := $(touchgfx_path)/framework/tools/imageconvert/build/linux/imageconvert.out
fontconvert_executable := $(touchgfx_path)/framework/tools/fontconvert/build/linux/fontconvert.out
else
imageconvert_executable := $(touchgfx_path)/framework/tools/imageconvert/build/win/imageconvert.out
fontconvert_executable := $(touchgfx_path)/framework/tools/fontconvert/build/win/fontconvert.out
st_link_executable := "$(PROGRAMFILES)\\STMicroelectronics\\STM32 ST-LINK Utility\\ST-LINK Utility\\ST-LINK_CLI.exe"
st_link_external_loader := "$(PROGRAMFILES)\\STMicroelectronics\\STM32 ST-LINK Utility\\ST-LINK Utility\\ExternalLoader\\MX25L512G_STM32F769I-DISCO.stldr"
endif

target_executable := target.elf
target_hex := target.hex

assembler         := arm-none-eabi-gcc
assembler_options += -g  \
                    -fno-exceptions\
                    $(no_libs) -mthumb -mno-thumb-interwork  \
                     -Wall
										 
assembler_options += $(float_options)

c_compiler         := arm-none-eabi-gcc
c_compiler_options += -g \
                    -mthumb -fno-exceptions \
                    -mno-thumb-interwork -std=c99 \
                    $(no_libs) \
                    -Os -fno-strict-aliasing -fdata-sections -ffunction-sections
#TODO removed -Wall

c_compiler_options += $(float_options)

cpp_compiler         := arm-none-eabi-g++
cpp_compiler_options += -g -mthumb \
                    $(no_libs) \
                    -mno-thumb-interwork -fno-rtti -fno-exceptions  \
                    -Os -fno-strict-aliasing -fdata-sections -ffunction-sections

#TODO removed -Wall

cpp_compiler_options += $(float_options)

linker         := arm-none-eabi-g++
linker_options += -g -Wl,-static -mthumb $(no_libs) -mno-thumb-interwork \
                  -fno-exceptions -specs=nosys.specs -fno-rtti \
                  -Os -fno-strict-aliasing -Wl,--gc-sections

objcopy := arm-none-eabi-objcopy

archiver := arm-none-eabi-ar

strip := arm-none-eabi-strip

# Additional toolchain configuration for Cortex-M7 targets.

float_options := -mfpu=fpv5-sp-d16 -mfloat-abi=softfp

assembler_options += -mthumb -mcpu=cortex-m7 -Wno-psabi $(float_options) -DCORE_M7 -D__irq=""
c_compiler_options += -mthumb -mcpu=cortex-m7 -Wno-psabi $(float_options) -DCORE_M7 -D__irq=""
cpp_compiler_options += -mthumb -mcpu=cortex-m7 -Wno-psabi $(float_options) -DCORE_M7 -D__irq=""
linker_options += -mcpu=cortex-m7 -Wno-psabi $(float_options)

#include everything + specific vendor folders
framework_includes := $(touchgfx_path)/framework/include

#this needs to change when assset include folder changes.
all_components := $(components) \
	$(asset_fonts_output) \
	$(asset_images_output) \
	$(asset_texts_output)

#keep framework include and source out of this mess! :)
include_paths := $(library_includes) #$(foreach comp, $(all_components), $(comp)/include) $(framework_includes) #$(source_Middlewares_paths)
source_paths = $(foreach comp, $(all_components), $(comp)/Src) #$(Drivers_path)/STM32F7xx_HAL_Driver/Src

# Finds files that matches the specified pattern. The directory list
# is searched recursively. It is safe to invoke this function with an
# empty list of directories.
#
# Param $(1): List of directories to search
# Param $(2): The file pattern to search for
define find
  $(foreach dir,$(1), $(foreach d, $(wildcard $(dir)/*), $(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))
endef
unexport find

fontconvert_ttf_lower_files := $(call find, $(asset_fonts_input), *.ttf)
fontconvert_ttf_upper_files := $(call find, $(asset_fonts_input), *.TTF)
fontconvert_otf_lower_files := $(call find, $(asset_fonts_input), *.otf)
fontconvert_otf_upper_files := $(call find, $(asset_fonts_input), *.OTF)
fontconvert_bdf_lower_files := $(call find, $(asset_fonts_input), *.bdf)
fontconvert_bdf_upper_files := $(call find, $(asset_fonts_input), *.BDF)

fontconvert_font_files := \
	$(fontconvert_ttf_lower_files) \
	$(fontconvert_ttf_upper_files) \
	$(fontconvert_otf_lower_files) \
	$(fontconvert_otf_upper_files) \
	$(fontconvert_bdf_lower_files) \
	$(fontconvert_bdf_upper_files)

asm_source_files := $(bsp_path)/startup/startup_stm32f769xx.s

gcc_source_files := \
	$(touchgfx_path)/framework/config/gcc/stdio.c \
  	$(touchgfx_path)/framework/config/gcc/stdlib.c \
  	$(touchgfx_path)/framework/config/gcc/string.c

board_c_files := \
	$(bsp_path)/Src/stm32f769i_discovery.c \
	$(bsp_path)/Src/stm32f769i_discovery_sdram.c \
	$(bsp_path)/Src/stm32f769i_discovery_ts.c \
	$(bsp_path)/Src/stm32f769i_discovery_qspi.c \
	$(bsp_path)/Components/otm8009a/otm8009a.c \
	$(bsp_path)/Components/ft6x06/ft6x06.c

board_c_files += \
	$(wildcard $(user_app_path)/Src/*.c) \
	$(os_path)/Source/CMSIS_RTOS/cmsis_os.c

board_c_files += \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd_ex.c \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usb.c \
	#$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c \
	#$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c \
	#$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c \
	#$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c \
	#$(Drivers_path)/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_nor.c \

board_c_files += \
	$(usb_lib_path)/Core/Src/usbd_ioreq.c \
	$(usb_lib_path)/Core/Src/usbd_ctlreq.c \
	$(usb_lib_path)/Core/Src/usbd_core.c \
	$(usb_lib_path)/Class/CDC/Src/usbd_cdc.c \
	$(usb_device_path)/App/usb_device.c \
	$(usb_device_path)/App/usbd_desc.c \
	$(usb_device_path)/App/usbd_cdc_if.c \
	$(usb_device_path)/Target/usbd_conf.c \
	
board_cpp_files := \
	$(wildcard $(user_app_path)/Src/*.cpp) \
	$(touchgfx_path)/target/STM32F7Instrumentation.cpp \
	$(touchgfx_path)/target/OTM8009TouchController.cpp \
	$(touchgfx_path)/target/STM32F7DMA.cpp \
	$(touchgfx_path)/target/STM32F7HAL_DSI.cpp \
	$(touchgfx_path)/target/GPIO.cpp \
	$(touchgfx_path)/target/HW_Init.cpp \
	$(touchgfx_path)/target/BoardConfiguration.cpp

board_include_paths := \
	$(touchgfx_path)/gui/include \
	$(touchgfx_path)/target \
	$(touchgfx_path)/platform/os \
	$(touchgfx_path)/generated/fonts/include \
	$(touchgfx_path)/generated/images/include \
	$(touchgfx_path)/generated/texts/include \
	$(touchgfx_path)/generated/gui_generated/include \
	$(touchgfx_path)/framework/include \
	$(Drivers_path)/STM32F7xx_HAL_Driver/Inc \
	$(Drivers_path)/CMSIS/Include \
	$(Drivers_path)/CMSIS/Device/ST/STM32f7xx/Include \
	$(bsp_path)/Src \
	$(bsp_path)/Components/otm8009a \
	$(bsp_path)/Components/qspi \
	$(user_app_path)/Inc \
	$(usb_lib_path)/Core/Inc \
	$(usb_lib_path)/Class/CDC/Inc \
	$(usb_device_path)/App \
	$(usb_device_path)/Target \

c_compiler_options += -DST -DSTM32F769xx
cpp_compiler_options += -DST -DSTM32F769xx

include_paths += $(board_include_paths) $(os_include_paths)

c_source_files := $(os_source_files) $(board_c_files) #$(call find, $(source_paths),*.c)
cpp_source_files := $(os_wrapper) $(board_cpp_files) $(call find, $(source_paths),*.cpp)

object_files := $(cpp_source_files) $(c_source_files)
# Start converting paths
object_files := $(object_files:$(touchgfx_path)/%.cpp=$(object_output_path)/touchgfx/%.o)
object_files := $(object_files:%.cpp=$(object_output_path)/%.o)
#object_files := $(object_files:$(Middlewares_path)/%.c=$(object_output_path)/Middlewares/%.o)
object_files := $(object_files:$(Drivers_path)/%.c=$(object_output_path)/Drivers/%.o)
object_files := $(object_files:%.c=$(object_output_path)/%.o)
dependency_files := $(object_files:%.o=%.d)

object_asm_files := $(asm_source_files:%.s=$(object_output_path)/%.o)
object_asm_files := $(patsubst $(object_output_path)/%,$(object_output_path)/%,$(object_asm_files))

textconvert_script_path := $(touchgfx_path)/framework/tools/textconvert
textconvert_executable := $(call find, $(textconvert_script_path), *.rb)

text_database := $(asset_texts_input)/texts.xlsx

libraries := touchgfx
library_include_paths := $(touchgfx_path)/lib/core/$(platform)/gcc

.PHONY: _all_ _clean_ _assets_ _flash_ _intflash_ generate_assets build_executable

# Force linking each time
.PHONY: $(binary_output_path)/$(target_executable)

_all_: generate_assets

ifeq ($(shell find "$(application_path)" -wholename "$(application_path)/$(binary_output_path)/extflash.bin" -size +0c | wc -l | xargs echo),1)
_flash_: _extflash_
else
_flash_: _intflash_
endif

_extflash_:
	@$(st_link_executable) -c -P $(binary_output_path)/target.hex 0x90000000 -Rst -EL $(st_link_external_loader)

_intflash_:
	@$(st_link_executable) -c -P $(binary_output_path)/intflash.hex 0x08000000 -Rst

generate_assets: _assets_
	@"$(MAKE)" -f $(makefile_name) -r -s $(MFLAGS) build_executable

build_executable: $(binary_output_path)/$(target_executable)

$(binary_output_path)/$(target_executable): $(object_files) $(object_asm_files)
#@echo ""; $(foreach d, $(object_files), echo $(d);)
	@echo Linking $(@)
	@mkdir -p $(@D)
	@mkdir -p $(object_output_path)
	@$(file >$(build_root_path)/objects.tmp) $(foreach F,$(object_files),$(file >>$(build_root_path)/objects.tmp,$F))
	@$(linker) \
		$(linker_options) -T $(linker_script_path)/STM32F769NIHx_FLASH.ld -Wl,-Map=$(@D)/application.map $(linker_options_local) \
		$(patsubst %,-L%,$(library_include_paths)) \
		@$(build_root_path)/objects.tmp $(object_asm_files) -o $@ \
		-Wl,--start-group $(patsubst %,-l%,$(libraries)) -Wl,--end-group
	@rm -f $(build_root_path)/objects.tmp
	@echo "Producing additional output formats..."
	@echo "  target.hex   - Combined internal+external hex"
	@$(objcopy) -O ihex $@ $(@D)/target.hex
	@echo "  intflash.elf - Internal flash, elf debug"
	@$(objcopy) --remove-section=ExtFlashSection $@ $(@D)/intflash.elf 2>/dev/null
	@echo "  intflash.hex - Internal flash, hex"
	@$(objcopy) -O ihex --remove-section=ExtFlashSection $@ $(@D)/intflash.hex
	@echo "  extflash.bin - External flash, binary"
	@$(objcopy) -O binary --only-section=ExtFlashSection $@ $(@D)/extflash.bin

$(object_output_path)/touchgfx/%.o: $(touchgfx_path)/%.cpp $(touchgfx_mk)
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(cpp_compiler) \
		-MMD -MP $(cpp_compiler_options) $(cpp_compiler_options_local) $(user_cflags) \
		$(patsubst %,-I%,$(include_paths)) \
		-c $< -o $@

$(object_output_path)/%.o: %.cpp $(touchgfx_mk)
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(cpp_compiler) \
		-MMD -MP $(cpp_compiler_options) $(cpp_compiler_options_local) $(user_cflags) \
		$(patsubst %,-I%,$(include_paths)) \
		-c $< -o $@

$(object_output_path)/touchgfx/%.o: $(touchgfx_path)/%.c $(touchgfx_mk)
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(c_compiler) \
		-MMD -MP $(c_compiler_options) $(c_compiler_options_local) $(user_cflags) \
		$(patsubst %,-I%,$(include_paths)) \
		-c $< -o $@

$(object_output_path)/%.o: %.c $(touchgfx_mk)
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(c_compiler) \
		-MMD -MP $(c_compiler_options) $(c_compiler_options_local) $(user_cflags) \
		$(patsubst %,-I%,$(include_paths)) \
		-c $< -o $@

$(object_output_path)/%.o: %.s $(touchgfx_mk)
	@echo Compiling ASM $<
	@mkdir -p $(@D)
	@$(assembler) \
		$(assembler_options) \
		$(patsubst %,-I %,$(os_include_paths)) \
		-c $< -o $@

ifeq ($(MAKECMDGOALS),build_executable)
$(firstword $(dependency_files)): $(touchgfx_mk)
	@rm -rf $(object_output_path)
-include $(dependency_files)
endif

_assets_: BitmapDatabase $(asset_texts_output)/include/texts/TextKeysAndLanguages.hpp

alpha_dither ?= no
dither_algorith ?= 2
remap_identical_texts ?= yes

.PHONY: BitmapDatabase
BitmapDatabase:
	@echo Converting images
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) $(screen_orientation) -r $(asset_images_input) -w $(asset_images_output)
	
$(asset_texts_output)/include/texts/TextKeysAndLanguages.hpp: $(text_database) $(touchgfx_mk) $(textconvert_executable) $(fontconvert_executable) $(fontconvert_font_files)
	@rm -f $(asset_fonts_output)/src/*
	@rm -f $(asset_fonts_output)/include/fonts/*
	@rm -f $(asset_fonts_output)/UnicodeList*.txt
	@rm -f $(asset_fonts_output)/CharSizes*.csv
	@mkdir -p $(asset_texts_output)/include/texts
	@ruby $(textconvert_script_path)/main.rb $(text_database) $(fontconvert_executable) $(asset_fonts_output) $(asset_texts_output) $(asset_fonts_input) . $(remap_identical_texts) $(text_data_format)

_clean_:
	@echo Cleaning
	@rm -rf $(build_root_path)
	# Do not remove gui_generated
	@rm -rf $(asset_images_output)
	@rm -rf $(asset_fonts_output)
	@rm -rf $(asset_texts_output)
	# Create directory to avoid error if it does not exist
	@mkdir -p $(asset_root_path)
	# Remove assets folder if it is empty (i.e. no gui_generated folder)
	@rmdir --ignore-fail-on-non-empty $(asset_root_path)
