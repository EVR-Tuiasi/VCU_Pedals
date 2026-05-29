################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Communications/CanMessaging.c \
../src/Communications/UartMessaging.c 

OBJS += \
./src/Communications/CanMessaging.o \
./src/Communications/UartMessaging.o 

C_DEPS += \
./src/Communications/CanMessaging.d \
./src/Communications/UartMessaging.d 


# Each subdirectory must supply rules for building sources it contributes
src/Communications/%.o: ../src/Communications/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Communications/CanMessaging.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


