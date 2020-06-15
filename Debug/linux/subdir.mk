################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../linux/client.cpp \
../linux/connection.cpp \
../linux/jsonhandler.cpp \
../linux/main.cpp \
../linux/menus.cpp 

OBJS += \
./linux/client.o \
./linux/connection.o \
./linux/jsonhandler.o \
./linux/main.o \
./linux/menus.o 

CPP_DEPS += \
./linux/client.d \
./linux/connection.d \
./linux/jsonhandler.d \
./linux/main.d \
./linux/menus.d 


# Each subdirectory must supply rules for building sources it contributes
linux/%.o: ../linux/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


