#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <utility>
#include <bitset>
#include <stdint.h>
#include <math.h>
#include <fstream>
#include <boost/asio.hpp>
#include "Serial_Port.hpp"

#define MAX_BUFF_LEN 32768


std::vector<uint8_t> s_buff ;





int main ( int argc , char*argv[] ) {

/* Argument List  */
// name of COM Port
// Baud rate
// Samples File
std::vector<std::string> arg_v( argc );
for ( int i = 0  ; i < argc ; i++ )
    arg_v[i] = std::string(*(argv+i));



Serial_Port sp( std::string(arg_v[1]) , std::stoi(std::string(arg_v[2])) );

if ( sp.isOpen() == false ){

    std::cout << arg_v[0] << " Is not a valid COM-Port ..." << std::endl ;
    return -1 ;

}

std::fstream  fs ;
//IN readonly
fs.open( arg_v[3] , std::ios_base::in  );
if ( fs.is_open() == false ){
     std::cout << "Sample file open failure " << std::endl ;
    return -1 ;
}


/* Parse Sample File */
int16_t c ;
int16_t c_prev ;
while ( (c_prev = fs.get() ) != '{'   &&  c!= EOF   );



if ( c_prev == EOF ){
    std::cout << "Invalid Sample File Syntax" << std::endl;
    return -1 ;
}

std::string s ;
s.clear();
for (  ; ( (c = fs.get() )!= '}' && c != EOF ) && s_buff.size() < MAX_BUFF_LEN ; c_prev = c  )
{
    if(   c  <= '9' && c >= '0' )
        s.push_back((uint8_t)c);
    else{
        if (  c_prev  <= '9' && c_prev >= '0'){
        uint32_t sample = std::stoi(s) ;
        s_buff.push_back( ( r > 255 ? (uint8_t)255u : (uint8_t)sample ) ) ;
        s.clear();
        }
        else
            continue ;

}
}


if ( s_buff.size() <  MAX_BUFF_LEN )
{
    for (  ; s_buff.size() < MAX_BUFF_LEN ;   )
        s_buff.push_back(0);

}




fs.close();

/* Start Communication */
std::vector<uint8_t> s_w ;
s_w.push_back('D') ;
sp.Write(s_w);
s_w.clear();
sp.Read(s_w);
if ( !s_w.size() ||( s_w.size() && s_w[0] != 'D')  ){
    std::cout << "Device Didn't Respond or Invalid Response " << std::endl ; return -1 ; }
s_w.clear(); s_w.push_back('S') ;
sp.Write( s );
sp.Write( s_buff );


sp.Close();


  return 0 ;





}
