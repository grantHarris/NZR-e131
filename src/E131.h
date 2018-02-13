#ifndef __E131_H__
#define __E131_H__

class E131 {
public:
    E131();
    void receive_data();
private:
    YAML::Node *config;
    void join_universe();
    int sockfd;
    e131_packet_t packet;
    e131_error_t error;
    uint8_t last_seq = 0x00;
};

#endif /* __E131_H__ */