################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/app_comm.c \
../source/app_msg.c \
../source/convert_types_var.c \
../source/general.c \
../source/main_ctrl.c 

OBJS += \
./source/app_comm.o \
./source/app_msg.o \
./source/convert_types_var.o \
./source/general.o \
./source/main_ctrl.o 

C_DEPS += \
./source/app_comm.d \
./source/app_msg.d \
./source/convert_types_var.d \
./source/general.d \
./source/main_ctrl.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/intelbras/Projetos/github/marcovargas74/eclipse/example_ctrl/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


