################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lexico/HashTable.cpp \
../lexico/Item.cpp \
../lexico/Lexicon.cpp \
../lexico/List.cpp \
../lexico/Token.cpp 

OBJS += \
./lexico/HashTable.o \
./lexico/Item.o \
./lexico/Lexicon.o \
./lexico/List.o \
./lexico/Token.o 

CPP_DEPS += \
./lexico/HashTable.d \
./lexico/Item.d \
./lexico/Lexicon.d \
./lexico/List.d \
./lexico/Token.d 


# Each subdirectory must supply rules for building sources it contributes
lexico/%.o: ../lexico/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


