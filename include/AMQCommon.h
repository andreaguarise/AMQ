/*
 * AMQCommon.h
 *
 *  Created on: Nov 21, 2012
 *      Author: root
 */

#ifndef AMQCOMMON_H_
#define AMQCOMMON_H_

#include <activemq/library/ActiveMQCPP.h>
#include <activemq/util/Config.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Session.h>
#include <cms/Connection.h>
#include <cms/Destination.h>
#include <cms/DeliveryMode.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <string>
#include <time.h>

static const char* GET_STATUS_MSG = "get status";
static const char* MESSAGE_SENT_BY_PRODUCER = "Ecco il messaggio";
static const char* BROKER_URI = "failover:(tcp://dgas-dev-28.to.infn.it:61616"")";
static const char* QUEUE_NAME = "client.messages";
static const int NULL_INTVAL = -1;

using namespace std;
int getIntFromStr(string str);

#endif /* AMQCOMMON_H_ */
