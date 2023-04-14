#include "WZMSInterface.h"
#include "WZMSCore.h"
#include "lmic.h"

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t pins[12] = {
	0xB0,		//nss
	UNUSED_PIN,	//rxtx
	0xA1,		//reset
	0xB1,		//dio0
	0xA0,		//dio1
	UNUSED_PIN,		//dio2
};
HalPinmap_t lmic_pins;

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
u1_t APPEUI[8]={ 0x95, 0x72, 0x02, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) { }//memcpy(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
//static const u1_t DEVEUI[8]={ 0x58, 0xB2, 0x89, 0x21, 0xDE, 0x3D, 0xBE, 0x9C };
u1_t DEVEUI[8]={ 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
void os_getDevEui (u1_t* buf) { memcpy(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
u1_t APPKEY[16] = { 0xA7, 0x7B, 0x8B, 0xAB, 0x23, 0x6C, 0xCD, 0x94, 0x33, 0xB6, 0xD6, 0x37, 0xC2, 0xBE, 0xBE, 0xC0 };
void os_getDevKey (u1_t* buf) {  memcpy(buf, APPKEY, 16);}

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).

const unsigned TX_INTERVAL = 1000;

uint8_t Joined = 0;
static uint8_t mydata[] = "Hello, world!";
static osjob_t sendjob;
void do_send(osjob_t* j);

void onEvent (ev_t ev) {
    xprintf("\r\n[%lu] %lu: ",micros(),os_getTime());
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            xprintln("EV_SCAN_TIMEOUT");
            break;
        case EV_BEACON_FOUND:
            xprintln("EV_BEACON_FOUND");
            break;
        case EV_BEACON_MISSED:
            xprintln("EV_BEACON_MISSED");
            break;
        case EV_BEACON_TRACKED:
            xprintln("EV_BEACON_TRACKED");
            break;
        case EV_JOINING:
            xprintln("EV_JOINING");
            Joined = 0;
            break;
        case EV_JOINED:
            xprintln("EV_JOINED");
            {
              Joined = 1;//joining was successful.
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              xprintf("netid: %d\r\n",netid);
              xprintf("devaddr: %08X\r\n",devaddr);
              xprintf("artKey: ");
              for (size_t i=0; i<sizeof(artKey); ++i) {
                xprintf("%02X",artKey[i]);
              }
              xprintln("");
              xprintf("nwkKey: ");
              for (size_t i=0; i<sizeof(nwkKey); ++i) {
                xprintf("%02X",nwkKey[i]);
              }
              xprintln("");
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
	    // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     xprintln("EV_RFU1");
        ||     break;
        */
        case EV_JOIN_FAILED:
            xprintln("EV_JOIN_FAILED");
            Joined = 2;//failed
            break;
        case EV_REJOIN_FAILED:
            xprintln("EV_REJOIN_FAILED");
            Joined = 2;//failed
            break;
        case EV_TXCOMPLETE:
			      debug_led(1);
            xprintln("EV_TXCOMPLETE (includes waiting for RX windows)");
            if (LMIC.txrxFlags & TXRX_ACK)
              xprintln("Received ack");
            if (LMIC.dataLen) {
              xprintf("Received %d bytes of payload\n",LMIC.dataLen);
            }
            Joined = 1;//joining was successful.
            break;
        case EV_LOST_TSYNC:
            xprintln("EV_LOST_TSYNC");
            break;
        case EV_RESET:
            xprintln("EV_RESET");
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            xprintln("EV_RXCOMPLETE");
            break;
        case EV_LINK_DEAD:
            xprintln("EV_LINK_DEAD");
            break;
        case EV_LINK_ALIVE:
            xprintln("EV_LINK_ALIVE");
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    xprintln("EV_SCAN_FOUND");
        ||    break;
        */
        case EV_TXSTART:
            xprintln("EV_TXSTART");
            break;
        case EV_TXCANCELED:
            xprintln("EV_TXCANCELED");
            break;
        case EV_RXSTART:
            xprintln("EV_RXSTART");
            break;
        case EV_JOIN_TXCOMPLETE:
            xprintln("EV_JOIN_TXCOMPLETE");
            break;
        default:
            xprintf("Unknown event: %d\r\n",(unsigned) ev);
            break;
    }
}
void do_send(osjob_t* j){
    // Schedule next transmission
		//os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        xprintln("OP_TXRXPEND, not sending");
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        xprintln("===========================Packet queued");
    }
    // Next TX is scheduled after TX_COMPLETE event.
}
/*void LMICOS_logEvent(const char *pMessage){
	xprintf("Syslog : %s\r\n",pMessage);
}
void LMICOS_logEventUint32(const char *pMessage, uint32_t datum){
	xprintf("Syslog : %s 0x%04X\r\n",pMessage,datum);
}*/



void WZMSInt_Loop()
{
	os_runloop_once();
}

void WZMSInt_preset()
{
    WZMSInterval = Data_SystemInfo->sendtime*1000;
	memcpy(DEVEUI, Data_SystemConst->DEVEUI, 8);
	memcpy(APPKEY, Data_SystemInfo->APPKEY, 16);
    DeviceCode = DEVEUI;
    NwkCode = APPKEY;
}


// Setup for WZMS Interface (LoRa).
int  WZMSInt_init()
{
    lmic_pins = byte2Pinmap(pins);
    
    WZMSInt_EmergencyTime = 5000;
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    // Start job (sending automatically starts OTAA too)
    // do_send(&sendjob);
    LMIC_startJoining();
    return 0;
}

// Get from Interfacing Module.
int  WZMSInt_stat()
{
    return Joined; 
}

// Sends the packet to the Module.
// Get from Interfacing Module.
void WZMSIntBase_Data()
{
    // Code_SendBuf
    WZMSLora_Data();
}
void WZMSLora_Data()
{
	println("Data : %s",printBin(Code_SendBuf,SendBuflen));

    if (LMIC.opmode & OP_TXRXPEND) {
        xprintln("OP_TXRXPEND, not sending");
		return;
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, Code_SendBuf, SendBuflen, 0);
        xprintf("Packet Sended.\r\n data : ");
		printBin(Code_SendBuf,SendBuflen);
		println("");
    }
}

void WZMSInt_ReConnect() {
    Joined = 0;
    LMIC_unjoinAndRejoin();
}

// Resetup the Module. (after the Masterkey Interfacing)
void WZMSInt_Resetup()
{
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
}

#ifdef __cplusplus
}
#endif