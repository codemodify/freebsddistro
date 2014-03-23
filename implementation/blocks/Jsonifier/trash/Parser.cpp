
// locla headers
#include "Parser.h"

Parser::Parser() 
{}

Parser::~Parser() 
{}

Parser& Parser::instance()
{
    static Parser instance;
    return instance;
}

Object* Parser::parse( const QString& jsonObject, QString& errorMessage );
{
    Object* o;
    parse_info<> info = boost::spirit::parse(stream, this->json_, space_p);
    if (info.full)
        o = this->json_.parsed_object();
    else {
        std::string error_msg = "Invalid JSON Object (";
        error_msg += info.stop;
        this->json_.clean_up();
        throw jost::MalformedJSONStream(error_msg);
    }
    
    return o;
}

