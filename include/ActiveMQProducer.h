/*
 * ActiveMQProducer.h
 *
 *  Created on: Nov 21, 2012
 *      Author: root
 */

#ifndef ACTIVEMQPRODUCER_H_
#define ACTIVEMQPRODUCER_H_

#include <AMQCommon.h>

using namespace activemq::core;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace cms;
using namespace std;





namespace AMQReqRes {

class ActiveMQProducer: public ExceptionListener,
						public MessageListener,
						public Runnable {

private:

	string clientQueueName ;


	MessageProducer* producer;
    CountDownLatch latch;
    CountDownLatch doneLatch;
    Connection* connection;
    Session* session;
    MessageConsumer* consumer;
    long waitMillis;
    bool sessionTransacted;
    string brokerURI;
    int lastRecord;
    void cleanup();


	string createRandomString();
	void sendMessages();
	void setLastRecord(int iLast);
	int getLastRecord();

public:
	ActiveMQProducer( int numMessages,  bool sessionTransacted = false, int waitMillis  = 30000) :
			latch(1),
			doneLatch(numMessages),
			clientQueueName(QUEUE_NAME),
			sessionTransacted(sessionTransacted),
		    connection(NULL),
		    session(NULL),
		    consumer(NULL),
		    producer(NULL),
		    waitMillis(waitMillis),
		    brokerURI(BROKER_URI),
		    lastRecord(NULL_INTVAL)
		{}
	virtual ~ActiveMQProducer() throw();
	virtual void onMessage(const Message* message) throw();
	virtual void onException(const CMSException& ex AMQCPP_UNUSED);
	virtual void run();
	void waitUntilReady();
	void close();


};

} /* namespace AMQReqRes */
#endif /* ACTIVEMQPRODUCER_H_ */
