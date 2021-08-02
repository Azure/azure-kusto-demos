FROM python:3.6-alpine

COPY ./run.sh /home/

RUN apk add --no-cache openssl-dev && \
    apk add --no-cache --virtual .build-deps gcc cmake openssl-dev build-base libffi-dev && \
	apk add --no-cache git && \
	apk add --no-cache bash && \
    python3 -m pip install cryptography==3.1.1 requests azure-eventhub==1.3.3 --no-cache-dir &&\ 
	chmod a+x /home/run.sh && \
	apk --purge del .build-deps
	
CMD bash /home/run.sh 