#!/bin/bash

cd /home/

git clone https://github.com/Azure/azure-kusto-demos.git 

cp /home/config/EventHubsConfig.json /home/azure-kusto-demos/eventhub-python/github-events-demo/config/

cd /home/azure-kusto-demos/eventhub-python/github-events-demo/

python crawl_send_eventhub.py &> log