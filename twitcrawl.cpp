#include "twitcrawl.h"
#include "api.h"

int main( int argc, char* argv[] )
{
    twitCurl twitterObj;
    std::string tmpStr, tmpStr2;
    std::string replyMsg;
    char tmpBuf[1024];

    /* Lets get authenticated */
    twitterObj.getOAuth().setConsumerKey( std::string( CONSUMER_KEY ) );
    twitterObj.getOAuth().setConsumerSecret( std::string( CONSUMER_SECRET ) );

    twitterObj.getOAuth().setOAuthTokenKey( ACCESS_TOKEN );
    twitterObj.getOAuth().setOAuthTokenSecret( ACCESS_TOKEN_SECRET );

#if 0
    /* Post a new status message */
    memset( tmpBuf, 0, 1024 );
    printf( "\nEnter a new status message: " );
    gets( tmpBuf );
    tmpStr = tmpBuf;
    replyMsg = "";
    if( twitterObj.statusUpdate( tmpStr ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        printf( "\nresponse:\n%s\n", replyMsg.c_str() );
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        fprintf( stderr, "\nerror:\n%s\n", replyMsg.c_str() );
        exit(0);
    }
#endif

    /* Search with keyword */
    printf( "\nEnter string to search: " );
    memset( tmpBuf, 0, 1024 );
    gets( tmpBuf );
    tmpStr = tmpBuf;
#ifdef LIMIT_SEARCH
    printf( "\nLimit search results to: " );
    memset( tmpBuf, 0, 1024 );
    gets( tmpBuf );
    tmpStr2 = tmpBuf;
#endif
    replyMsg = "";
    if( twitterObj.search( tmpStr, "" ) )
    {
        twitterObj.getLastWebResponse( replyMsg );

        tmpStr += std::string(".json");
        std::ofstream replyMsgOut;

        replyMsgOut.open( tmpStr.c_str(), std::ofstream::out );
        replyMsgOut.clear();

        replyMsgOut << replyMsg.c_str();
        replyMsgOut.close();
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        fprintf( stderr, "\nerror:\n%s\n", replyMsg.c_str() );
    }

    return 0;
}
