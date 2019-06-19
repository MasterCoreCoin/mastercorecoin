#!/bin/bash

# by enakee

# :: TODO ::
# ./autogen.sh \
# ./configure \
# make -j 2 \
# cd /bin/tar -zxvf bin.tar.gz ./* \

# :: GOTO ::
#  github release page and upload upon tar.archive, this shell script and type wget -qO- https://github.com/mastercorecoin/mastercorecoin/releases/download/start_mn_installer.sh | bash

declare -r COIN_NAME='mastercorecoin' # by enakee
declare -r COIN_CORE='mastercorecoincore' # by enakee
declare -r COIN_DAEMON="${COIN_NAME}d"
declare -r COIN_CLI="${COIN_NAME}-cli"
declare -r COIN_PATH='/usr/bin/'
declare -r COIN_ARH='https://github.com/MasterCoreCoin/mastercorecoin/releases/download/1.0.0.0/bin.tar.gz' # by enakee
declare -r COIN_TGZ=$(echo ${COIN_ARH} | awk -F'/' '{print $NF}')
declare -r COIN_PORT=29871
declare -r COIN_RPC_PORT=29872
declare -r CONFIG_FILE="${COIN_NAME}.conf"
declare -r CONFIG_FOLDER="${HOME}/.${COIN_CORE}"
declare -r SERVICE_FILE="/etc/systemd/system/${COIN_DAEMON}.service"
declare -r TMP_FOLDER="${HOME}/${COIN_NAME}/tmp"
declare -r RED='\033[0;31m'
declare -r GREEN='\033[0;32m'
declare -r NC='\033[0m'

