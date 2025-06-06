#include "Client.hpp"


Client client("127.0.0.1", 12345);


std::unordered_map<std::string,std::string> SELECT_FROM_WHERE(std::string select, std::string from,std::string where)
{
    uint64_t count_req = 1;
    BinaryProtocol::PacketRequest p(BinaryProtocol::CommandType::SQL,count_req++);
    using tags = BinaryProtocol::SQL_Tags;
    p.addData(tags::SELECT, select);
    p.addData(tags::FROM, from);
    p.addData(tags::WHERE, where);
    
    return client.sendCommand(p);
}

int main() {

    for (const auto& initFunc : initStaticFunctions) {
        initFunc(nullptr);
    }

    std::shared_ptr<RecordTest2> test = RecordTest2::Open(1,SELECT_FROM_WHERE);

    // uint64_t count_req = 1;
    // BinaryProtocol::Packet p(BinaryProtocol::CommandType::SQLR,count_req++);
    // using tags = BinaryProtocol::SQL_Tags;
    // p.addData(tags::SELECT, "*");
    // p.addData(tags::FROM, "r11recordtest2");
    // p.addData(tags::WHERE, "id = 1");


    // BinaryProtocol::Packet pr = client.sendCommand(p);

    //client.sendCommand(BinaryProtocol::CommandType::CREATE_TABLE, "test_create_table (id INTEGER NOT NULL PRIMARY KEY);");
    //client.sendCommand(BinaryProtocol::CommandType::ADD_COLUMN, "test_create_table (id INTEGER NOT NULL PRIMARY KEY);");
    return 0;
}
