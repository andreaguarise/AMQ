/*
 * AMQCommon.cpp
 *
 *  Created on: Dec 6, 2012
 *      Author: root
 */
#include <AMQCommon.h>

	int getIntFromStr(string strIn){
		locale loc;
		int iOut = -1;
		if ( isdigit(strIn[0],loc) )
		{
			stringstream(strIn) >> iOut;
		}
		return iOut;
	}


