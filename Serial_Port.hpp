









#ifndef SERIAL_PORT_TOKEN
#define SERIAL_PORT_TOKEN 1UL



#include <boost/asio.hpp>
#include <string>
#include <iostream>



typedef struct {


    boost::asio::serial_port_base::parity::type par ;
    boost::asio::serial_port_base::stop_bits::type stopb ;
    boost::asio::serial_port_base::flow_control::type flowc;
    uint32_t cs ;
    uint32_t br ;


} Serial_Port_Opt_Struct ;

class Serial_Port {

private :

    boost::asio::io_context *msi ;
    boost::asio::basic_serial_port<> *bsp ;
    Serial_Port_Opt_Struct opt ;



    void Update_Options( void );


public :
    Serial_Port(  const std::string &s  , uint32_t baud_rate , uint8_t char_s = 8u , boost::asio::serial_port_base::parity::type par = boost::asio::serial_port_base::parity::type::even  ,
                          boost::asio::serial_port_base::stop_bits::type stopb =  boost::asio::serial_port_base::stop_bits::type::one ,
                          boost::asio::serial_port_base::flow_control::type flow =  boost::asio::serial_port_base::flow_control::type::none );


    int32_t Open (const std::string& s  );


    int32_t Write( std::vector<uint8_t> &rb );


    int32_t Read ( std::vector<uint8_t> &rb  );


    int32_t Set_Options ( const Serial_Port_Opt_Struct &opts );


    bool isOpen( void );


    void Close ( void );


    ~Serial_Port()
    {
        this->bsp->~basic_serial_port();
        this->msi->~io_context();

    }




};










#endif
