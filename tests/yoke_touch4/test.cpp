#include <yoke_touch4/yoke_touch4.hpp>

int main() {
    Yoke_touch4 t;
    assert(t.begin(nullptr)==YOKE_PORT_ERROR_NOT_CONFIGURED);
    assert(t.end()==YOKE_PORT_ERROR_NOT_INITIALIZED);
    uint16_t v=99;
    assert(!t.getVersion(&v) && v==99);
    std::vector<uint8_t> shortp={0,0,0};
    assert(t.begin(&shortp)==YOKE_PORT_ERROR_NOT_CONFIGURED);
    std::vector<uint8_t> port={0,0,0,2,17,18};
    port[1]=1;assert(t.begin(&port)==YOKE_PORT_ERROR_INVALID);port[1]=0;
    port[0]=2;assert(t.begin(&port)==YOKE_PORT_ERROR_INVALID);port[0]=0;
    assert(t.begin(&port)==0 && port[1]==1);
    assert(t.begin(&port)==YOKE_PORT_ERROR_ALREADY_INITIALIZED);
    assert(t.getVersion(&v) && v==0x1001);
    assert(t.setEnabledChannels(15) && sent[2]==0x22 && sent[4]==15);
    assert(!t.setEnabledChannels(16));
    assert(t.readData() && t.hasData() && t.getTouchMask()==5);
    assert(t.isTouched(0) && !t.isTouched(1) && t.isTouched(2) && !t.isTouched(4));
    assert(t.getRawData(0)==0x1234 && t.getRawData(1)==0xABCD);
    assert(t.getRawData(2)==1 && t.getRawData(3)==0xFFFE && t.getRawData(4)==0);
    for(int bad : {1,2,3,4,5,8,9}) {
        mode=bad;assert(!t.readData(150));assert(t.getRawData(0)==0x1234);
        mode=0;assert(t.readData());
    }
    mode=6;assert(t.readData());mode=0;
    uint8_t config[8]={};assert(t.getChannelConfig(3,config));
    assert(memcmp(config,cfgbytes,8)==0);
    assert(t.setChannelConfig(2,config)); // Includes trailing version reply.
    assert(t.reconfigure() && t.setThresholds(config));
    assert(!t.getChannelConfig(4,config) && !t.setChannelConfig(0,nullptr));
    assert(!t.setThresholds(nullptr));
    mode=7;assert(t.readData() && t.getTouchMask()==0);mode=0;
    assert(t.end()==0 && !installed && port[1]==0 && !t.hasData());
    assert(t.begin(&port)==0 && t.readData());assert(t.end()==0);
    puts("PASS: UART frames/CRC, cache, byte order, command ACKs, malformed frames, timeouts, resync, and port lifecycle");
}
