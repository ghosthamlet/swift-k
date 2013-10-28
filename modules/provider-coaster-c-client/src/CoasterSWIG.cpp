#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

#include <CoasterSWIG.h>
#include "CoasterLoop.h"
#include "CoasterClient.h"
#include "Settings.h"
#include "Job.h"
using namespace std;

/** CoasterSWIGLoopCreate : create, starts and returns
 *  a pointer to a CoasterLoop object.
 */
CoasterLoop* CoasterSWIGLoopCreate(void)
{
    cout << "CoasterSWIGLoopCreate()..." << endl;
    CoasterLoop *loop = new CoasterLoop();
    loop->start();
    return loop;
}

/* Destroys the loop
 */
int CoasterSWIGLoopDestroy(CoasterLoop* loop)
{
    delete(loop);
    return 0;
}

CoasterClient* CoasterSWIGClientCreate(CoasterLoop *loop, char* serviceURL)
{
    cout << "CoasterSWIGClientCreate(" << serviceURL << ")..." << endl;
    CoasterClient* client = new CoasterClient(serviceURL, *loop);
    client->start();
    return client;
}

int CoasterSWIGClientDestroy(CoasterClient *client)
{
    delete(client);
    return 0;
}

int CoasterSWIGClientSettings(CoasterClient *client, char *settings)
{
    Settings s;
    cout << "CoasterSWIGClientSettings(" << settings << ")" <<endl;
    cout << "Client : [" << (void *) client << "]"<<endl;

    // Parsing logic
    // K1=V1, K2=V2 is the format of the settings string
    std::vector<std::string> elems;
    std::stringstream ss(settings);
    std::string item, key, value;

    while (std::getline(ss, item, ',')) {
        elems.push_back(item);
        std::stringstream kv(item);
        std::string kv_item;
        std::getline(kv, kv_item, '=');
        key = kv_item;
        std::getline(kv, kv_item);
        value = kv_item;
        s.set(key, value);
        cout << "Key,Value : " << key <<", "<<value << endl;
    }

    client->setOptions(s);
    return 0;
}

Job* CoasterSWIGJobCreate(char *cmd_string)
{
    cout << "CoasterSWIGJobCreate("<< cmd_string <<") "<< endl;
    Job *job = new Job(cmd_string);
    return job;
}

int CoasterSWIGSubmitJob(CoasterClient *client, Job* job)
{
    client->submit(*job);
    client->waitForJob(*job);
    int status = job->getStatus()->getStatusCode();
    cout << "SubmitJob returns code :" << status << endl;
    return 0;
}

int CoasterSWIGWaitForJob(CoasterClient *client, Job *job)
{
    client->waitForJob(*job);
    return 0;
}


/**
 * This is a test function.
 */
int CoasterSWIGTest (CoasterLoop *loop, char *serviceURL, CoasterClient *client)
{
	try {
        //CoasterClient* client = new CoasterClient(serviceURL, *loop);

        //CoasterClient* ptr = &client;
        CoasterClient* ptr = client;
        //client.start();
        //CoasterClient* client = new CoasterClient(serviceURL, *loop);

		Settings s;
		s.set(Settings::Key::SLOTS, "1");
		s.set(Settings::Key::MAX_NODES, "1");
		s.set(Settings::Key::JOBS_PER_NODE, "2");

		//client->setOptions(s);
        //client.setOptions(s);
        ptr->setOptions(s);

		Job j1("/bin/date");
		Job j2("/bin/echo");
		j2.addArgument("testing");
		j2.addArgument("1, 2, 3");


		ptr->submit(j1);
		ptr->submit(j2);

		ptr->waitForJob(j1);
		ptr->waitForJob(j2);
		list<Job*>* doneJobs = ptr->getAndPurgeDoneJobs();
        /*
        client.submit(j1);
		client.submit(j2);

		client.waitForJob(j1);
		client.waitForJob(j2);
		list<Job*>* doneJobs = client.getAndPurgeDoneJobs();
        */
		delete doneJobs;

		if (j1.getStatus()->getStatusCode() == FAILED) {
			cerr << "Job 1 failed: " << *j1.getStatus()->getMessage() << endl;
		}
		if (j2.getStatus()->getStatusCode() == FAILED) {
			cerr << "Job 2 failed: " << *j2.getStatus()->getMessage() << endl;
		}

		cout << "All done" << endl;

		return EXIT_SUCCESS;
	}
	catch (exception& e) {
		cerr << "Exception caught: " << e.what() << endl;
		return EXIT_FAILURE;
	}
}


