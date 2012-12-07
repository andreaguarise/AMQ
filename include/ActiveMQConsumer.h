/*
 * ActiveMQConsumer.h
 *
 *  Created on: Nov 22, 2012
 *      Author: root
 */

#ifndef ACTIVEMQCONSUMER_H_
#define ACTIVEMQCONSUMER_H_

#include <AMQCommon.h>

using namespace cms;
using namespace std;
using namespace activemq::core;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;




namespace AMQReqRes  {

class ActiveMQConsumer : public ExceptionListener,
							public MessageListener,
							public Runnable {

private:
		CountDownLatch latch;
	    CountDownLatch doneLatch;
	    Connection* connection;
	    Session* session;
	    MessageConsumer* consumer;
	    long waitMillis;
	    bool sessionTransacted;
	    string brokerURI;
	    string messageQueueName;

		string handleProtocolMessage(string messageText);
		void replyToProducer(const Message* message,string replyMessage);
		void cleanup();


	public:

	ActiveMQConsumer();
	ActiveMQConsumer(int numMessages, bool sessionTransacted = false, int waitMillis  = 600000)
    : latch(1),
    doneLatch(numMessages),
    connection(NULL),
    session(NULL),
    consumer(NULL),
    waitMillis(waitMillis),
    sessionTransacted(sessionTransacted),
    messageQueueName(QUEUE_NAME),
    brokerURI(BROKER_URI){}

	virtual ~ActiveMQConsumer() throw();
	virtual void onMessage(const Message* message) throw();
	virtual void onException(const CMSException& ex AMQCPP_UNUSED);
	void run();
	void waitUntilReady();
	void close();

};

} /* namespace AMQReqRes */
#endif /* ACTIVEMQSERVERSIDE_H_ */
