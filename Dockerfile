FROM nestybox/alpine-docker

EXPOSE 8080

COPY arduino arduino
COPY rpi-pico rpi-pico
COPY main.py .
COPY relay relay
COPY requirements.txt .

RUN apk add --no-cache usbutils python3 py3-pip && \
    pip install --upgrade --ignore-installed packaging -r requirements.txt

CMD dockerd > /var/log/dockerd.log 2>&1 & waitress-serve main:app