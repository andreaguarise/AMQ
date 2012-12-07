/*
 * ActiveMQConsumer.cpp
 *
 *  Created on: Nov 22, 2012
 *      Author: root
 */

#include "ActiveMQConsumer.h"

using namespace AMQReqRes;

ActiveMQConsumer::~ActiveMQConsumer() throw () {
	this->cleanup();
	// TODO Auto-generated destructor stub
}
void ActiveMQConsumer::waitUntilReady() {
      latch.await();
  }

void ActiveMQConsumer::close() {
     this->cleanup();
 }

string ActiveMQConsumer::handleProtocolMessage(string messageText) {
	string responseText = "";
	if (messageText.compare(GET_STATUS_MSG) == 0) {
		responseText = "10";
	}
	return responseText;
}

void ActiveMQConsumer::run() {
	Connection* connection;

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
			this->session = connection->createSession(
					Session::SESSION_TRANSACTED);
		} else {
			this->session = connection->createSession(
					Session::AUTO_ACKNOWLEDGE);
		}
		Destination* adminQueue = this->session->createQueue(messageQueueName);

		//Set up a consumer to consume messages off of the admin queue
		MessageConsumer* consumer = this->session->createConsumer(adminQueue);
		consumer->setMessageListener(this);

        // Indicate we are ready for messages.
         latch.countDown();

         // Wait while asynchronous messages come in.
         doneLatch.await(waitMillis);


	} catch (CMSException& e) {
		e.printStackTrace();
	}
}

void ActiveMQConsumer::onMessage(const Message* message) throw () {
	try {
		const TextMessage* txtMsg = dynamic_cast<const TextMessage*>(message);
		if (txtMsg != NULL) {
			string messageText = txtMsg->getText();
			string replyMessage = this->handleProtocolMessage(messageText);
			if (!replyMessage.empty()) {
				replyToProducer(message, replyMessage);
			} else {
				cout << "Message received: " + messageText +"\n";
			}
		}

	} catch (CMSException& e) {
		e.printStackTrace();
	}
}

//Setup a message producer to respond to messages from clients, we will get the destination
//to send to from the JMSReplyTo header field from a Message

void ActiveMQConsumer::replyToProducer(const Message* message, string replyMessage) {
	MessageProducer* replyProducer;
	replyProducer = this->session->createProducer();
	replyProducer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
	TextMessage* response = this->session->createTextMessage();
	response->setText(replyMessage);
	//uses the correlationid to identify the tmp queue
	response->setCMSCorrelationID(message->getCMSCorrelationID());
	replyProducer->send(message->getCMSReplyTo(), response);
}

// If something bad happens you see it here as this class is also been
// registered as an ExceptionListener with the connection.
void ActiveMQConsumer::onException(const CMSException& ex AMQCPP_UNUSED) {
	printf("CMS Exception occurred.  Shutting down client.\n");
	ex.printStackTrace();
	exit(1);
}

void ActiveMQConsumer::cleanup() {
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
         delete session;
         session = NULL;
         delete connection;
         connection = NULL;
     } catch (CMSException& e) {
         e.printStackTrace();
     }
 }

