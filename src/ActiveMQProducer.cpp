/*
 * ActiveMQConsumer.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: root
 */

#include "ActiveMQProducer.h"
using namespace AMQReqRes;

ActiveMQProducer::~ActiveMQProducer() throw () {
	this->cleanup();
}
void ActiveMQProducer::run() {

	try {
		// Create a ConnectionFactory
		auto_ptr<ConnectionFactory> connectionFactory(
				ConnectionFactory::createCMSConnectionFactory(BROKER_URI));

		// Create a Connection
		connection = connectionFactory->createConnection();
		connection->start();
		connection->setExceptionListener(this);

		// Create a Session e' duplicato, ma per ora uso ESATTAMENTE l'esempio del libro
		if (this->sessionTransacted == true) {
			session = connection->createSession(Session::SESSION_TRANSACTED);
		} else {
			session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
		}

		Destination* adminQueue = session->createQueue(clientQueueName);

		//Setup a message producer to send message to the queue the server is consuming from

		this->producer = session->createProducer(adminQueue);
		this->producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

		//Create a temporary queue that this client will listen for responses on then create a consumer
		//that consumes message from this temporary queue...for a real application a client should reuse
		//the same temp queue for each message to the server...one temp queue per client

		Destination * tmpDest = session->createTemporaryQueue();
		MessageConsumer * responseConsumer = session->createConsumer(tmpDest);

		//This class will handle the messages to the temp queue as well
//coda temporanea
		responseConsumer->setMessageListener(this);


		//Now create the actual message you want to send

		TextMessage* txtMessage = session->createTextMessage();
		txtMessage->setText(GET_STATUS_MSG);

		//Set the reply to field to the temp queue you created above, this is the queue the server
		//will respond to

		txtMessage->setCMSReplyTo(tmpDest);
		//Set a correlation ID so when you get a response you know which sent message the response is for
		//If there is never more than one outstanding message to the server then the
		//same correlation ID can be used for all the messages...if there is more than one outstanding
		//message to the server you would presumably want to associate the correlation ID with this
		//message somehow...a Map works good
		string correlationId = this->createRandomString();
		txtMessage->setCMSCorrelationID(correlationId);
		this->producer->send(txtMessage);
		// Indicate we are ready for messages.
		latch.countDown();

		// Wait while asynchronous messages come in.
		doneLatch.await(waitMillis);



	} catch (CMSException& e) {
		this->cleanup();
		e.printStackTrace();
	}

}
// create a string with a random hex
string ActiveMQProducer::createRandomString() {

	stringstream stream;
	srand(time(0));
	int irand = rand();
	stream << hex << irand;
	return stream.str();

}
// The message srrived is the number of last record.
void ActiveMQProducer::onMessage(const Message* message) throw () {
	string text = "";
	try {
		const TextMessage* textMessage =
				dynamic_cast<const TextMessage*>(message);
		//the message arrived is the last registered record
		if (textMessage != NULL) {
			text = textMessage->getText();
			int iLastRecord = getIntFromStr(text);
			setLastRecord(iLastRecord);
			printf("Message Received: %i\n",iLastRecord);
		} else {
			throw new CMSException("Last Record needed to start");
		}
		sendMessages();
		printf("Message Received: %s\n", text.c_str());
	} catch (CMSException& e) {
		this->cleanup();
		e.printStackTrace();
	}

}
void ActiveMQProducer::close() {
     this->cleanup();
 }

void ActiveMQProducer::waitUntilReady() {
      latch.await();
  }

void ActiveMQProducer::sendMessages(){
for (int ix = 0; ix < getLastRecord(); ix++) {
     std::auto_ptr<TextMessage> message(session->createTextMessage(MESSAGE_SENT_BY_PRODUCER));
     message->setIntProperty("Integer", ix);
      producer->send(message.get());
 }
}


// If something bad happens you see it here as this class is also been
// registered as an ExceptionListener with the connection.
void ActiveMQProducer::onException(const CMSException& ex AMQCPP_UNUSED) {
	printf("CMS Exception occurred.  Shutting down client.\n");
	ex.printStackTrace();
	exit(1);
}

void ActiveMQProducer::setLastRecord(int iLast){
	this->lastRecord = iLast;
}

int ActiveMQProducer::getLastRecord(){
	return this->lastRecord;
}
void ActiveMQProducer::cleanup() {
     if (connection != NULL) {
         try {
             connection->close();
         } catch (cms::CMSException& ex) {
             ex.printStackTrace();
         }
     }
     // Destroy resources.
     try {
         delete consumer;
         consumer = NULL;
         delete producer;
         producer = NULL;
         delete session;
         session = NULL;
         delete connection;
         connection = NULL;
     } catch (CMSException& e) {
         e.printStackTrace();
     }
 }


