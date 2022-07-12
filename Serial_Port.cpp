

#include "Serial_Port.hpp"




Serial_Port::Serial_Port(  const std::string &s ,uint32_t baud_rate , uint8_t char_s , boost::asio::serial_port_base::parity::type par  ,
                          boost::asio::serial_port_base::stop_bits::type stopb ,
                          boost::asio::serial_port_base::flow_control::type flow  )
:opt(*new Serial_Port_Opt_Struct )
{

    this->msi = new boost::asio::io_context();
    this->bsp = new boost::asio::basic_serial_port<>( *(this->msi) , s );

    this->opt.br = baud_rate ;
    this->opt.cs = char_s ;
    this->opt.stopb = stopb ;
    this->opt.par = par ;
    this->opt.flowc = flow ;

    this->Update_Options();


}

int32_t Serial_Port::Open ( const std::string &s )
{
    boost::system::error_code errc ;
    this->bsp->open( s , errc );

    if( errc.failed() == true )
        std::cout << errc.message() << std::endl ;

    return errc.value() ;

}


int32_t Serial_Port::Write(  std::vector<uint8_t> &rb )
{
    boost::system::error_code errc ;
    boost::asio::mutable_buffer buff( &rb[0] , rb.size()) ;

    this->bsp->write_some( buff , errc ) ;

    if( errc.failed() == true )
        std::cout << errc.message() << std::endl ;

    return errc.value() ;


}

int32_t Serial_Port::Read ( std::vector<uint8_t> &rb  ){


    boost::system::error_code errc ;
    boost::asio::mutable_buffer buff ;

    this->bsp->read_some( buff ,  errc );

    if( errc.failed() == true )
        std::cout << errc.message() << std::endl ;

    for ( int32_t i = 0 ; i < buff.size() ; i++ )
        rb.push_back( *( static_cast<uint8_t*>(buff.data()) +  i ) );

    return errc.value() ;


}

int32_t Serial_Port::Set_Options ( const Serial_Port_Opt_Struct &opts )
{

    this->opt.br    = opts.br    ;
    this->opt.cs    = opts.cs    ;
    this->opt.par   = opts.par   ;
    this->opt.stopb = opts.stopb ;
    this->opt.flowc = opts.flowc ;

    this->Update_Options();

}


void Serial_Port::Update_Options( void )
{
    this->bsp->set_option( boost::asio::basic_serial_port<>::baud_rate(this->opt.br));
    this->bsp->set_option( boost::asio::basic_serial_port<>::character_size(this->opt.cs));
    this->bsp->set_option( boost::asio::basic_serial_port<>::stop_bits(this->opt.stopb));
    this->bsp->set_option( boost::asio::basic_serial_port<>::flow_control(this->opt.flowc));
    this->bsp->set_option( boost::asio::basic_serial_port<>::parity(this->opt.par));
}


bool Serial_Port::isOpen( void )
{
    return this->bsp->is_open () ;

}


void Serial_Port::Close ( void )
{
    this->bsp->close();
}
