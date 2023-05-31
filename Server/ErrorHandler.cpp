#include "ErrorHandler.hpp"

const char * ErrorMessage::ERROR_400 = "400 Bad Request";
const char * ErrorMessage::ERROR_414 = "414 (Request-URI Too Long)";
const char * ErrorMessage::ERROR_505 = "505 (HTTP Version Not Supported)";


const short	ErrorNumbers::_400_BAD_REQUEST						= 400;
const short ErrorNumbers::_408_REQUEST_TIMEOUT					= 408;
const short ErrorNumbers::_411_LENGTH_REQUIRED					= 411;
const short ErrorNumbers::_414_URI_TOO_LONG						= 414;
const short	ErrorNumbers::_413_PAYLOAD_TOO_LARGE 				= 413;
const short ErrorNumbers::_431_REQUEST_HEADER_FIELDS_TOO_LARGE	= 431;
const short	ErrorNumbers::_500_INTERNAL_SERVER_ERROR			= 500;
const short	ErrorNumbers::_501_NOT_IMPLEMENTED					= 501;
const short	ErrorNumbers::_505_HTTP_VERSION_NOT_SUPPORTED 		= 505;


