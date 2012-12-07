/*
 * main.cpp
 *
 *  Created on: Oct 17, 2012
 *      Author: root
 */

#include "ActiveMQConsumer.h"
#include "ActiveMQProducer.h"

using namespace std;
using namespace decaf::lang;

using namespace AMQReqRes;

int main(int argc, char* argv[] ){
	    activemq::library::ActiveMQCPP::initializeLibrary();
	     {
	     std::cout << "=====================================================\n";
	     std::cout << "Starting the example:" << std::endl;
	     std::cout << "-----------------------------------------------------\n";


	     int numMessages = -1;
	     string text;

	     if(argc == 3){
	    	 numMessages = atoi(argv[1]);
	    	 text = argv[2];
	     }else{
	    	 cout << "No input messages, using 10 and 'Hello world' as default values " << endl;
	    	 numMessages = 10;
	    	 text = "Hello World!!! ";
	     }
	     ActiveMQProducer producer(2);
	     ActiveMQConsumer consumer(numMessages);
	     // Start the consumer thread.
	     Thread consumerThread(&consumer);
	     consumerThread.start();


	     // Wait for the consumer to indicate that its ready to go.
	     consumer.waitUntilReady();

	     // Start the producer thread.
	     Thread producerThread(&producer);
	     producerThread.start();

	     producer.waitUntilReady();

	     // Wait for the threads to complete.
	     producerThread.join();
	     consumerThread.join();

	  //   long long endTime = System::currentTimeMillis();
	 //    double totalTime = (double)(endTime - startTime) / 1000.0;

	     consumer.close();
	     producer.close();

	  //   std::cout << "Time to completion = " << totalTime << " seconds." << std::endl;
	     std::cout << "-----------------------------------------------------\n";
	     std::cout << "Finished with the example." << std::endl;
	     std::cout << "=====================================================\n";

	     }
	     activemq::library::ActiveMQCPP::shutdownLibrary();



}



