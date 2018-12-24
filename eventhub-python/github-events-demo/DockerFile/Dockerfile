FROM python:3.6

RUN apt-get install -y git

RUN pip install requests azure-eventhub

COPY ./run.sh /home/

RUN chmod a+x /home/run.sh

CMD bash /home/run.sh 