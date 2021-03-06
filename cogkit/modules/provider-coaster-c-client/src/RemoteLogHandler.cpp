/*
 * Swift Parallel Scripting Language (http://swift-lang.org)
 *
 * Copyright 2014 University of Chicago
 *	
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*
 * RemoteLogHandler.cpp
 *
 *	Created on: Aug 28, 2014
 *			Author: Tim Armstrong
 */

#include "RemoteLogHandler.h"

#include <sstream>

using namespace Coaster;

using std::string;
using std::stringstream;

RemoteLogHandler::RemoteLogHandler() {
}

RemoteLogHandler::~RemoteLogHandler() {
}

void RemoteLogHandler::requestReceived() {
	string logType, msg;
	getInDataAsString(0, logType);
	getInDataAsString(1, msg);

	if (logType == "INFO") {
		LogInfo << msg << endl; 
	} else if (logType == "WARN") {
		LogWarn << msg << endl; 
	} else if (logType == "STDOUT") {
		std::cout << msg << std::endl; 
	} else if (logType == "STDERR") {
		std::cerr << msg << std::endl; 
	} else if (logType == "DEBUG") {
		LogDebug << msg << endl; 
	} else if (logType == "ERROR") {
		LogError << msg << endl; 
	} else if (logType == "FATAL") {
		LogFatal << msg << endl; 
	} else {
		LogWarn << "Unexpected RemoteLog type: " << logType <<
			" msg: " << msg << endl;
	}
	sendReply("OK");
}
