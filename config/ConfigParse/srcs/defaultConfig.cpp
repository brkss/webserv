#include "../inc/defaultConfig.hpp"

const std::string DefaulConfig::ERROR_LOG 				= "error.log";
const std::string DefaulConfig::ROOT					= "/var/www/";
const std::string DefaulConfig::INDEX	 				= "index.html";
const bool		  DefaulConfig::AUTOINDEX 				= true;
const size_t 	  DefaulConfig::CLIENT_MAX_BODY_SIZE	= 1;
const size_t 	  DefaulConfig::CLIENT_REQUEST_TIMOUT	= 60; // in seconds 
const std::string DefaulConfig::ADDRESS					= "__REQUIRED"; 
const short		  DefaulConfig::PORT 					= REQUIRED;
const std::string DefaulConfig::SERVER_NAME				= "__REQUIRED" ;
const std::string DefaulConfig::UPLOAD_STORE			= "/tmp";
