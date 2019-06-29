################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sintatico/AST.cpp \
../sintatico/Parser.cpp 

OBJS += \
./sintatico/AST.o \
./sintatico/Parser.o 

CPP_DEPS += \
./sintatico/AST.d \
./sintatico/Parser.d 


# Each subdirectory must supply rules for building sources it contributes
sintatico/%.o: ../sintatico/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


