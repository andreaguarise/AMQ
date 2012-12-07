################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AMQCommon.cpp \
../src/ActiveMQConsumer.cpp \
../src/ActiveMQProducer.cpp \
../src/main.cpp 

OBJS += \
./src/AMQCommon.o \
./src/ActiveMQConsumer.o \
./src/ActiveMQProducer.o \
./src/main.o 

CPP_DEPS += \
./src/AMQCommon.d \
./src/ActiveMQConsumer.d \
./src/ActiveMQProducer.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/activemq-cpp-3.4.0 -I"/mnt/persist/laura/eclipseworkspace/AMQ/include" -O3 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


