#ifndef __JsonParser_h__
#define __JsonParser_h__

// local headers
#include <Json/Object.h>


/****************************************************************************
*
*   JSON objects parser.
*
*****************************************************************************/
class Parser
{
    public:
        static Parser& instance();

        Object* parse( const QString& jsonObject, QString& errorMessage );
        
    private:
        Parser();
        Parser( const Parser& );
        Parser operator=( const Parser& );
        ~Parser();

    private:
        jost::json_parser json_;

};

    
#endif

