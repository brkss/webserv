#include "../inc/defaultConfig.hpp"

const std::string DefaulConfig::ERROR_LOG 				= "error.log";
const std::string DefaulConfig::ROOT					= "__REQUIRED";
const std::string DefaulConfig::INDEX	 				= "index.html";
const bool		  DefaulConfig::AUTOINDEX 				= true;
const bool			DefaulConfig::ALLOW_UPLOADS			= false;
const size_t 	  DefaulConfig::CLIENT_MAX_BODY_SIZE	= 42; // 200MB
const size_t 	  DefaulConfig::CLIENT_REQUEST_TIMOUT	= 60; // in seconds 
const std::string DefaulConfig::ADDRESS					= "__REQUIRED"; 
const short		  DefaulConfig::PORT 					= REQUIRED;
const std::string DefaulConfig::SERVER_NAME				= "__REQUIRED" ;
const std::string DefaulConfig::UPLOAD_STORE			= "";