function check_system() {
    echo -e "* Checking system for compatibilities"
if [[ $EUID -ne 0 ]]; then
   echo -e "${RED}$0 must be run as root.${NC}"
   exit 1
fi

if [[ -r /etc/os-release ]]; then
    . /etc/os-release
	echo -e "Verision ${VERSION_ID}"
	if [[ "${VERSION_ID}" != "16.04" ]] ; then
		echo -e "This script only supports Ubuntu 16.04 LTS, exiting."
		exit 1
	fi
else
    echo -e "This script only supports Ubuntu 16.04 LTS, exiting."
    exit 1
fi
MAIN_IP=$(wget -qO- https://api.ipify.org)
}

function create_mn_dirs() {
    echo -e "* Creating masternode directories"
    mkdir -p ${CONFIG_FOLDER}
    mkdir -p ${TMP_FOLDER}
}

function download_binary(){
    echo -e "* Download binary files"
	cd 	${TMP_FOLDER}
	rm -f ${COIN_TGZ}
	if [[ ! -f "${TMP_FOLDER}/${COIN_TGZ}" ]]; then
		wget ${COIN_ARH}
	fi
	if [[ -f "${TMP_FOLDER}/${COIN_TGZ}" ]]; then
		tar xvzf "${TMP_FOLDER}/${COIN_TGZ}" --strip-components=1 -C ${TMP_FOLDER}
		chmod +x $COIN_DAEMON $COIN_CLI
		mv $COIN_DAEMON $COIN_CLI $COIN_PATH/
		rm -Rf ${TMP_FOLDER}/*
	fi
}

function install_packages() {
    echo -e "* Package installation"

# by enakee
   sudo apt-get install -y build-essential libtool bsdmainutils autotools-dev autoconf pkg-config automake python3
   sudo apt-get install -y libssl-dev libgmp-dev libevent-dev libboost-all-dev
   sudo apt-get install -y software-properties-common
   sudo add-apt-repository -yu ppa:bitcoin/bitcoin
   sudo apt-get update -y
   sudo apt-get install -y libdb4.8-dev libdb4.8++-dev libboost1.58-all-dev libdb5.3++-dev libminiupnpc-dev

    # only for 18.04 // openssl
if [[ "${VERSION_ID}" == "18.04" ]] ; then
       apt-get -qqy -o=Dpkg::Use-Pty=0 -o=Acquire::ForceIPv4=true install libssl1.0-dev
fi
}

function createswap() {
    echo -e "* Check if swap is available"
if [[  $(( $(wc -l < /proc/swaps) - 1 )) > 0 ]] ; then
    echo -e "All good, you have a swap"
else
    echo -e "No proper swap, creating it"
    rm -f /var/swapfile.img
    dd if=/dev/zero of=/var/swapfile.img bs=1024k count=2000
    chmod 0600 /var/swapfile.img
    mkswap /var/swapfile.img
    swapon /var/swapfile.img
    echo '/var/swapfile.img none swap sw 0 0' | tee -a /etc/fstab
    echo 'vm.swappiness=10' | tee -a /etc/sysctl.conf
    echo 'vm.vfs_cache_pressure=50' | tee -a /etc/sysctl.conf
fi
}

function create_config() {
    echo -e "* Creating config file ${CONFIG_FILE}"
if [[ ! -d "${CONFIG_FOLDER}" ]]; then mkdir -p ${CONFIG_FOLDER}; fi

if [[ ! -f "${CONFIG_FOLDER}/${CONFIG_FILE}" ]]; then
	RPCUSER=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 10 | head -n 1)
	RPCPASS=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 20 | head -n 1)
	printf "%s\n" "rpcuser=${RPCUSER}" "rpcpassword=${RPCPASS}" "rpcport=${COIN_RPC_PORT}" "rpcallowip=127.0.0.1" "listen=1" "server=1" "daemon=1" > ${CONFIG_FOLDER}/${CONFIG_FILE}
	${COIN_DAEMON}
	sleep 30
	if [ -z "$(ps axo cmd:100 | grep ${COIN_DAEMON})" ]; then
	   echo -e "${RED}${COIN_NAME} server couldn not start. Check /var/log/syslog for errors.{$NC}"
	   exit 1
	fi
    MNPRIVKEY=$(${COIN_CLI} masternode genkey)
	if [ "$?" -gt "0" ];
		then
		echo -e "${RED}Wallet not fully loaded. Let us wait and try again to generate the Private Key${NC}"
		sleep 30
		MNPRIVKEY=$(${COIN_CLI} masternode genkey)
	fi
    ${COIN_CLI} stop
	printf "%s\n" "" "externalip=${MAIN_IP}" "masternodeaddr=${MAIN_IP}:${COIN_PORT}" "masternode=1" "masternodeprivkey=${MNPRIVKEY}" >> ${CONFIG_FOLDER}/${CONFIG_FILE}
else
    echo -e "* Config file ${CONFIG_FILE} already exists!"
    . "${CONFIG_FOLDER}/${CONFIG_FILE}"
	MNPRIVKEY=${masternodeprivkey}
fi
}

function create_systemd_service() {

cat > ${SERVICE_FILE} <<-EOF
[Unit]
Description=${COIN_DAEMON} distributed currency daemon
After=network.target

[Service]
User=$(whoami)
Group=$(id -gn)

Type=forking
PIDFile=${CONFIG_FOLDER}/${COIN_DAEMON}.pid
ExecStart=${COIN_PATH}/${COIN_DAEMON} -pid=${CONFIG_FOLDER}/${COIN_DAEMON}.pid
ExecStop=${COIN_PATH}/${COIN_CLI} stop
PrivateTmp=true

[Install]
WantedBy=multi-user.target
EOF

if [ -n "$(ps axo cmd:100 | grep ${COIN_DAEMON})" ]; then
	${COIN_CLI} stop
	sleep 3
fi

	${COIN_DAEMON}

sleep 3
}

function display_information() {
 echo
 echo -e "================================================================================================================================"
 echo -e "${COIN_NAME} Masternode is up and running listening on port ${GREEN}${COIN_PORT}${NC}."
 echo -e "Configuration folder is: ${RED}$CONFIG_FOLDER${NC}"
 echo -e "Configuration file is: ${RED}$CONFIG_FOLDER/$CONFIG_FILE${NC}"
 echo -e "Configuration masternode file is: ${RED}$CONFIG_FOLDER/masternode.conf${NC}"
 echo -e "Node start: ${RED}${COIN_DAEMON}${NC}"
 echo -e "Node stop: ${RED}${COIN_DAEMON} stop{NC}"
 echo -e "VPS_IP:PORT ${RED}${MAIN_IP}:${COIN_PORT}${NC}"
 echo -e "MASTERNODE PRIVATEKEY is: ${RED}${MNPRIVKEY}${NC}"
 echo -e "Please check ${GREEN}$COIN_NAME${NC} is running with the following command: ${GREEN}${COIN_CLI} getinfo${NC}"
 echo -e "================================================================================================================================"
}

check_system
createswap
install_packages
create_mn_dirs
download_binary
create_config
create_systemd_service
display_information
