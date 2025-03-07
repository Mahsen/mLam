docker run -d -v ollama:/root/.ollama --name ollama ollama/ollama
or docker run -d --gpus=all -v ollama:/root/.ollama --name ollama ollama/ollama

docker ps -a
docker start 192a95a55570
docker exec -it 192a95a55570 "/bin/bash"
docker exec -it 192a95a55570 "/bin/bash"

apt update
apt install -y busybox
apt install -y nano
apt install -y g++
apt install -y curl
apt install -y libcurl4-openssl-dev
apt install -y libkrb5-dev
apt install -y libssl-dev
apt install -y net-tools
apt install -y wget
apt install -y make
apt install -y build-essential 
apt install -y libpcre3 
apt install -y libpcre3-dev 
apt install -y zlib1g 
apt install -y zlib1g-dev 
apt install -y lsof
apt install -y socat


ollama ps
ollama run phi3:medium

mkdir -p /www
mkdir -p /www/cgi-bin

mkdir -p /etc/stunnel
openssl req -new -x509 -days 365 -nodes -out /etc/stunnel/stunnel.pem -keyout /etc/stunnel/stunnel.pem
chmod 600 /etc/stunnel/stunnel.pem
socat openssl-listen:443,reuseaddr,cert=/etc/stunnel/stunnel.pem,verify=0,fork tcp-connect:127.0.0.1:80

cat > /root/start.sh <<EOF
#!/bin/sh

# Start the BusyBox HTTP server on port 80
busybox httpd -f -p 80 -h /www &

# Start socat to handle HTTPS on port 443 and forward to HTTP (port 80)
socat openssl-listen:443,reuseaddr,cert=/etc/server.pem,verify=0,fork tcp-connect:127.0.0.1:80 &
EOF

chmod +x /root/start.sh
echo "/root/start.sh &" >> ~/.bashrc

exit
docker restart 192a95a55570
docker exec -it 192a95a55570 "/bin/bash"

docker export 192a95a55570 > App.tar
docker import App.tar mlam
docker run -t -i mlam /bin/bash
docker ps -a

