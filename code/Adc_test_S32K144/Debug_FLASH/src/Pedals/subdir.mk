################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Pedals/Pedals.c 

OBJS += \
./src/Pedals/Pedals.o 

C_DEPS += \
./src/Pedals/Pedals.d 


# Each subdirectory must supply rules for building sources it contributes
src/Pedals/%.o: ../src/Pedals/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Pedals/Pedals.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


